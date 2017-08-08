#include "fileimporter.h"
#include <QDebug>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QDateTime>

#include "ffmpeghandler.h"

FileImporter::FileImporter(DbManager* dbManager, QMutex* mutex,
                           QSettings* settings)
{
    this->dbManager = dbManager;
    this->semaphore = new QSemaphore();
    this->settings = settings;
    this->mutex = mutex;
}

void FileImporter::addMediaFolderToQueue(QString path, bool isVideo,
                                         bool isPicture)
{
    QMap<QString, QVariant> tempMediaFolder;
    tempMediaFolder["path_to_dir"] = path;
    tempMediaFolder["is_video"] = isVideo;
    tempMediaFolder["is_picture"] = isPicture;
    this->mediaFoldersQueue.enqueue(tempMediaFolder);
}

void FileImporter::walkPath()
{
    int videoCounter = 1;
    int videoTotal = this->videosToAdd.size();
    int pictureCounter = 1;
    int pictureTotal = this->picturesToAdd.size();
    int folderCounter = 1;
    int folderTotal = this->treeFoldersToAdd.size();
    QElapsedTimer videoTimer;
    QElapsedTimer pictureTimer;
    QElapsedTimer folderTimer;
    QElapsedTimer totalTimer;
    totalTimer.restart();
    qDebug() << "Starting to add videos...";
    videoTimer.restart();

    while (!this->videosToAdd.isEmpty())
    {
        qDebug() << "Video " << QString::number(videoCounter) << " Out of " << QString::number(videoTotal);
        emit progressChanged((double)videoCounter / videoTotal);
        auto currentVideo = this->videosToAdd.dequeue();
        QString fileToCheck = currentVideo["fileToCheck"].toString();
        QString baseName = currentVideo["baseName"].toString();

        if (!existsInDb("Scene", "path_to_file", fileToCheck))
        {
            qInfo() << fileToCheck << " Is video and does not exist in the database.";
            QMap<QString, QVariant> videoToAddToDb;
            videoToAddToDb["name"] = baseName;
            videoToAddToDb["path_to_file"] = fileToCheck;
            int ffmpegScore =  FfmpegHandler::getSceneFfprobeInfo(videoToAddToDb, this->settings);
            qInfo() << "FFPROBE score is :" << ffmpegScore;

            if (ffmpegScore >= 25)
            {
                int nextId = getNextInsertedId("Scene");
                int createdThumb = FfmpegHandler::takeSceneScreenShot(videoToAddToDb, this->settings, nextId); //returns 1 if failed
                bool generateContactSheet = !settings->value("ffmpeg/generatecontactsheet").toBool(); // later would be configureable in settings

                if (createdThumb == 0 && generateContactSheet)
                {
                    qDebug("Would generate contact sheet");
                    int generatedContactSheet = FfmpegHandler::generateContactSheetForScene(videoToAddToDb, this->settings, nextId); //returns 1 if failed
                }
            }

            bool success = saveToDb(videoToAddToDb, "Scene");

            if (success)
            {
                //                QString elapsedTime = QDateTime::fromTime_t(timer.elapsed() / 1000).toUTC().toString("hh:mm:ss");
                //                qDebug() << "Finished inserting scene Thumbnail " << videoToAddToDb["path_to_file"].toString() << " process took *" << elapsedTime ;
            }
        }
        else
        {
            qInfo() << fileToCheck << " Is video and *ALREADY* exist in the database.";
        }

        videoCounter++;
    }

    QString elapsedTimeVideo = QDateTime::fromTime_t(videoTimer.elapsed() / 1000).toUTC().toString("hh:mm:ss");
    qDebug() << " Finished inserting videos ... Proccess took " << elapsedTimeVideo;
    qDebug() << " Start adding pictures";
    pictureTimer.restart();

    while (!this->picturesToAdd.isEmpty())
    {
        qDebug() << "Picture " << QString::number(pictureCounter) << " Out of " << QString::number(pictureTotal);
        emit progressChanged((double)pictureCounter / pictureTotal);
        auto currentPictureToAdd = this->picturesToAdd.dequeue();
        QString fileToCheck = currentPictureToAdd["fileToCheck"].toString();
        QString baseName = currentPictureToAdd["baseName"].toString();

        if (!existsInDb("Picture", "path_to_file", fileToCheck))
        {
            QMap<QString, QVariant> pictureToAddToDb;
            pictureToAddToDb["name"] = baseName;
            pictureToAddToDb["path_to_file"] = fileToCheck;
            int ffprobescore = FfmpegHandler::getPictureFfprobeInfo(pictureToAddToDb, this->settings);
            bool success = saveToDb(pictureToAddToDb, "Picture");

            if (success)
            {
                //                QString elapsedTime = QDateTime::fromTime_t(pictureTimer.elapsed() / 1000).toUTC().toString("hh:mm:ss");
                //                qDebug() << "Finished inserting picture " << pictureToAddToDb["path_to_file"].toString() << " process took *" << elapsedTime ;
            }
        }
        else
        {
            qDebug() << "Picture " << fileToCheck << " already exists in DB";
        }

        pictureCounter++;
    }

    QString elapsedTimePictures = QDateTime::fromTime_t(pictureTimer.elapsed() / 1000).toUTC().toString("hh:mm:ss");
    qDebug() << " Finished inserting " << QString::number(pictureTotal) << " pictures ... Proccess took " << elapsedTimePictures;
    qDebug() << "Starting to add folder";

    while (!this->treeFoldersToAdd.isEmpty())
    {
        folderTimer.restart();
        qDebug() << "Folder " << QString::number(folderCounter) << " Out of " << QString::number(folderTotal);
        auto currentFolder = this->treeFoldersToAdd.dequeue();

        if (!existsInDb("TreeFolder", "path_to_dir", currentFolder["path_to_dir"].toString()))
        {
            bool success = saveToDb(currentFolder, "TreeFolder");
        }
        else
        {
            qDebug() << "Folder " << currentFolder["path_to_dir"].toString() << "already exist in db";
        }

        //        if (success)
        //        {
        //            //            QString elapsedTime = QDateTime::fromTime_t(timer.elapsed() / 1000).toUTC().toString("hh:mm:ss");
        //            //            qDebug() << "Finished inserting treefolder " << currentFolder["path_to_dir"].toString() << " process took *" << elapsedTime ;
        //        }
        folderCounter++;
    }

    folderCounter++;
    QString elapsedTimeFolders = QDateTime::fromTime_t(folderTimer.elapsed() / 1000).toUTC().toString("hh:mm:ss");
    qDebug() << " Finished inserting " << QString::number(folderTotal) << " folders ... Proccess took " << elapsedTimeFolders;
    QString elapsedTimeTotal = QDateTime::fromTime_t(totalTimer.elapsed() / 1000).toUTC().toString("hh:mm:ss");
    qDebug() << " Finished inserting everything ... process took " << elapsedTimeTotal;
    emit finishedImporting();
}



bool FileImporter::existsInDb(QString tableName, QString columnName, QString columnValue)
{
    QString stmt = QString("SELECT COUNT(*) FROM %1 WHERE %2='%3'").arg(tableName, columnName, DbManager::escapeSqlChars(columnValue));
    QList<QMap<QString, QVariant>> dbResponse = this->dbManager->executeArbitrarySqlWithReturnValue(stmt);
    qDebug() << "dbResponse[\"COUNT(*)\"] = " << dbResponse[0]["COUNT(*)"].toString();
    return dbResponse[0]["COUNT(*)"].toInt() >= 1;
}

int FileImporter::getNextInsertedId(QString tableName)
{
    QString stmt = QString("SELECT id from %1 ORDER BY id DESC LIMIT 1").arg(tableName);
    QList<QMap<QString, QVariant>> dbResponse = this->dbManager->executeArbitrarySqlWithReturnValue(stmt);

    if (dbResponse.size() > 0)
    {
        qDebug() << "dbResponse[\"id\"] = " << dbResponse[0]["id"].toString();
        return dbResponse[0]["id"].toInt() + 1;
    }
    else
    {
        return 1;
    }
}

bool FileImporter::saveToDb(QMap<QString, QVariant> objectToSave, QString tableToSaveTo)
{
    bool success = false;
    QMapIterator<QString, QVariant> i(objectToSave);
    QString csvColumns = "";
    QString csvValues = "";
    bool first = true;

    while (i.hasNext())
    {
        i.next();

        if (first)
        {
            csvColumns = csvColumns + i.key();
            csvValues = csvValues + QString("'%1'").arg(DbManager::escapeSqlChars(i.value().toString()));
            first = false;
        }
        else
        {
            csvColumns = csvColumns + "," + i.key();
            csvValues = csvValues + "," +  QString("'%1'").arg(DbManager::escapeSqlChars(i.value().toString()));
        }
    }

    QString stmt = QString("INSERT INTO %1 (%2) VALUES (%3)").arg(tableToSaveTo, csvColumns, csvValues);
    success = this->dbManager->executeArbitrarySqlWithoutReturnValue(stmt);
    return success;
}

void FileImporter::findItemsToAdd(QMap<QString, QVariant> mediaFolderToWalk)
{
    QElapsedTimer timer;
    timer.start();
    this->videosToAdd.clear();
    this->picturesToAdd.clear();
    this->treeFoldersToAdd.clear();
    QString pathToWalk = mediaFolderToWalk["path_to_dir"].toString();
    bool isVideo = mediaFolderToWalk["is_video"].toBool();
    bool isPicture = mediaFolderToWalk["is_picture"].toBool();
    QDirIterator it(pathToWalk, QDir::Files, QDirIterator::Subdirectories);
    bool somethingIsAdded = false;
    qDebug() << "Starting to scan folder " << pathToWalk << "for files to add...";

    while (it.hasNext())
    {
        somethingIsAdded = false;
        QString fileToCheck = it.next();
        //        qDebug() << "Going to add file " << fileToCheck << " to database";
        QFile f(fileToCheck);
        QFileInfo fileInfo(f.fileName());
        QString extention = fileInfo.suffix();
        QString baseName = fileInfo.completeBaseName();
        QString absPath = fileInfo.absolutePath();
        //        QString pathLevel = QString::number(absPath.count('/') + 1) ;

        if (isVideo && this->videoExtentions.contains(extention))
        {
            QMap<QString, QVariant> temp;
            temp["fileToCheck"] = fileToCheck;
            temp["baseName"] = baseName;
            this->videosToAdd.enqueue(temp);
            somethingIsAdded = true;
        }

        if (isPicture && this->pictureExtentions.contains(extention))
        {
            QMap<QString, QVariant> temp;
            temp["fileToCheck"] = fileToCheck;
            temp["baseName"] = baseName;
            this->picturesToAdd.enqueue(temp);
            somethingIsAdded = true;
        }

        if (somethingIsAdded)
        {
            QMap<QString, QVariant> temp;
            QStringList splitPath = absPath.split('/');
            QString pathSoFar = "";
            bool first = true;

            for (int i = 0 ; i < splitPath.size() ; i ++)
            {
                if (first)
                {
                    temp.clear();
                    pathSoFar = splitPath.at(i) + "/";
                    first = false;
                    temp["path_to_dir"] = pathSoFar;
                    temp["level"] = i;

                    if (!this->treeFoldersToAdd.contains(temp))
                    {
                        this->treeFoldersToAdd.append(temp);
                    }
                }
                else
                {
                    temp.clear();
                    pathSoFar = pathSoFar + splitPath.at(i) + "/";
                    temp["path_to_dir"] = pathSoFar;
                    temp["level"] = i;

                    if (!this->treeFoldersToAdd.contains(temp))
                    {
                        this->treeFoldersToAdd.append(temp);
                    }
                }
            }
        }
    }

    QString elapsedTime = QDateTime::fromTime_t(timer.elapsed() / 1000).toUTC().toString("hh:mm:ss");
    qDebug() << "Finished scanning folder " << pathToWalk;
    qDebug() << "Found " << QString::number(this->videosToAdd.size()) << " Videos and " << QString::number(this->picturesToAdd.size()) << " pictures  process took " << elapsedTime ;
    emit foundFiles(this->videosToAdd.size(), this->picturesToAdd.size(), this->treeFoldersToAdd.size());
    this->walkPath();
}

//void FileImporter::walkPath(QMap<QString, QVariant> mediaFolderToWalk)
//{
//    QList<QStringList> videosToAdd;
//    QList<QStringList> picturesToAdd;
//    QList<QStringList> treeFoldersToAdd;
//    QString pathToWalk = mediaFolderToWalk["path_to_dir"].toString();
//    bool isVideo = mediaFolderToWalk["is_video"].toBool();
//    bool isPicture = mediaFolderToWalk["is_picture"].toBool();
//    QDirIterator it(pathToWalk, QDir::Files, QDirIterator::Subdirectories);
//    while (it.hasNext())
//    {
//        QString fileToCheck = it.next();
//        QFile f(fileToCheck);
//        QFileInfo fileInfo(f.fileName());
//        QString extention = fileInfo.suffix();
//        QString baseName = fileInfo.baseName();
//        QString absPath = fileInfo.absolutePath();
//        QString pathLevel = QString::number(absPath.count('/'));
//        bool somethingIsAdded = false;
//        if (isVideo && this->videoExtentions.contains(extention))
//        {
//            QStringList temp;
//            temp.append(baseName);
//            temp.append(fileToCheck);
//            videosToAdd.append(temp);
//            somethingIsAdded = true;
//            //      FfmpegHandler::ffprobeTest(fileToCheck);
//            qDebug() << "This is a *VIDEO*: " << fileToCheck;
//        }
//        else if (isPicture && this->pictureExtentions.contains(extention))
//        {
//            QStringList temp;
//            temp.append(baseName);
//            temp.append(fileToCheck);
//            picturesToAdd.append(temp);
//            somethingIsAdded = true;
//            qDebug() << "This is a *PICTURE*: " << fileToCheck;
//        }
//        if (somethingIsAdded)
//        {
//            QStringList temp;
//            temp.append(absPath);
//            temp.append(pathLevel);
//            treeFoldersToAdd.append(temp);
//        }
//    }
//    qDebug() << "Scanning complete, found : Scenes *" << videosToAdd.size()
//             << "* Pictures *" << picturesToAdd.size() << "* Folders *"
//             << treeFoldersToAdd.size() << "*";
//    this->dbManager->addScenes(videosToAdd);
//    this->dbManager->addPictures(picturesToAdd);
//    this->dbManager->addTreeFolders(treeFoldersToAdd);
//    qDebug() << "Finished walking path...";
//    qDebug() << "Starting probing unprobed scenes with FFPROBE ... ";
//    QList<QMap<QString, QVariant>> scenesBeforeFFprobe =
//                                    this->dbManager->getScenesBeforeFFprobe();
//    qDebug() << "Number of scenes to proble is: " << scenesBeforeFFprobe.size();
//    QList<QMap<QString, QVariant>> listOfSceneUpdates;
//    QElapsedTimer timer;
//    timer.start();
//    for (int i = 0; i < scenesBeforeFFprobe.size(); i++)
//    {
//        QMap<QString, QVariant> temp =
//            ffmpegHandler->ffprobeTest(scenesBeforeFFprobe.at(i));
//        if (!temp.contains("failed"))
//        {
//            listOfSceneUpdates.append(temp);
//        }
//    }
//    qDebug() << "Finished FFprobing scenes, probing all unprobed scenes took "
//             << timer.elapsed() << " ms.";
//    qDebug() << "Updating FFprobed scenes in database ...";
//    this->dbManager->updateScenes(listOfSceneUpdates);
//    //  delete scenesBeforeFFprobe;
//}
void FileImporter::run()
{
    qDebug() << "File importer thread started ...";

    //  this->mutex->lock();
    while (!this->mediaFoldersQueue.isEmpty())
    {
        //    this->mutex->unlock();
        QMap<QString, QVariant> currentMediaFolder =
            this->mediaFoldersQueue.dequeue();

        if (currentMediaFolder["path_to_dir"].toString() == "poison")
        {
            break;
        }

        emit startedScanningFolders();
        this->findItemsToAdd(currentMediaFolder);
        //            this->walkPath(currentMediaFolder);
    }

    qDebug() << "File importer thread exiting ...";
}
