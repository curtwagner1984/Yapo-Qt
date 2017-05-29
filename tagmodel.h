#ifndef TAGMODEL_H
#define TAGMODEL_H

#include <QObject>
#include "basiclistmodel.h"
#include "dbmanager.h"


class TagModel : public BasicListModel
{
    Q_OBJECT
public:
    TagModel(DbManager* dbManager);
    QVariant data(const QModelIndex &index, int role) const;
    void search(const QString searchString);
    void getActorTags(const QString actorId);
    Q_INVOKABLE void getActorTagsForTagger(const QString actorId);
    Q_INVOKABLE void getSceneTagsForTagger(const QString sceneId);

//    Q_INVOKABLE bool addTag(QString tagToAddId, QString tagToAddName, QString tagOfType, QString tagOfId);
//    Q_INVOKABLE bool removeTag(QString tagToRemoveId, QString tagOfType, QString tagOfId , bool removeFromDb);

    void getWebsiteTags(const QString websiteId);
    QHash<int, QByteArray> roleNames() const;


    enum TagRoles {
            IdRole = Qt::UserRole + 1,
            ThumbRole = Qt::UserRole + 2,
            ThumbRole64 = Qt::UserRole + 3,
            ThumbRole320 = Qt::UserRole + 4,
            NameRole = Qt::UserRole + 5,
            NumberOfScenesRole = Qt::UserRole + 6,
            NumberOfPicturesRole = Qt::UserRole + 7,
            NumberOfActorsRole = Qt::UserRole + 8


        };
private:
    QString NUMBER_OF_PICTURES = "SELECT COUNT (*) FROM ( "
            "SELECT Picture.* FROM Picture "
            "JOIN Picture_Tag ON Picture.id = Picture_Tag.picture_id "
            "WHERE Picture_Tag.tag_id = Tag.id "
            "UNION "
            "SELECT Picture.* FROM Picture JOIN Picture_Actor ON Picture.id = Picture_Actor.picture_id "
            "JOIN Actor ON Actor.id = Picture_Actor.actor_id "
            "JOIN Actor_Tag ON Actor.id = Actor_Tag.actor_id "
            "WHERE Actor_Tag.tag_id = Tag.id "
            "UNION "
            "SELECT Picture.* FROM Picture JOIN Picture_Website ON Picture.id = Picture_Website.picture_id "
            "JOIN Website ON Website.id = Picture_Website.website_id "
            "JOIN Website_Tag ON Website.id = Website_Tag.website_id "
            "WHERE Website_Tag.tag_id = Tag.id)";

    QString NUMBER_OF_ACTORS = " SELECT COUNT(*) FROM Actor_Tag WHERE Actor_Tag.tag_id = Tag.id ";


//    QString NUMBER_OF_SCENES = " SELECT COUNT(*)  FROM "
//            "( SELECT T1.* FROM (SELECT * FROM Scene) AS T1 "
//            "JOIN (SELECT * FROM Scene_Tag) AS T2 ON T1.id = T2.scene_id "
//            "UNION "
//            "SELECT Scene.* FROM Scene JOIN Scene_Actor ON Scene.id = Scene_Actor.scene_id "
//            "JOIN Actor ON Actor.id = Scene_Actor.actor_id "
//            "JOIN Actor_Tag ON Actor.id = Actor_Tag.actor_id "
//            "JOIN Tag ON Tag.id = Actor_Tag.tag_id "
//            "UNION "
//            "SELECT Scene.* FROM Scene JOIN Scene_Website ON Scene.id = Scene_Website.scene_id "
//            "JOIN Website ON Website.id = Scene_Website.website_id "
//            "JOIN Website_Tag ON Website.id = Website_Tag.website_id "
//            "JOIN Tag ON Tag.id = Website_Tag.tag_id) ";


    QString NUMBER_OF_SCENES = "SELECT COUNT (*) FROM ("
                "SELECT Scene.* FROM SCENE "
                "JOIN Scene_Tag ON Scene.id = Scene_Tag.scene_id "
                "WHERE Scene_Tag.tag_id = Tag.id "
                "UNION "
                "SELECT Scene.* FROM Scene JOIN Scene_Actor ON Scene.id = Scene_Actor.scene_id "
                "JOIN Actor ON Actor.id = Scene_Actor.actor_id "
                "JOIN Actor_Tag ON Actor.id = Actor_Tag.actor_id "
                "WHERE Actor_Tag.tag_id = Tag.id "
                "UNION "
                "SELECT Scene.* FROM Scene JOIN Scene_Website ON Scene.id = Scene_Website.scene_id "
                "JOIN Website ON Website.id = Scene_Website.website_id "
                "JOIN Website_Tag ON Website.id = Website_Tag.website_id "
                "WHERE Website_Tag.tag_id = Tag.id)";


    QString NUMBER_OF_WEBSITES = " SELECT COUNT(*) FROM Website_Tag WHERE Website_Tag.tag_id = Tag.id ";



    QString SEARCH_SELECT = QString("SELECT * ,"
                                    "( %1 ) as "
                                    "NumberOfScenes, "
                                    "( %2) as "
                                    "NumberOfActors, "
                                    "( %3) as "
                                    "NumberOfPictures, "
                                    "( %4) as "
                                    "NumberOfWebsites ").arg(NUMBER_OF_SCENES,NUMBER_OF_ACTORS,NUMBER_OF_PICTURES,NUMBER_OF_WEBSITES);

    QString SEARCH_FROM = "FROM Tag";
    QString SEARCH_WHERE = "WHERE Tag.name LIKE '%%1%'";



    QString ACTOR_SEARCH_SELECT = "SELECT * ";
    QString ACTOR_SEARCH_FROM = "FROM Tag JOIN Actor_Tag ON Tag.id = Actor_Tag.tag_id ";
    QString ACTOR_SEARCH_WHERE = "WHERE Actor_Tag.actor_id = %1";
    QString ACTOR_ORDER_BY = " ORDER BY Tag.name";

    QString WEBSITE_SEARCH_SELECT = "SELECT * ";
    QString WEBSITE_SEARCH_FROM = "FROM Tag JOIN Website_Tag ON Tag.id = Website_Tag.tag_id ";
    QString WEBSITE_SEARCH_WHERE = "WHERE Website_Tag.website_id = %1";


    QString SCENE_SEARCH_SELECT = "SELECT * ";
    QString SCENE_SEARCH_FROM = "FROM Tag JOIN Scene_Tag ON Tag.id = Scene_Tag.tag_id ";
    QString SCENE_SEARCH_WHERE = "WHERE Scene_Tag.scene_id = %1";
    QString SCENE_ORDER_BY = " ORDER BY Tag.name";


};

#endif  // TAGMODEL_H
