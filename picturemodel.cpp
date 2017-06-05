#include "picturemodel.h"
#include <QDebug>
#include <QFileInfo>




PictureModel::PictureModel()
             : BasicListModel()
{
    qDebug() << "Making test picture search ...";

    this->MODEL_TYPE = "PictureModel";

    this->baseSqlSelect =  SEARCH_SELECT;
    this->baseSqlFrom = SEARCH_FROM;
    this->generateSqlLimit();

    qDebug() << "Picture Model initialized ...";
//    this->search("");
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
    this->baseSqlSelect =  SEARCH_SELECT;
    this->baseSqlFrom = SEARCH_FROM;
    this->baseSqlWhere = SEARCH_WHERE.arg(escapedSearchString);

//  Resets count and gets number of items and executes search
    this->baseSearch();
}

void PictureModel::getActorPictures(const QString actorId)
{
    this->baseSqlWhere = "";
    this->baseSqlOrder = "";
    this->baseSqlFrom = ACTOR_SEARCH_FROM.arg(actorId);
    this->baseSearch();
}

void PictureModel::getTagPictures(const QString tagId)
{
    this->baseSqlWhere = "";
    this->baseSqlOrder = "";
    this->baseSqlFrom = TAG_SEARCH_FROM.arg(tagId);
    this->baseSearch();

}

void PictureModel::getWebsitePictures(const QString websiteId)
{
    this->baseSqlWhere = this->WEBSITE_SEARCH_WHERE.arg(websiteId);;
    this->baseSqlOrder = "";
    this->baseSqlFrom = this->WEBSITE_SEARCH_FROM;
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

void PictureModel::setOrder(QString orderBy, QString orderDirection)
{
    BasicListModel::setOrder(orderBy,orderDirection);
}
