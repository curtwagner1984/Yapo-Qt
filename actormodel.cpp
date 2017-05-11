#include "actormodel.h"
#include <qdebug.h>
#include <QFileInfo>




ActorModel::ActorModel(DbManager *dbManager)
    : BasicListModel(dbManager)
{
    qDebug() << "Making test actor search ...";

    this->baseSqlSelect =  SEARCH_SELECT;
    this->baseSqlFrom = SEARCH_FROM ;

    this->generateSqlLimit();
    this->search("");

}



QVariant ActorModel::data(const QModelIndex &index, int role) const
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



void ActorModel::search(const QString searchString)
{

    QString escapedSearchString = this->escaleSqlChars(searchString);
    this->baseSqlSelect =  SEARCH_SELECT;
    this->baseSqlFrom = SEARCH_FROM ;
    this->baseSqlWhere = SEARCH_WHERE.arg(escapedSearchString);

//  Resets count and gets number of items and executes search
    this->baseSearch();




}

void ActorModel::getTagActor(const QString tagId)
{


    this->baseSqlFrom = TAG_SEARCH_FROM.arg(tagId);
    this->baseSqlWhere = "";

//  Resets count and gets number of items and executes search
    this->baseSearch();

}


QHash<int, QByteArray> ActorModel::roleNames() const
{
    QHash<int, QByteArray> roles;
            roles[IdRole] = "id";
            roles[ThumbRole] = "thumb";
            roles[ThumbRole64] = "thumb_64";
            roles[ThumbRole320] = "thumb_320";
            roles[NumberOfScenesRole] = "numberOfScenes";
            roles[NumberOfPicturesRole] = "numberOfPictures";
            roles[NameRole] = "name";
            roles[GenderRole] = "gender";
            return roles;

}






