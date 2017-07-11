#include "ffmpeghandler.h"
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QScriptEngine>

//const QString _ffprobePath = "D:/Webstorm/Yapo-Electron/app/bin/ffmpeg/ffprobe";
//const QString _ffmpegPath = "D:/Webstorm/Yapo-Electron/app/bin/ffmpeg/ffmpeg";

//const QString _basePath = "D:/YAPOC++/";

//static const int idealThreadCount = QThread::idealThreadCount();
//static int numberOfCurrentProcesses = 0;

FfmpegHandler::FfmpegHandler(QSettings* settings, QObject *parent) : QObject(parent) {
    this->settings = settings;

    this->_basePath = settings->value("general/homepath").toString();
    this->_ffmpegPath = settings->value("general/homepath").toString() + "/bin/ffmpeg";
    this->_ffprobePath = settings->value("general/homepath").toString() + "/bin/ffprobe";

    this->_idealThreadCount = QThread::idealThreadCount();
    this->_numberOfCurrentProcesses = 0;

}

QMap<QString, QVariant> FfmpegHandler::ffprobeTest(
    QMap<QString, QVariant> scene) {
  //    connect(sender, &QObject::destroyed, [=](){
  //    this->m_objects.remove(sender); });

  QString filename = scene["path_to_file"].toString();
  QMap<QString, QVariant> updatedScene;
  QMap<QString, QVariant> ffmpegFailed;

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

  QProcess *myProcess = new QProcess();
  myProcess->setProcessChannelMode(QProcess::MergedChannels);
  myProcess->start(_ffprobePath, arguments);

  if (!myProcess->waitForFinished()){
    qDebug() << "failed:" << myProcess->errorString();
    return ffmpegFailed;

  }else {
    //        qDebug() << "output:" << myProcess->readAll();
    QString jsonData = myProcess->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject jsonObj = doc.object();
    QJsonValue value = jsonObj.value(QString("streams"));

    QJsonArray streams = value.toArray();

    QJsonObject streamZero;

    QScriptEngine expression;
    //        double my_val = expression.evaluate(expression_string).toNumber();

    for (int i = 0; i < streams.size(); i++) {
      QJsonObject stream = streams[i].toObject();
      QJsonValue codecType = stream["codec_type"];
      if (codecType.toString() == "video") {
        streamZero = stream;
        break;
      }
    }

    QJsonValue codecName = streamZero["codec_name"];
    QJsonValue width = streamZero["width"];
    QJsonValue height = streamZero["height"];
    QJsonValue frameRate = streamZero["r_frame_rate"];

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

  if (currentProbeScore > 25) {
    QString basePath =
        _basePath + "/scene/" + updatedScene["id"].toString() + "/" + "screenshots";
    QString outputFilename = basePath + "/screenshot.jpg";
    int sceneLength = updatedScene["duration"].toInt();
    QString screenShotTime =
        QDateTime::fromTime_t(sceneLength * 0.1).toUTC().toString("hh:mm:ss");

    QDir qdir;
    if (qdir.mkpath(basePath) && !QFile::exists(outputFilename)) {
      arguments.clear();
      arguments
          << "-ss" << screenShotTime << "-i" << scene["path_to_file"].toString()
          << "-vframes"
          << "1"
          << "-vf"
          << "scale=854:480:force_original_aspect_ratio=increase,crop=854:480"
          << "-q:v"
          << "5" << outputFilename;

      QProcess *myProcess = new QProcess();
      myProcess->setProcessChannelMode(QProcess::MergedChannels);
      myProcess->start(_ffmpegPath, arguments);

      if (!myProcess->waitForFinished())
        qDebug() << "failed:" << myProcess->errorString();

      else {
        updatedScene["thumbnail"] = outputFilename;
      }

    }else{
        updatedScene["thumbnail"] = outputFilename;
    }

    return updatedScene;

  }else{
      return ffmpegFailed;
  }
}

QList<QMap<QString, QVariant>> FfmpegHandler::ffprobeScenes(
    QList<QMap<QString, QVariant>> *scenesToProbeList) {
  QList<QMap<QString, QVariant>> listOfSceneUpdates;
  int i = 0;

  while (i < scenesToProbeList->size() &&
         _numberOfCurrentProcesses <= _idealThreadCount) {
    QMap<QString, QVariant> temp =
        FfmpegHandler::ffprobeTest(scenesToProbeList->at(i));
    listOfSceneUpdates.append(temp);
  }

  return listOfSceneUpdates;
}
