#ifndef ACTORMODEL_H
#define ACTORMODEL_H

#include <QObject>
#include "basiclistmodel.h"
#include "dbmanager.h"


class ActorModel : public BasicListModel
{
    Q_OBJECT
public:
    ActorModel(DbManager* dbManager);
    QVariant data(const QModelIndex &index, int role) const;
    void search (const QString searchString);
    void getTagActor (const QString tagId);
    QHash<int, QByteArray> roleNames() const;    


    enum ActorRoles {
            IdRole = Qt::UserRole + 1,
            ThumbRole = Qt::UserRole + 2,
            ThumbRole64 = Qt::UserRole + 3,
            ThumbRole320 = Qt::UserRole + 4,
            NumberOfScenesRole = Qt::UserRole + 5,
            NumberOfPicturesRole = Qt::UserRole + 6,
            NameRole = Qt::UserRole + 7,
            GenderRole = Qt::UserRole + 8

        };
private:

    QString SEARCH_SELECT = "SELECT * ,"
                            "(SELECT COUNT(*) FROM Scene_Actor WHERE Scene_Actor.actor_id = Actor.id) as NumberOfScenes, "
                            "(SELECT COUNT(*) FROM Picture_Actor WHERE Picture_Actor.actor_id = Actor.id) as NumberOfPictures, "
                            "(SELECT COUNT(*) FROM Actor_tag WHERE Actor_tag.actor_id = Actor.id) as NumberOfTags ";

    QString SEARCH_FROM = "FROM Actor";

    QString SEARCH_WHERE = "WHERE Actor.name LIKE '%%1%'";


    QString TAG_SEARCH_FROM = "FROM ACTOR "
            "JOIN Actor_Tag ON Actor.id = Actor_Tag.actor_id "
            "WHERE Actor_Tag.tag_id = %1";




};

#endif // ACTORMODEL_H
