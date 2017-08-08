#include "ffmpeghandler.h"
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QScriptEngine>
#include <QElapsedTimer>

//const QString _ffprobePath = "D:/Webstorm/Yapo-Electron/app/bin/ffmpeg/ffprobe";
//const QString _ffmpegPath = "D:/Webstorm/Yapo-Electron/app/bin/ffmpeg/ffmpeg";

//const QString _basePath = "D:/YAPOC++/";

//static const int idealThreadCount = QThread::idealThreadCount();
//static int numberOfCurrentProcesses = 0;

FfmpegHandler::FfmpegHandler(QSettings* settings, QObject* parent) : QObject(parent)
{
    this->settings = settings;
    this->_basePath = settings->value("general/homepath").toString();
    this->_ffmpegPath = settings->value("general/homepath").toString() + "/bin/ffmpeg";
    this->_ffprobePath = settings->value("general/homepath").toString() + "/bin/ffprobe";
    this->_idealThreadCount = QThread::idealThreadCount();
    this->_numberOfCurrentProcesses = 0;
}

QMap<QString, QVariant> FfmpegHandler::ffprobeTest(
    QMap<QString, QVariant> scene)
{
    //    connect(sender, &QObject::destroyed, [=](){
    //    this->m_objects.remove(sender); });
    QString filename = scene["path_to_file"].toString();
    QMap<QString, QVariant> updatedScene;
    QMap<QString, QVariant> ffmpegFailed;
    int nthFrame = -1;
    int NUMBER_OF_PREVIEW_FRAMES = 30;
    ffmpegFailed["failed"] = "True";
    updatedScene["id"] = scene["id"];
    int currentProbeScore = -1;
    QStringList arguments;
    arguments << "-v"
              << "0"
              << "-print_format"
              << "json"
              << "-show_format"
              << "-show_streams" << filename;
    QString command = _ffprobePath + " -v 0 " + "-print_format json " +
                      "-show_format " + "-show_streams" + '"' + filename + '"';
    qDebug() << "Command: " << command;
    QProcess* myProcess = new QProcess();
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(_ffprobePath, arguments);

    if (!myProcess->waitForFinished())
    {
        qDebug() << "failed:" << myProcess->errorString();
        return ffmpegFailed;
    }
    else
    {
        //        qDebug() << "output:" << myProcess->readAll();
        QString jsonData = myProcess->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        QJsonObject jsonObj = doc.object();
        QJsonValue value = jsonObj.value(QString("streams"));
        QJsonArray streams = value.toArray();
        QJsonObject streamZero;
        QScriptEngine expression;
        //        double my_val = expression.evaluate(expression_string).toNumber();

        for (int i = 0; i < streams.size(); i++)
        {
            QJsonObject stream = streams[i].toObject();
            QJsonValue codecType = stream["codec_type"];

            if (codecType.toString() == "video")
            {
                streamZero = stream;
                break;
            }
        }

        QJsonValue codecName = streamZero["codec_name"];
        QJsonValue width = streamZero["width"];
        QJsonValue height = streamZero["height"];
        QJsonValue frameRate = streamZero["r_frame_rate"];
        QJsonValue nb_frames = streamZero["nb_frames"];
        qDebug() << "Number of frames is " << nb_frames.toString();

        if (nb_frames.toString() != "N/A")
        {
            int nb_frames_int = nb_frames.toString().toInt();
            nthFrame = nb_frames_int / 120;
        }

        QJsonValue value2 = jsonObj.value(QString("format"));
        QJsonObject format = value2.toObject();
        QJsonValue duration = format["duration"];
        QJsonValue size = format["size"];
        QJsonValue bit_rate = format["bit_rate"];
        QJsonValue probeScore = format["probe_score"];
        qDebug() << "codec_name" << codecName.toString();
        updatedScene["codec_name"] = codecName.toString();
        qDebug() << "duration"
                 << expression.evaluate(duration.toString()).toInteger();
        updatedScene["duration"] =
            expression.evaluate(duration.toString()).toInteger();
        qDebug() << "width" << width.toInt();
        updatedScene["width"] = width.toInt();
        qDebug() << "height" << height.toInt();
        updatedScene["height"] = height.toInt();
        qDebug() << "frameRate"
                 << expression.evaluate(frameRate.toString()).toNumber();
        updatedScene["framerate"] =
            expression.evaluate(frameRate.toString()).toNumber();
        qDebug() << "size to variant to int" << size.toVariant().toLongLong();
        updatedScene["size"] = size.toVariant().toLongLong();
        qDebug() << "bit_rate to variant to int" << bit_rate.toVariant().toInt();
        updatedScene["bit_rate"] = bit_rate.toVariant().toInt();
        updatedScene["date_last_lookup"] = "DATETIME('now','localtime')";
        qDebug() << "probe_score" << probeScore.toInt();
        currentProbeScore = probeScore.toInt();
    }

    if (currentProbeScore > 25)
    {
        QString basePath =
            _basePath + "/scene/" + updatedScene["id"].toString() + "/" + "screenshots";
        QString contactSheetPath = _basePath + "/scene/" + updatedScene["id"].toString() + "/" + "contactSheet/";
        QString outputFilename = basePath + "/screenshot.jpg";
        int sceneLength = updatedScene["duration"].toInt();
        QString screenShotTime =
            QDateTime::fromTime_t(sceneLength * 0.1).toUTC().toString("hh:mm:ss");
        QDir qdir;

        if (qdir.mkpath(basePath) && !QFile::exists(outputFilename))
        {
            arguments.clear();
            arguments
                    << "-ss" << screenShotTime << "-i" << scene["path_to_file"].toString()
                    << "-vframes"
                    << "1"
                    << "-vf"
                    << "scale=854:480:force_original_aspect_ratio=increase,crop=854:480"
                    << "-q:v"
                    << "5" << outputFilename;
            QProcess* myProcess = new QProcess();
            myProcess->setProcessChannelMode(QProcess::MergedChannels);
            myProcess->start(_ffmpegPath, arguments);

            if (!myProcess->waitForFinished())
            {
                qDebug() << "failed:" << myProcess->errorString();
            }
            else
            {
                QString contactSheetOutputFilename;
                QString contactSheetShotTime;

                for (int i = 0 ; i < NUMBER_OF_PREVIEW_FRAMES && sceneLength > 300 ; i ++)
                {
                    contactSheetShotTime = QDateTime::fromTime_t((sceneLength / NUMBER_OF_PREVIEW_FRAMES) * i).toUTC().toString("hh:mm:ss");
                    QString contactSheetShotTimeString = contactSheetShotTime;
                    contactSheetShotTimeString.replace(':', '-');
                    contactSheetOutputFilename = contactSheetPath +  QString("cs_T_%1_O_%2.jpg").arg(contactSheetShotTimeString, QString::number(i));

                    if (qdir.mkpath(contactSheetPath) && !QFile::exists(contactSheetOutputFilename))
                    {
                        arguments.clear();
                        arguments
                                << "-ss" << contactSheetShotTime << "-i" << scene["path_to_file"].toString()
                                << "-vframes"
                                << "1"
                                << "-vf"
                                << "scale=384:216:force_original_aspect_ratio=increase,crop=384:216"
                                << "-q:v"
                                << "8" << contactSheetOutputFilename;
                        myProcess->setProcessChannelMode(QProcess::MergedChannels);
                        myProcess->start(_ffmpegPath, arguments);

                        if (!myProcess->waitForFinished())
                        {
                            qDebug() << "failed:" << myProcess->errorString();
                        }

                        qDebug() << myProcess->exitCode();
                        //                        qDebug() << myProcess->readAll();
                        //                        qDebug() << myProcess->errorString();
                    }
                    else
                    {
                        break;
                    }
                }

                updatedScene["thumbnail"] = outputFilename;
            }
        }
        else
        {
            updatedScene["thumbnail"] = outputFilename;
        }

        return updatedScene;
    }
    else
    {
        return ffmpegFailed;
    }
}

QList<QMap<QString, QVariant>> FfmpegHandler::ffprobeScenes(
                                QList<QMap<QString, QVariant>>* scenesToProbeList)
{
    QList<QMap<QString, QVariant>> listOfSceneUpdates;
    int i = 0;

    while (i < scenesToProbeList->size())
    {
        QMap<QString, QVariant> temp =
            FfmpegHandler::ffprobeTest(scenesToProbeList->at(i));
        listOfSceneUpdates.append(temp);
    }

    return listOfSceneUpdates;
}

int FfmpegHandler::getSceneFfprobeInfo(QMap<QString, QVariant>& scene, QSettings* settings)
{
    QElapsedTimer timer;
    timer.start();
    qDebug() << "Starting to FFprobe Scene " << scene["path_to_file"].toString();
    QString basePath = settings->value("general/homepath").toString();
    QString ffprobePath = settings->value("ffmpeg/ffprobepath").toString();
    QString filename = scene["path_to_file"].toString();
    int ffprobeScore = -1;
    QStringList arguments;
    arguments << "-v"
              << "0"
              << "-print_format"
              << "json"
              << "-show_format"
              << "-show_streams" << filename;
    QString command = ffprobePath + " -v 0 " + "-print_format json " +
                      "-show_format " + "-show_streams" + '"' + filename + '"';
    //    qDebug() << "Running ffmpeg command: " << command;
    QProcess* myProcess = new QProcess();
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(ffprobePath, arguments);

    if (!myProcess->waitForFinished())
    {
        qDebug() << "FFPROBE failed:" << myProcess->errorString();
        //        return ffmpegFailed;
    }
    else
    {
        //        qDebug() << "output:" << myProcess->readAll();
        QString jsonData = myProcess->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        QJsonObject jsonObj = doc.object();
        QJsonValue value = jsonObj.value(QString("streams"));
        QJsonArray streams = value.toArray();
        QJsonObject videoStream;
        QScriptEngine expression;
        //        double my_val = expression.evaluate(expression_string).toNumber();

        for (int i = 0; i < streams.size(); i++)
        {
            QJsonObject stream = streams[i].toObject();
            QJsonValue codecType = stream["codec_type"];

            if (codecType.toString() == "video")
            {
                videoStream = stream;
                break;
            }
        }

        QJsonValue codecName = videoStream["codec_name"];
        QJsonValue width = videoStream["width"];
        QJsonValue height = videoStream["height"];
        QJsonValue frameRate = videoStream["r_frame_rate"];
        //        QJsonValue nb_frames = videoStream["nb_frames"];
        //        qDebug() << "Number of frames is " << nb_frames.toString();
        //        if (nb_frames.toString() != "N/A")
        //        {
        //            int nb_frames_int = nb_frames.toString().toInt();
        //            nthFrame = nb_frames_int / 120;
        //        }
        QJsonValue value2 = jsonObj.value(QString("format"));
        QJsonObject format = value2.toObject();
        QJsonValue duration = format["duration"];
        QJsonValue size = format["size"];
        QJsonValue bit_rate = format["bit_rate"];
        QJsonValue probeScore = format["probe_score"];
        //        qDebug() << "codec_name" << codecName.toString();
        scene["codec_name"] = codecName.toString();
        //        qDebug() << "duration"
        //                 << expression.evaluate(duration.toString()).toInteger();
        scene["duration"] =
            expression.evaluate(duration.toString()).toInteger();
        //        qDebug() << "width" << width.toInt();
        scene["width"] = width.toInt();
        //        qDebug() << "height" << height.toInt();
        scene["height"] = height.toInt();
        //        qDebug() << "frameRate"
        //                 << expression.evaluate(frameRate.toString()).toNumber();
        scene["framerate"] =
            expression.evaluate(frameRate.toString()).toNumber();
        //        qDebug() << "size to variant to int" << size.toVariant().toLongLong();
        scene["size"] = size.toVariant().toLongLong();
        //        qDebug() << "bit_rate to variant to int" << bit_rate.toVariant().toInt();
        scene["bit_rate"] = bit_rate.toVariant().toInt();
        //        scene["date_last_lookup"] = "DATETIME('now','localtime')";
        //        qDebug() << "probe_score" << probeScore.toInt();
        ffprobeScore = probeScore.toInt();
        QString elapsedTime = QDateTime::fromTime_t(timer.elapsed() / 1000).toUTC().toString("hh:mm:ss");
        qDebug() << "Finished FFprobing scene " << scene["path_to_file"].toString() << " process took *" << elapsedTime ;
    }

    delete myProcess;
    return ffprobeScore;
}

int FfmpegHandler::getPictureFfprobeInfo(QMap<QString, QVariant>& picture, QSettings* settings)
{
    QElapsedTimer timer;
    timer.start();
    qDebug() << "Starting to FFprobe Scene " << picture["path_to_file"].toString();
    QString basePath = settings->value("general/homepath").toString();
    QString ffprobePath = settings->value("ffmpeg/ffprobepath").toString();
    QString filename = picture["path_to_file"].toString();
    int ffprobeScore = -1;
    QStringList arguments;
    arguments << "-v"
              << "0"
              << "-print_format"
              << "json"
              << "-show_format"
              << "-show_streams" << filename;
    QString command = ffprobePath + " -v 0 " + "-print_format json " +
                      "-show_format " + "-show_streams" + '"' + filename + '"';
    //    qDebug() << "Running ffmpeg command: " << command;
    QProcess* myProcess = new QProcess();
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(ffprobePath, arguments);

    if (!myProcess->waitForFinished())
    {
        qDebug() << "FFPROBE failed:" << myProcess->errorString();
        //        return ffmpegFailed;
    }

    if (myProcess->exitCode() == 1)
    {
        qDebug() << "FFPROBE failed:" << myProcess->errorString();
    }
    else
    {
        //        qDebug() << "output:" << myProcess->readAll();
        QString jsonData = myProcess->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        QJsonObject jsonObj = doc.object();
        QJsonValue value = jsonObj.value(QString("streams"));
        QJsonArray streams = value.toArray();
        QJsonObject videoStream;
        //        double my_val = expression.evaluate(expression_string).toNumber();

        for (int i = 0; i < streams.size(); i++)
        {
            QJsonObject stream = streams[i].toObject();
            QJsonValue codecType = stream["codec_type"];

            if (codecType.toString() == "video")
            {
                videoStream = stream;
                break;
            }
        }

        QJsonValue width = videoStream["width"];
        QJsonValue height = videoStream["height"];
        QJsonValue value2 = jsonObj.value(QString("format"));
        QJsonObject format = value2.toObject();
        QJsonValue size = format["size"];
        QJsonValue probeScore = format["probe_score"];
        picture["width"] = width.toInt();
        //        qDebug() << "height" << height.toInt();
        picture["height"] = height.toInt();
        picture["megapixel"] = width.toDouble() * height.toDouble() / 1000000;
        //        qDebug() << "frameRate"
        //                 << expression.evaluate(frameRate.toString()).toNumber();
        picture["size"] = size.toVariant().toLongLong();
        //        qDebug() << "bit_rate to variant to int" << bit_rate.toVariant().toInt();
        //        scene["date_last_lookup"] = "DATETIME('now','localtime')";
        //        qDebug() << "probe_score" << probeScore.toInt();
        ffprobeScore = probeScore.toInt();
        QString elapsedTime = QDateTime::fromTime_t(timer.elapsed() / 1000).toUTC().toString("hh:mm:ss");
        qDebug() << "Finished FFprobing Picture " << picture["path_to_file"].toString() << " process took *" << elapsedTime ;
    }

    delete myProcess;
    return ffprobeScore;
}

int FfmpegHandler::takeSceneScreenShot(QMap<QString, QVariant>& scene, QSettings* settings, int nextId)
{
    QElapsedTimer timer;
    int didFail = 0;
    timer.start();
    qDebug() << "Generating Scene Thumbnail for Scene " << scene["path_to_file"].toString() << " ....";
    QString homePath = settings->value("general/homepath").toString();
    QString ffmpegPath = settings->value("ffmpeg/ffmpegpath").toString();
    QString screenShotPath =
        homePath + "/scene/" + QString::number(nextId) + "/" + "screenshots";
    QString outputFilename = screenShotPath + "/screenshot.jpg";
    int sceneLength = scene["duration"].toInt();
    QString screenShotTime =
        QDateTime::fromTime_t(sceneLength * 0.1).toUTC().toString("hh:mm:ss");
    QDir qdir;

    if (qdir.mkpath(screenShotPath))
    {
        QStringList arguments;
        arguments
                << "-ss" << screenShotTime << "-i" << scene["path_to_file"].toString()
                << "-vframes"
                << "1"
                << "-vf"
                << "scale=854:480:force_original_aspect_ratio=increase,crop=854:480"
                << "-q:v"
                << "5" << outputFilename;
        QProcess* myProcess = new QProcess();
        myProcess->setProcessChannelMode(QProcess::MergedChannels);
        myProcess->start(ffmpegPath, arguments);

        if (!myProcess->waitForFinished())
        {
            qDebug() << "failed:" << myProcess->errorString();
            didFail = 1;
        }

        if (myProcess->exitCode() == 1)
        {
            qDebug() << "failed:" << myProcess->errorString();
            didFail = 1;
        }
        else
        {
            qDebug() << "Made Screenshot";
            scene["thumbnail"] = outputFilename;
            QString elapsedTime = QDateTime::fromTime_t(timer.elapsed() / 1000).toUTC().toString("hh:mm:ss");
            qDebug() << "Finished generating scene Thumbnail " << scene["path_to_file"].toString() << " process took *" << elapsedTime ;
        }
    }

    return didFail;
}

int FfmpegHandler::generateContactSheetForScene(QMap<QString, QVariant>& scene, QSettings* settings, int nextId)
{
    QElapsedTimer timer;
    int didFail = 0;
    timer.start();
    QString homePath = settings->value("general/homepath").toString();
    QString ffmpegPath = settings->value("ffmpeg/ffmpegpath").toString();
    QString contactSheetPath = homePath + "/scene/" + QString::number(nextId) + "/" + "contactSheet/";
    QString contactSheetOutputFilename;
    QString contactSheetShotTime;
    QDir qdir;
    // for now a shot will be made for every 20 seconds of video this will be later configureable in settings...
    int screenShortPerThisNumberOfSeconds = 20;
    int sceneDuration = scene["duration"].toInt();
    QString pathToFile = scene["path_to_file"].toString();
    int timeToAddPerTick = sceneDuration / screenShortPerThisNumberOfSeconds;
    int screenShotTimeInSeconds = screenShortPerThisNumberOfSeconds;
    int order = 0;
    QProcess* myProcess = new QProcess();
    bool failed = false;

    while (screenShotTimeInSeconds <= sceneDuration && !failed)
    {
        contactSheetShotTime = QDateTime::fromTime_t(screenShotTimeInSeconds).toUTC().toString("hh:mm:ss");
        QString contactSheetShotTimeInFilename = contactSheetShotTime;
        contactSheetShotTimeInFilename.replace(':', '-');
        contactSheetOutputFilename = contactSheetPath +  QString("cs_T_%1_O_%2.jpg").arg(contactSheetShotTimeInFilename, QString::number(order));

        if (qdir.mkpath(contactSheetPath))
        {
            QStringList arguments;
            arguments
                    << "-ss" << contactSheetShotTime << "-i" << pathToFile
                    << "-vframes"
                    << "1"
                    << "-vf"
                    << "scale=384:216:force_original_aspect_ratio=increase,crop=384:216"
                    << "-q:v"
                    << "5" << contactSheetOutputFilename;
            myProcess->setProcessChannelMode(QProcess::MergedChannels);
            myProcess->start(ffmpegPath, arguments);

            if (!myProcess->waitForFinished())
            {
                qDebug() << "failed:" << myProcess->errorString();
                failed = true;
            }

            if (myProcess->exitCode() == 1)
            {
                qDebug() << "failed:" << myProcess->errorString();
                failed = true;
            }
            else
            {
                qDebug() << "Generated contact list image for scene " << pathToFile << " at time " << contactSheetShotTime;
            }
        }

        screenShotTimeInSeconds = screenShotTimeInSeconds + screenShortPerThisNumberOfSeconds;
        order++;
    }

    QString elapsedTime = QDateTime::fromTime_t(timer.elapsed() / 1000).toUTC().toString("hh:mm:ss");

    if (!failed)
    {
        qDebug() << "Finished generating " << order + 1 << " contact sheet images for scene" <<  scene["path_to_file"].toString() << " process took *" << elapsedTime ;
    }
    else
    {
        didFail = 1;
        qDebug() << "Something went wrong while generating contact sheet for " <<  scene["path_to_file"].toString() << " Skipping ...";
    }

    return didFail;
}


