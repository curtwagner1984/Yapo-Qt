#include "tmdbscraper.h"
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QJsonDocument>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QScriptEngine>
#include <QUrl>
#include <QUrlQuery>
#include <QtNetwork>

//const QString BASE_PATH = "D:/YAPOC++/";

TmdbScraper::TmdbScraper(DbManager* dbManager,QSettings* setting, QObject* parent)
    : QObject(parent) {
  this->dbManager = dbManager;
  this->networkManager = new QNetworkAccessManager(this);
  this->setting = setting;
  this->_basePath = setting->value("general/homepath").toString();

  connect(this->networkManager, &QNetworkAccessManager::finished, this,
          &TmdbScraper::onActorSearchResult);

  connect(this, &TmdbScraper::searchActor, this, &TmdbScraper::onSearchActor);
  connect(this, &TmdbScraper::scrapeActors, this, &TmdbScraper::onScrapeActors);
}

void TmdbScraper::setActorsToScrape(
    QList<QMap<QString, QVariant>> actorsToScrape) {
  this->actorsToScrape = actorsToScrape;
}

void TmdbScraper::onSearchActor(int actorToSearchIndex) {
  //    actor search tmdb call
  //    "https://api.themoviedb.org/3/search/person?api_key=04308f6d1c14608e9f373b84ad0e4e4c&language=en-US&query={}&page=1&include_adult=true"
  QUrlQuery url("https://api.themoviedb.org/3/search/person?");
  QMap<QString, QVariant> params;
  params["api_key"] = "04308f6d1c14608e9f373b84ad0e4e4c";
  params["language"] = "en-US";
  params["query"] =
      this->actorsToScrape.at(actorToSearchIndex)["name"].toString();
  params["include_adult"] = "true";
  foreach (QString param, params.keys()) {
    url.addQueryItem(param, params[param].toString());
  }

  qDebug() << "TMDB SCRAPER: person search url  " << url.toString();

  QNetworkRequest request;
  QUrl myUrl(url.toString());
  request.setUrl(myUrl);

  this->numberOfNetworkCalls++;
  QNetworkReply* reply = this->networkManager->get(request);

  reply->setProperty("index", actorToSearchIndex);
  reply->setProperty("requestType", "actorSearch");
}

void TmdbScraper::onScrapeActors() {
  qDebug() << "Entering onScrapeActors Slot";

  if (this->actorsToScrapeIndex == 0) {
    qDebug() << "actorsToScrapeIndex == 0 is true, starting timer";
    this->timer.start();
  }

  bool didTenSecondPass = (10000 - timer.elapsed()) < 0;
  int timeRemaining = 10000 - timer.elapsed();

  if (!(this->actorsToScrapeIndex < this->actorsToScrape.size())) {
    qDebug() << "!(this->actorsToScrapeIndex < this->actorsToScrape.size()) is "
                "true, saving updated actors";
    this->saveCurrentlyUpdatedActorsToDb();
    return;
  }

  if (this->updatedActors.size() >= 5) {
    qDebug()
        << "this->updatedActors.size() > 30 is true, saving updated actors";
    this->saveCurrentlyUpdatedActorsToDb();
  }

  if (!didTenSecondPass) {
    qDebug() << "!didTenSecondPass true";

    if (this->numberOfNetworkCalls < 30) {
      qDebug() << "numberOfNetworkCalls < 30 is true, searching next actor";
      emit this->searchActor(this->actorsToScrapeIndex);
      this->actorsToScrapeIndex++;

    } else {
      qDebug() << "numberOfNetworkCalls < 30 is false, waiting "
               << timeRemaining + 1000 << " ms";
      QTimer::singleShot(timeRemaining + 1000, this,
                         &TmdbScraper::onScrapeActors);
    }
  } else {
    qDebug() << "!didTenSecondPass false, resetting timer and nubmer of "
                "network calls";
    this->timer.start();
    this->numberOfNetworkCalls = 0;

    emit this->searchActor(this->actorsToScrapeIndex);
    this->actorsToScrapeIndex++;
  }
}

void TmdbScraper::startScrape() {
  if (this->actorsToScrape.size() == 0) {
    return;
  }

  this->actorsToScrapeIndex = 0;
  emit this->scrapeActors();

  //  for (int i = 0; i < 5 /*this->actorsToScrape->size()*/; i++) {
  //    emit this->searchActor(i);
  //    qDebug() << "search actor loop iteration #" << i;
  //  }
}

TmdbScraper::~TmdbScraper() {
  this->actorsToScrape.clear();
  this->actorsAliases.clear();

  delete this->networkManager;
  //  delete this->actorsToScrape;
  //  delete this->actorsAliases;
}

int TmdbScraper::getActorTmdbIdFromJson(QString jsonData,
                                        QMap<QString, QVariant> actor) {
  qDebug() << " Actor name is: " << actor["name"].toString();

  QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
  QJsonObject jsonObj = doc.object();
  QJsonValue resultsValue = jsonObj.value(QString("results"));

  QJsonArray results = resultsValue.toArray();
  QJsonValue actorTmdbId;
  if (results.size() > 0) {
    for (int i = 0; i < results.size(); i++) {
      QJsonObject result = results[i].toObject();
      if (!actor["is_mainstream"].toBool()) {
        QJsonValue isAdult = result["adult"];
        if (isAdult.toBool()) {
          actorTmdbId = result["id"];
          break;
        }
      } else {
        actorTmdbId = result["id"];
        break;
      }
    }
  }

  qDebug() << "Actor TMDB id is: " << actorTmdbId.toInt()
           << " Actor name is: " << actor["name"].toString()
           << ((actor["is_mainstream"].toBool()) ? " Is Mainstream."
                                                 : " Is Adult.");

  return actorTmdbId.toInt();
}

void TmdbScraper::saveCurrentlyUpdatedActorsToDb() {
  QList<QMap<QString, QVariant>> finishedList;

  QMapIterator<int, QMap<QString, QVariant>> i(this->updatedActors);
  while (i.hasNext()) {
    i.next();
    QMap<QString, QVariant> temp = i.value();
    if (temp.contains("finished")) {
      temp.remove("finished");
      finishedList.append(temp);
      this->updatedActors.remove(i.key());
    }
  }
  this->dbManager->updateActors(finishedList);
  this->dbManager->addActorsAliases(this->actorsAliases);
  this->actorsAliases.clear();
  qDebug() << this->actorsAliases.size();
}

void TmdbScraper::onActorSearchResult(QNetworkReply* reply) {
  reply->ignoreSslErrors();
  //        qDebug() << reply->errorString();

  QString requestType = reply->property("requestType").toString();
  QMap<QString, QVariant> actor =
      this->actorsToScrape.at(reply->property("index").toInt());

  if (reply->error() != QNetworkReply::NoError) {
    qDebug() << "Network Error! " << reply->errorString();
    qDebug() << "Waiting 10 seconds before procedding ... ";
    QTimer::singleShot(10000, this, &TmdbScraper::onScrapeActors);
    return;
  }

  if (requestType == "actorSearch") {
    QString data = (QString)reply->readAll();

    int actorTmdbId = this->getActorTmdbIdFromJson(data, actor);

    if (actorTmdbId != 0) {
      QUrlQuery url("https://api.themoviedb.org/3/person/" +
                    QString::number(actorTmdbId) + "?");
      QMap<QString, QVariant> params;
      params["api_key"] = "04308f6d1c14608e9f373b84ad0e4e4c";
      params["language"] = "en-US";
      foreach (QString param, params.keys()) {
        url.addQueryItem(param, params[param].toString());
      }

      qDebug() << "TMDB SCRAPER: get person info url " << url.toString();

      QNetworkRequest request;
      QUrl myUrl(url.toString());
      request.setUrl(myUrl);

      this->numberOfNetworkCalls++;
      QNetworkReply* getInfoReply = this->networkManager->get(request);

      getInfoReply->setProperty("index", reply->property("index").toInt());
      getInfoReply->setProperty("requestType", "actorInfo");
    } else {
      emit this->scrapeActors();
    }

  } else if (requestType == "actorInfo") {
    QString data = (QString)reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject jsonObj = doc.object();

    QMap<QString, QVariant> updatedActor;
    updatedActor["id"] = actor["id"].toInt();

    QJsonValue tempValue;

    if (!(jsonObj["birthday"].toString() == "")) {
      QDateTime temp;
      //                    "1980-08-27"
      qDebug() << "Birthday in JSON is " << jsonObj["birthday"].toString();
      temp =
          QDateTime::fromString(jsonObj["birthday"].toString(), "yyyy-MM-dd");
      QString tempString = temp.toString("yyyy-MM-dd");
      if (tempString != ""){
        updatedActor["date_of_birth"] = tempString;
      }

    }

    tempValue = jsonObj["biography"];
    if (!(tempValue.toString() == "")) {
      updatedActor["description"] = tempValue.toString().trimmed();
    }

    tempValue = jsonObj["gender"];
    if (!tempValue.isNull()) {
      if (tempValue.toInt() != 0) {
        updatedActor["gender"] = tempValue.toInt();
      }
    }

    tempValue = jsonObj["homepage"];
    if (!(tempValue.toString() == "")) {
      updatedActor["official_pages"] = tempValue.toString().trimmed();
    }

    tempValue = jsonObj["id"];
    if (!tempValue.isNull()) {
      updatedActor["tmdb_id"] = tempValue.toInt();
    }

    tempValue = jsonObj["imdb_id"];
    if (!(tempValue.toString() == "")) {
      updatedActor["imdb_id"] = tempValue.toString();
    }

    tempValue = jsonObj["place_of_birth"];
    if (!(tempValue.toString() == "")) {
      updatedActor["country_of_origin"] = tempValue.toString().trimmed();
    }

    QJsonArray aliases = jsonObj["also_known_as"].toArray();

    for (int i = 0; i < aliases.size(); i++) {
      QStringList list;
      QJsonValue temp = aliases.at(i);
      list.append(temp.toString().trimmed());
      list.append(actor["id"].toString());
      this->actorsAliases.append(list);
    }
    updatedActor["date_last_lookup"] = "DATETIME('now','localtime')";
    this->updatedActors.insert(updatedActor["tmdb_id"].toInt(), updatedActor);

    tempValue = jsonObj["profile_path"];

    if (!tempValue.isNull()) {
      qDebug() << actor["name"].toString() << "profile picture is: "
               << this->tmdbImageBasePath + tempValue.toString();

      QUrl myUrl(this->tmdbImageBasePath + tempValue.toString());
      QNetworkRequest request;
      request.setUrl(myUrl);

      this->numberOfNetworkCalls++;
      QNetworkReply* getProfileImageReply = this->networkManager->get(request);

      getProfileImageReply->setProperty("index",
                                        reply->property("index").toInt());
      getProfileImageReply->setProperty("tmdbId",
                                        updatedActor["tmdb_id"].toInt());
      getProfileImageReply->setProperty("requestType", "getProfileImage");
    } else {
      updatedActor["finished"] = true;
      this->updatedActors.insert(updatedActor["tmdb_id"].toInt(), updatedActor);
      emit this->scrapeActors();
    }

  } else if (requestType == "getProfileImage") {
    //        qDebug() << reply->error();
    //        qDebug() << reply->errorString();

    QString basePath =
        _basePath + "/actor/" + actor["id"].toString() + "/" + "profile";
    QString outputFilename = basePath + "/profile.jpg";

    int tmdbId = reply->property("tmdbId").toInt();
    QMap<QString, QVariant> updatedActorTemp =
        this->updatedActors.value(tmdbId);

    QDir qdir;
    if (qdir.mkpath(basePath) && !QFile::exists(outputFilename)) {
      QFile* file = new QFile(outputFilename);
      file->open(QIODevice::WriteOnly);
      file->write(reply->readAll());
      file->close();
      delete file;

      QImage img = QImage(outputFilename);
      QImage imgWidth64 = img.scaledToWidth(64);
      QImage imgWidth256 = img.scaledToWidth(320);

      imgWidth64.save(basePath + "/profile_64.jpg", 0, 70);
      imgWidth256.save(basePath + "/profile_320.jpg", 0, 70);

      updatedActorTemp["thumbnail"] = outputFilename;
    }

    updatedActorTemp["finished"] = true;
    this->updatedActors.insert(tmdbId, updatedActorTemp);
    emit this->scrapeActors();
  }
  reply->deleteLater();
}
