#include "mediafoldermodel.h"
#include <QSqlRecord>
#include <QDebug>

MediafolderModel::MediafolderModel(DbManager *dbManager)
                :BasicListModel(dbManager)
{
    qDebug() << "initiating MediafolderModel  ...";
    this->MODEL_TYPE = "MediafolderModel";
    this->baseSqlSelect =  "SELECT * ";
    this->baseSqlFrom = "FROM MediaFolder";
    this->generateSqlLimit();
    this->search("");
}

//int MediafolderModel::rowCount(const QModelIndex &parent) const
//{
//    return this->mediaFolders->size();
//}

//int MediafolderModel::columnCount(const QModelIndex &parent) const
//{
//    return 6;
//}

QVariant MediafolderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }

    QMap<QString,QVariant> currentItem = this->items.at(index.row());

    if (role == IdRole){
        return currentItem["id"];
    }else if (role == PathRole){
        return currentItem["path_to_dir"];
    }else if (role == IsPictureRole){
        return currentItem["is_picture"];
    }else if (role == IsVideoRole){
        return currentItem["is_video"];
    }else{
        return QVariant();
    }
}

void MediafolderModel::search(const QString searchString)
{
    QString escapedSearchString = this->escaleSqlChars(searchString);
    this->baseSqlWhere = "WHERE MediaFolder.path_to_dir LIKE '%" + searchString  +"%'";

//  Resets count and gets number of items and executes search
    this->baseSearch();
}

QHash<int, QByteArray> MediafolderModel::roleNames() const
{
    QHash<int, QByteArray> roles;
        roles[IdRole] = "id";
        roles[PathRole] = "path";
        roles[IsVideoRole] = "isVid";
        roles[IsPictureRole] = "isPic";
        return roles;
}







