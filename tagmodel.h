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
    QString SEARCH_SELECT = "SELECT * ,"
                            "(SELECT COUNT(*) FROM Scene_Tag WHERE Scene_Tag.tag_id = Tag.id) as "
                            "NumberOfScenes, "
                            "(SELECT COUNT(*) FROM Actor_Tag WHERE Actor_Tag.tag_id = Tag.id) as "
                            "NumberOfActors, "
                            "(SELECT COUNT(*) FROM Picture_Tag WHERE Picture_Tag.tag_id = Tag.id) as "
                            "NumberOfPictures ";

    QString SEARCH_FROM = "FROM Tag";
    QString SEARCH_WHERE = "WHERE Tag.name LIKE '%%1%'";



    QString ACTOR_SEARCH_SELECT = "SELECT * ";
    QString ACTOR_SEARCH_FROM = "FROM (%1 FROM Tag) AS T1 "
                                "JOIN (SELECT * FROM Actor_Tag WHERE Actor_Tag.actor_id = %2) AS T2 "
                                "ON T1.id = T2.tag_id";


};

#endif  // TAGMODEL_H
