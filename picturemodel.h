#ifndef PICTUREMODEL_H
#define PICTUREMODEL_H

#include "basiclistmodel.h"
#include "dbmanager.h"

class PictureModel : public BasicListModel
{
    Q_OBJECT
public:
    PictureModel();
    QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE void search(const QString searchString);
    void getActorPictures(const QString actorId);
    void getTagPictures(const QString tagId);
    void getWebsitePictures(const QString websiteId);
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void setOrder(QString orderBy, QString orderDirection);

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


    QString TAG_SEARCH_SELECT = "SELECT * ";
    QString TAG_SEARCH_FROM =
            "FROM ( "
            "SELECT T1.* "
            "FROM (SELECT * FROM Picture) AS T1 "
            "JOIN (SELECT * FROM Picture_Tag WHERE Picture_Tag.tag_id = %1) AS T2 "
            "ON T1.id = T2.picture_id "
            "UNION "
            "SELECT Picture.* FROM Picture "
            "JOIN Picture_Actor ON Picture.id = Picture_Actor.picture_id "
            "JOIN Actor ON Actor.id = Picture_Actor.actor_id "
            "JOIN Actor_Tag ON Actor.id = Actor_Tag.actor_id "
            "JOIN Tag ON Tag.id = Actor_Tag.tag_id "
            "WHERE Tag.id = %1 "
            "UNION "
            "SELECT Picture.* FROM Picture "
            "JOIN Picture_Website ON Picture.id = Picture_Website.picture_id "
            "JOIN Website ON Website.id = Picture_Website.website_id "
            "JOIN Website_Tag ON Website.id = Website_Tag.website_id "
            "JOIN Tag ON Tag.id = Website_Tag.tag_id "
            "WHERE Tag.id = %1 "
            ") ";


    QString WEBSITE_SEARCH_SELECT = "SELECT * ";
    QString WEBSITE_SEARCH_FROM = "FROM Picture JOIN Picture_Website ON Picture.id = Picture_Website.picture_id ";
    QString WEBSITE_SEARCH_WHERE = "WHERE Picture_Website.website_id = %1 ";




};

#endif // PICTUREMODEL_H
