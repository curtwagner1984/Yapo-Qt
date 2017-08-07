#ifndef FFMPEGHANDLER_H
#define FFMPEGHANDLER_H

#include <QObject>
#include <QThread>
#include <QSettings>

class FfmpegHandler : public QObject
{
        Q_OBJECT
    public:
        explicit FfmpegHandler(QSettings* settings, QObject* parent = 0);
        QMap<QString, QVariant> ffprobeTest(QMap<QString, QVariant> scene);
        QList<QMap<QString, QVariant>> ffprobeScenes(QList<QMap<QString, QVariant> >* scenesToProbeList);

        static int getSceneFfprobeInfo(QMap<QString, QVariant>& scene, QSettings* settings);
        static int getPictureFfprobeInfo(QMap<QString, QVariant>& picture, QSettings* settings);
        static int takeSceneScreenShot(QMap<QString, QVariant>& scene, QSettings* settings, int nextId);
        static int generateContactSheetForScene(QMap<QString, QVariant>& scene, QSettings* settings, int nextId);

    private slots:

    private:
        QSettings* settings;
        QString _basePath;
        QString _ffmpegPath;
        QString _ffprobePath;
        int _idealThreadCount;
        int _numberOfCurrentProcesses;


    signals:

    public slots:
};

#endif // FFMPEGHANDLER_H
