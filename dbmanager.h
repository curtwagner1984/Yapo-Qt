#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QList>
#include <QSqlQuery>
#include <QStringList>
#include <QObject>


class DbManager: public QObject
{
    Q_OBJECT

public:
    DbManager(QObject *parent = 0);
    DbManager(const QString& path, QObject *parent = 0);
    void createTables();

//    Search Functions
    QList<QMap<QString, QVariant>> actorSearch(const QString searchString);
    QList<QMap<QString, QVariant>> sceneSearch(const QString searchString);
    QList<QMap<QString, QVariant>> pictureSearch(const QString searchString);
    QList<QMap<QString, QVariant>> mediaFolderSearch(const QString searchString);

    QList<QMap<QString, QVariant>> getScenesBeforeFFprobe();
    QList<QMap<QString, QVariant>> getActorsBeforeScrape();

    QList<QMap<QString, QVariant>> getAllScenes();
    QList<QMap<QString, QVariant>> getAllPictures();
    QList<QMap<QString, QVariant>> getTaggingElements();


    QList<QMap<QString, QVariant>> executeArbitrarySqlWithReturnValue(const QString sqlStatment);

    bool executeArbitrarySqlWithoutReturnValue(const QString sqlStatment);
    bool executeArbitrarySqlWithoutReturnValueForTransaction(QSqlQuery query);

    bool beginTransaction();
    bool commitTransaction();

//    Add Single Item Functions
    bool addActor(QString actorName, bool isMainstream);
    bool addTagWithRelation (QString tagId, QString tagName, QString tagType, QString tagOfId);

    bool addTag(QString tagName);
    bool addWebsite(QString websiteName);
    bool addScene(QString name, QString path);
    bool addPicture(QString name, QString path);
    bool addMediaFolder(QString folderPath, bool isPicture, bool isVideo);


    //  Bulk Add functions
    bool addActors(QList<QStringList> scenesToAdd);
    bool addActorsAliases (QList<QStringList> aliasesToAdd);



    bool addSceneActors (QList<QStringList> sceneActorsToAdd);
    bool addSceneTags (QList<QStringList> sceneTagsToAdd);
    bool addSceneWebsites (QList<QStringList> sceneWebsitesToAdd);

    bool addPictureActors (QList<QStringList> pictureActorsToAdd);
    bool addPictureTags (QList<QStringList> pictureTagsToAdd);
    bool addPictureWebsite (QList<QStringList> pictureWebsitesToAdd);

    bool addTags(QList<QStringList> tagsToAdd);
    bool addWebsites(QList<QStringList> websitesToAdd);
    bool addScenes(QList<QStringList> scenesToAdd);
    bool addPictures(QList<QStringList> picturesToAdd);
    bool addTreeFolders(QList<QStringList> treeFoldersToAdd);

    bool updateScenes(QList<QMap<QString, QVariant>> scenesToUpdate);
    bool updateActors(QList<QMap<QString, QVariant>> actorsToUpdate);



private:
    QSqlDatabase m_db;
    bool echo;
    QString generateBulkInsertQueryString(QList<QStringList> bulkInsertArgumentList);

    QString generateUpdateQueryString(QMap<QString, QVariant> updatedObject);

    QString escapeSqlChars(QString string);


    bool executeQuery(QString sqlStmt, QString sendingFunction);
    bool executeQueryForTransaction(QString sqlStmt, QString sendingFunction);
    bool executeQuery(QSqlQuery query, QString sendingFunction);


    QList<QMap<QString, QVariant>> parseQueryResult(QSqlQuery query);
    QList<QMap<QString, QVariant>> executeFetchQueryWrapper(QString sqlStmt, QString callingFunction );


};

#endif // DBMANAGER_H
