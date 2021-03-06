#ifndef QMLCOMM_H
#define QMLCOMM_H

#include <QObject>
#include <QDateTime>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QMutex>
#include <QSettings>


#include "dbmanager.h"
#include "fileimporter.h"
#include "tmdbscraper.h"
#include "tagger.h"

#include "mediafoldermodel.h"
#include "actormodel.h"
#include "scenemodel.h"
#include "picturemodel.h"
#include "tagmodel.h"
#include "websitemodel.h"

#include "scenepreviewmodel.h"



#include "generalalias.h"
#include "autocompletemodel.h"

#include "detailobject.h"


class QmlComm : public QObject
{
        Q_OBJECT
    public:
        explicit QmlComm(QQmlApplicationEngine& view, DbManager* dbManager, QObject* parent = 0);
        Q_INVOKABLE QDateTime getCurrentDateTime() const
        {
            return QDateTime::currentDateTime();
        }

        Q_INVOKABLE bool addMediaFolder(QString folderPath, bool isPicture, bool isVideo);
        Q_INVOKABLE bool addActors(QString actorList, bool isMainstream);
        Q_INVOKABLE bool addTags(QString tagList);
        Q_INVOKABLE bool addWebsites(QString websiteList);


        Q_INVOKABLE void scanMediaFolder(QString path, bool isVideo, bool isPicture);

        Q_INVOKABLE void ffmpegTest();
        Q_INVOKABLE void scrapeActors();
        Q_INVOKABLE void tagAll();


        Q_INVOKABLE void actorSearch(QString searchTerm);
        Q_INVOKABLE void sceneSearch(QString searchTerm);
        Q_INVOKABLE void pictureSearch(QString searchTerm);
        Q_INVOKABLE void tagSearch(QString searchTerm);
        Q_INVOKABLE void websiteSearch(QString searchTerm);
        Q_INVOKABLE void autoCompleteSearch(QString searchTerm, QString searchType);

        Q_INVOKABLE void aliasSearch(QString parentId, QString aliasType);


        Q_INVOKABLE void getActorScenes(QString actorId);
        Q_INVOKABLE void getActorPictures(QString actorId);
        Q_INVOKABLE void getTagsOfItem(QString itemId, QString tagType);

        Q_INVOKABLE void playScene(QString scenePath);


        //    Q_INVOKABLE void prepareDetailView (QString detailObjectId, QString detailObjectType);

        Q_INVOKABLE void addAlias(QString aliasName, QString aliasType, QString aliasOfId);
        Q_INVOKABLE void addTag(QString tagId, QString tagName, QString tagType, QString tagOfId);

        Q_INVOKABLE bool deleteAlias(QString aliasToDeleteId, QString aliasOf);

        Q_INVOKABLE void setSetting(QString key, QVariant value);
        Q_INVOKABLE QVariant getSetting(QString key);

        Q_INVOKABLE void init();



        ~QmlComm();

    private:
        DbManager* dbManager;
        FileImporter* fileImporter;
        QMutex* fileImporterMutex;
        TmdbScraper* scraper;
        Tagger* tagger;

        //    models
        MediafolderModel* mediaFolderModel;
        ActorModel* actorModel;
        SceneModel* sceneModel;
        PictureModel* pictureModel;
        TagModel* tagModel;
        WebsiteModel* websiteModel;
        ScenePreviewModel* scenePreview;



        //    DetailObject* detailObject;

        GeneralAlias* generalAlias;
        AutoCompleteModel* autoCompleteModel;

        QSettings* settings;




        QStringList splitCsv(QString csvString);


    signals:

    public slots:
};

#endif // QMLCOMM_H
