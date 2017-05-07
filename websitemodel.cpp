#include "websitemodel.h"
#include <QDebug>
#include <QFileInfo>

WebsiteModel::WebsiteModel(DbManager *dbManager)
             :BasicListModel(dbManager)
{
    qDebug() << "Making test website search ...";

    this->baseSqlSelect =  "SELECT * ,"
                           "(SELECT COUNT(*) FROM Scene_Website WHERE Scene_Website.website_id = Website.id) as NumberOfScenes, "
                           "(SELECT COUNT(*) FROM Picture_Website WHERE Picture_Website.website_id = Website.id) as NumberOfPictures ";

    this->baseSqlFrom = "FROM Website";
    this->generateSqlLimit();
    this->search("");

}

QVariant WebsiteModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
            return QVariant();
        }

        QMap<QString,QVariant> currentItem = this->items.at(index.row());

        if (role == IdRole){
            return currentItem["id"];
        }else if (role == ThumbRole){
            return currentItem["thumbnail"];
        }else if (role == ThumbRole64){
            QString currentThumbPath = currentItem["thumbnail"].toString();
            QFileInfo fInfo = QFileInfo(currentThumbPath);
            QString path = fInfo.path();
            path = path + "/profile_64.jpg";
            return path;
        }else if (role == ThumbRole320){
            QString currentThumbPath = currentItem["thumbnail"].toString();
            QFileInfo fInfo = QFileInfo(currentThumbPath);
            QString path = fInfo.path();
            path = path + "/profile_320.jpg";
            return path;
        }else if (role == NumberOfScenesRole){
            return currentItem["NumberOfScenes"];
        }else if (role == NumberOfPicturesRole){
            return currentItem["NumberOfPictures"];
        }else if (role == NameRole){
            return currentItem["name"];
        }else{
            return QVariant();
        }
}

void WebsiteModel::search(const QString searchString)
{
    QString escapedSearchString = this->escaleSqlChars(searchString);
    this->baseSqlWhere = "WHERE Website.name LIKE '%" + escapedSearchString  +"%'";

//  Resets count and gets number of items and executes search
    this->baseSearch();

}

QHash<int, QByteArray> WebsiteModel::roleNames() const
{
    QHash<int, QByteArray> roles;
            roles[IdRole] = "id";
            roles[ThumbRole] = "thumb";
            roles[ThumbRole64] = "thumb_64";
            roles[ThumbRole320] = "thumb_320";
            roles[NumberOfScenesRole] = "numberOfScenes";
            roles[NumberOfPicturesRole] = "numberOfPictures";
            roles[NameRole] = "name";
            return roles;

}
