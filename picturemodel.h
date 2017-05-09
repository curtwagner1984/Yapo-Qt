#ifndef PICTUREMODEL_H
#define PICTUREMODEL_H

#include "basiclistmodel.h"
#include "dbmanager.h"

class PictureModel : public BasicListModel
{
    Q_OBJECT
public:
    PictureModel(DbManager* dbManager);
    QVariant data(const QModelIndex &index, int role) const;
    void search(const QString searchString);
    void getActorPictures(const QString actorId);
    QHash<int, QByteArray> roleNames() const;

    enum PictureRoles {
                IdRole = Qt::UserRole + 1,
                ThumbRole = Qt::UserRole + 2,
                ThumbRole64 = Qt::UserRole + 3,
                ThumbRole360 = Qt::UserRole + 4,
                PathRole = Qt::UserRole + 5,
                NameRole = Qt::UserRole + 6

            };
private:
    QString SEARCH_SELECT = "SELECT * ";

    QString SEARCH_FROM = "FROM Picture";

    QString SEARCH_WHERE = "WHERE Picture.path_to_file LIKE '%%1%'";



    QString ACTOR_SEARCH_SELECT = "SELECT * ";
    QString ACTOR_SEARCH_FROM = "FROM (SELECT * FROM Picture) AS T1 "
                                "JOIN (SELECT * FROM Picture_Actor WHERE Picture_Actor.actor_id = %1) AS T2 "
                                "ON T1.id = T2.picture_id";
};

#endif // PICTUREMODEL_H
