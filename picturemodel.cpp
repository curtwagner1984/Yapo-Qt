#include "picturemodel.h"
#include <QDebug>
#include <QFileInfo>

PictureModel::PictureModel(DbManager *dbManager)
             : BasicListModel(dbManager)
{
    qDebug() << "Making test picture search ...";


    this->baseSqlSelect =  "SELECT * ";
    this->baseSqlFrom = "FROM Picture";
    this->generateSqlLimit();


    this->search("");
}

QVariant PictureModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
            return QVariant();
        }

        QMap<QString,QVariant> currentItem = this->items.at(index.row());

        if (role == IdRole){
            return currentItem["id"];
        }else if (role == ThumbRole){
            return currentItem["path_to_file"];
        }else if (role == ThumbRole64){
            QString currentThumbPath = currentItem["thumbnail"].toString();
            QFileInfo fInfo = QFileInfo(currentThumbPath);
            QString path = fInfo.path();
            path = path + "/profile_64.jpg";
            return path;
        }else if (role == PathRole){
            return currentItem["path_to_file"];
        }else if (role == NameRole){
            return currentItem["name"];
        }else{
            return QVariant();
        }
}

void PictureModel::search(const QString searchString)
{
    QString escapedSearchString = this->escaleSqlChars(searchString);
    this->baseSqlWhere = "WHERE Picture.path_to_file LIKE '%" + escapedSearchString  +"%'";

//  Resets count and gets number of items and executes search
    this->baseSearch();
}

void PictureModel::getActorPictures(const QString actorId)
{
    this->baseSqlWhere = "";
    this->baseSqlOrder = "";
    this->baseSqlFrom = QString("FROM (SELECT * FROM Picture) AS T1 "
                                "JOIN (SELECT * FROM Picture_Actor WHERE Picture_Actor.actor_id = %1) AS T2 "
                                "ON T1.id = T2.picture_id").arg(actorId);
    this->baseSearch();
}

QHash<int, QByteArray> PictureModel::roleNames() const
{
    QHash<int, QByteArray> roles;
            roles[IdRole] = "id";
            roles[ThumbRole] = "thumb";
            roles[ThumbRole64] = "thumb_64";
            roles[ThumbRole360] = "thumb_360";
            roles[PathRole] = "path";
            roles[NameRole] = "name";
            return roles;
}
