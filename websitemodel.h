#ifndef WEBSITEMODEL_H
#define WEBSITEMODEL_H

#include <QObject>
#include "basiclistmodel.h"
#include "dbmanager.h"

class WebsiteModel: public BasicListModel
{
    Q_OBJECT
public:
    WebsiteModel(DbManager* dbManager);
    QVariant data(const QModelIndex &index, int role) const;
    void search(const QString searchString);
    void getTagWebsites(const QString tagId);
    QHash<int, QByteArray> roleNames() const;


    enum TagRoles {
            IdRole = Qt::UserRole + 1,
            ThumbRole = Qt::UserRole + 2,
            ThumbRole64 = Qt::UserRole + 3,
            ThumbRole320 = Qt::UserRole + 4,
            NameRole = Qt::UserRole + 5,
            NumberOfScenesRole = Qt::UserRole + 6,
            NumberOfPicturesRole = Qt::UserRole + 7,


        };
private:
    QString SEARCH_SELECT = "SELECT * ,"
                            "(SELECT COUNT(*) FROM Scene_Website WHERE Scene_Website.website_id = Website.id) as NumberOfScenes, "
                            "(SELECT COUNT(*) FROM Picture_Website WHERE Picture_Website.website_id = Website.id) as NumberOfPictures ";

    QString SEARCH_FROM = "FROM Website";

    QString SEARCH_WHERE = "WHERE Website.name LIKE '%%1%'";


    QString TAG_SEARCH_SELECT = "SELECT * ,"
                            "(SELECT COUNT(*) FROM Scene_Website WHERE Scene_Website.website_id = Website.id) as NumberOfScenes, "
                            "(SELECT COUNT(*) FROM Picture_Website WHERE Picture_Website.website_id = Website.id) as NumberOfPictures ";

    QString TAG_SEARCH_FROM = "FROM Website "
                              "JOIN Website_Tag ON Website.id = Website_Tag.website_id "
                              "WHERE Website_Tag.tag_id = %1 ";

    QString TAG_SEARCH_WHERE = "";


};

#endif // WEBSITEMODEL_H
