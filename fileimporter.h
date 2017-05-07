#ifndef FILEIMPORTER_H
#define FILEIMPORTER_H

#include <QThread>
#include <QSemaphore>
#include <QQueue>
#include <QString>
#include "dbmanager.h"
#include "db_models.h"
#include <QMutex>

class FileImporter  : public QThread
{
    Q_OBJECT
public:
    FileImporter(DbManager* dbManager, QMutex* mutex);
    void addMediaFolderToQueue(QString path, bool isVideo, bool isPicture);
    void run() override;

private:
    void walkPath(MediaFolder* mediaFolderToWalk);
    QQueue <MediaFolder*> mediaFoldersQueue;
    DbManager* dbManager;
    QSemaphore* semaphore;
    QStringList pictureExtentions = QStringList() << "jpg" << "jpeg" << "bmp" << "gif" << "bmp" << "png" ;
    QStringList videoExtentions = QStringList() << "mp4" << "mkv" << "avi" << "xvid" << "flv" << "mpg" << "webm" << "webm" << "wmv" << "rm" << "m4v"<< "mov"  ;

    QMutex* mutex;
};

#endif // FILEIMPORTER_H
