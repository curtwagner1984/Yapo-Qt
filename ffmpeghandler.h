#ifndef FFMPEGHANDLER_H
#define FFMPEGHANDLER_H

#include <QObject>
#include <QThread>

class FfmpegHandler : public QObject
{
    Q_OBJECT
public:
    explicit FfmpegHandler(QObject *parent = 0);
    static QMap<QString, QVariant> ffprobeTest(QMap<QString, QVariant> scene);
    static QList<QMap<QString, QVariant>> ffprobeScenes (QList<QMap<QString, QVariant> >* scenesToProbeList);

private slots:

private:



signals:

public slots:
};

#endif // FFMPEGHANDLER_H
