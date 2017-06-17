#ifndef ACTORMODEL_H
#define ACTORMODEL_H

#include <QObject>
#include "basiclistmodel.h"
#include "dbmanager.h"


class ActorModel : public BasicListModel
{
    Q_OBJECT

public:
    ActorModel();

     QVariant data(const QModelIndex &index, int role) const;

     bool setData(const QModelIndex &index, const QVariant &value, int role);

     Q_INVOKABLE void search (const QString searchString);
     Q_INVOKABLE void searchById (const QString actorId);
     QHash<int, QByteArray> roleNames() const;


     Q_INVOKABLE void getTagActors (const QString tagId);

     Q_INVOKABLE void getSceneActorsForTagger(const QString sceneId);
     Q_INVOKABLE void getPictureActorsForTagger(const QString pictureId);

//    Q_INVOKABLE QVariant directData(QString roleName, int index);
     Q_INVOKABLE void setOrder(QString orderBy, QString orderDirection);


    enum ActorRoles {
            IdRole = Qt::UserRole + 1,
            ThumbRole = Qt::UserRole + 2,
            ThumbRole64 = Qt::UserRole + 3,
            ThumbRole320 = Qt::UserRole + 4,
            NumberOfScenesRole = Qt::UserRole + 5,
            NumberOfPicturesRole = Qt::UserRole + 6,
            NameRole = Qt::UserRole + 7,
            GenderRole = Qt::UserRole + 8,
            DobRole = Qt::UserRole + 9,
            AgeRole = Qt::UserRole + 10,
            RatingRole = Qt::UserRole + 11,
            NumberOfTags = Qt::UserRole + 13,
            SelectedRole = Qt::UserRole + 14

        };
private:

    QString SEARCH_SELECT = "SELECT * ,"
                            "(SELECT COUNT(*) FROM Scene_Actor WHERE Scene_Actor.actor_id = Actor.id) as NumberOfScenes, "
                            "(SELECT COUNT(*) FROM Picture_Actor WHERE Picture_Actor.actor_id = Actor.id) as NumberOfPictures, "
                            "(SELECT COUNT(*) FROM Actor_tag WHERE Actor_tag.actor_id = Actor.id) as NumberOfTags ";


    QString SEARCH_FROM = "FROM Actor";

    QString SEARCH_WHERE = "WHERE Actor.name LIKE '%%1%'";

    QString SEARCH_BY_ID_WHERE = "WHERE Actor.id = %1";


    QString TAG_SEARCH_FROM = "FROM ACTOR "
            "JOIN Actor_Tag ON Actor.id = Actor_Tag.actor_id "
            "WHERE Actor_Tag.tag_id = %1";


    QString SCENE_SEARCH_SELECT = "SELECT * ";
    QString SCENE_SEARCH_FROM = "FROM Actor JOIN Scene_Actor ON Actor.id = Scene_Actor.actor_id ";
    QString SCENE_SEARCH_WHERE = "WHERE Scene_Actor.scene_id = %1";
    QString SCENE_ORDER_BY = " ORDER BY Actor.name";

    QString PICTURE_SEARCH_SELECT = "SELECT * ";
    QString PICTURE_SEARCH_FROM = "FROM Actor JOIN Picture_Actor ON Actor.id = Picture_Actor.actor_id ";
    QString PICTURE_SEARCH_WHERE = "WHERE Picture_Actor.picture_id = %1";
    QString PICTURE_ORDER_BY = " ORDER BY Actor.name";




};

#endif // ACTORMODEL_H
