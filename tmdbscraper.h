#ifndef TMDBSCRAPER_H
#define TMDBSCRAPER_H

#include <QObject>
#include <QNetworkReply>
#include "dbmanager.h"
#include <QElapsedTimer>
#include <QSettings>

class TmdbScraper : public QObject
{
    Q_OBJECT
public:
    explicit TmdbScraper(DbManager* dbManager,QSettings* setting, QObject *parent = 0);
    void setActorsToScrape (QList<QMap<QString, QVariant>> actorsToScrape);

    void startScrape();
    ~TmdbScraper();
private:
    QList<QMap<QString, QVariant>> actorsToScrape;
    QList<QStringList> actorsAliases;
    QMap<int,QMap<QString, QVariant>> updatedActors;
    DbManager* dbManager;
    QSettings* setting;
    QString _basePath;
    int getActorTmdbIdFromJson(QString jsonData, QMap<QString, QVariant> actor );
    QString tmdbImageBasePath = "http://image.tmdb.org/t/p/original";
    QNetworkAccessManager *networkManager;

    void saveCurrentlyUpdatedActorsToDb ();

    int actorsToScrapeIndex = -1;
    int numberOfNetworkCalls = 0;
    QElapsedTimer timer;


signals:
    void searchActor(int actorToSearchIndex);
    void scrapeActors();
public slots:
    void onSearchActor(int actorToSearchIndex);
    void onScrapeActors();

private slots:
    void onActorSearchResult(QNetworkReply* reply);
};

#endif // TMDBSCRAPER_H
