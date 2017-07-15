#ifndef FILEIMPORTER_H
#define FILEIMPORTER_H

#include <QThread>
#include <QSemaphore>
#include <QQueue>
#include <QString>
#include "dbmanager.h"
#include <QMutex>
#include <QSettings>
#include "ffmpeghandler.h"

#include <QMap>

class FileImporter  : public QThread
{
    Q_OBJECT
public:
    FileImporter(DbManager* dbManager, QMutex* mutex, QSettings* settings);
    void addMediaFolderToQueue(QString path, bool isVideo, bool isPicture);
    void run() override;

private:
    void walkPath(QMap<QString, QVariant> mediaFolderToWalk);
    QQueue <QMap<QString, QVariant>> mediaFoldersQueue;
    DbManager* dbManager;
    QSemaphore* semaphore;
    QSettings* settings;
    FfmpegHandler* ffmpegHandler;
    QStringList pictureExtentions = QStringList() << "jpg" << "jpeg" << "bmp" << "gif" << "bmp" << "png" ;
    QStringList videoExtentions = QStringList() << "mp4" << "mkv" << "avi" << "xvid" << "flv" << "mpg" << "webm" << "webm" << "wmv" << "rm" << "m4v"<< "mov"  ;

    QMutex* mutex;
};

#endif // FILEIMPORTER_H
