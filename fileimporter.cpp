#include "fileimporter.h"
#include <QDebug>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QFileInfo>

#include "ffmpeghandler.h"

FileImporter::FileImporter(DbManager* dbManager, QMutex* mutex, QSettings* settings) {
  this->dbManager = dbManager;
  this->semaphore = new QSemaphore();
  this->settings = settings;
  this->mutex = mutex;

  this->ffmpegHandler = new FfmpegHandler(settings);
}

void FileImporter::addMediaFolderToQueue(QString path, bool isVideo,
                                         bool isPicture) {
  MediaFolder* tempMediaFolder = new MediaFolder();
  tempMediaFolder->pathToFile = path;
  tempMediaFolder->isVideo = isVideo;
  tempMediaFolder->isPicture = isPicture;

  this->mediaFoldersQueue.enqueue(tempMediaFolder);
}

void FileImporter::walkPath(MediaFolder* mediaFolderToWalk) {
  QList<QStringList> videosToAdd;
  QList<QStringList> picturesToAdd;
  QList<QStringList> treeFoldersToAdd;

  QDirIterator it(mediaFolderToWalk->pathToFile, QDir::Files,
                  QDirIterator::Subdirectories);
  while (it.hasNext()) {
    QString fileToCheck = it.next();
    QFile f(fileToCheck);
    QFileInfo fileInfo(f.fileName());
    QString extention = fileInfo.suffix();
    QString baseName = fileInfo.baseName();
    QString absPath = fileInfo.absolutePath();
    QString pathLevel = QString::number(absPath.count('/'));
    bool somethingIsAdded = false;
    if (mediaFolderToWalk->isVideo &&
        this->videoExtentions.contains(extention)) {
      QStringList temp;
      temp.append(baseName);
      temp.append(fileToCheck);
      videosToAdd.append(temp);
      somethingIsAdded = true;
      //      FfmpegHandler::ffprobeTest(fileToCheck);
      qDebug() << "This is a *VIDEO*: " << fileToCheck;
    } else if (mediaFolderToWalk->isPicture &&
               this->pictureExtentions.contains(extention)) {
      QStringList temp;
      temp.append(baseName);
      temp.append(fileToCheck);
      picturesToAdd.append(temp);
      somethingIsAdded = true;

      qDebug() << "This is a *PICTURE*: " << fileToCheck;
    }

    if (somethingIsAdded) {
      QStringList temp;
      temp.append(absPath);
      temp.append(pathLevel);
      treeFoldersToAdd.append(temp);
    }
  }

  qDebug() << "Scanning complete, found : Scenes *" << videosToAdd.size()
           << "* Pictures *" << picturesToAdd.size() << "* Folders *"
           << treeFoldersToAdd.size() << "*";

  this->dbManager->addScenes(videosToAdd);
  this->dbManager->addPictures(picturesToAdd);
  this->dbManager->addTreeFolders(treeFoldersToAdd);

  qDebug() << "Finished walking path...";

  qDebug() << "Starting probing unprobed scenes with FFPROBE ... ";
  QList<QMap<QString, QVariant>> scenesBeforeFFprobe =
      this->dbManager->getScenesBeforeFFprobe();
  qDebug() << "Number of scenes to proble is: " << scenesBeforeFFprobe.size();
  QList<QMap<QString, QVariant>> listOfSceneUpdates;

  QElapsedTimer timer;
  timer.start();

  for (int i = 0; i < scenesBeforeFFprobe.size(); i++) {
    QMap<QString, QVariant> temp =
        ffmpegHandler->ffprobeTest(scenesBeforeFFprobe.at(i));
    if (! temp.contains("failed")) {
      listOfSceneUpdates.append(temp);
    }
  }

  qDebug() << "Finished FFprobing scenes, probing all unprobed scenes took "
           << timer.elapsed() << " ms.";

  qDebug() << "Updating FFprobed scenes in database ...";

  this->dbManager->updateScenes(listOfSceneUpdates);

//  delete scenesBeforeFFprobe;
}

void FileImporter::run() {
  qDebug() << "File importer thread started ...";
  //  this->mutex->lock();
  while (!this->mediaFoldersQueue.isEmpty()) {
    //    this->mutex->unlock();
    MediaFolder* currentMediaFolder = this->mediaFoldersQueue.dequeue();
    if (currentMediaFolder->pathToFile == "poison") {
      break;
    }
    this->walkPath(currentMediaFolder);
    delete currentMediaFolder;
  }
  qDebug() << "File importer thread exiting ...";
}
