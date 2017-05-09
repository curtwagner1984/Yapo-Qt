#include "autocompletemodel.h"
#include <QDebug>
#include <QFileInfo>
#include <QModelIndex>





AutoCompleteModel::AutoCompleteModel(DbManager *dbManager)
    :BasicListModel(dbManager)
{
    qDebug() << "Making test autocomplete search ...";
    this->isAutoComplete = true;

    this->baseSqlSelect =  SEARCH_SELECT;

    this->baseSqlFrom = SEARCH_FROM;

    this->baseSqlLimit = SEARCH_LIMIT;

}

QVariant AutoCompleteModel::data(const QModelIndex &index, int role) const
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
            }else if (role == NameRole){                
                return currentItem["name"].toString();
            }else if (role == TableNameRole){
                return currentItem["TableName"];
            }else if (role == AliasOfRole){
                return currentItem["alias_of"];
            }else if (role == AliasOfIdRole){
                return currentItem["alias_of_id"];
            }else{
                return QVariant();
            }
}

void AutoCompleteModel::search(const QString searchString , QString searchType)
{
   if (searchType == "Tag"){
       this->tagSearch(searchString);
   }else{
       this->generalSearch(searchString);
   }



}

void AutoCompleteModel::tagSearch(const QString searchString)
{

    QString escapedSearchString = this->escaleSqlChars(searchString);
    this->baseSqlWhere = SEARCH_WHERE.arg(escapedSearchString);
    this->baseSqlOrder = SEARCH_ORDER;
    this->baseSqlFrom = TAG_SEARCH_FROM;

//  Resets count and gets number of items and executes search
    this->currentSearchString = searchString;
    this->baseSearch();

    QModelIndex a = QModelIndex();
    this->beginInsertRows(a,this->items.size(), this->items.size());


    QMap<QString,QVariant> newTag;
    newTag["name"] = this->currentSearchString;
    newTag["TableName"] = QString("New Tag");
    this->items.append(newTag);


    this->endInsertRows();

}

void AutoCompleteModel::generalSearch(const QString searchString)
{
    QString escapedSearchString = this->escaleSqlChars(searchString);
    this->baseSqlWhere = SEARCH_WHERE.arg(escapedSearchString);
    this->baseSqlOrder = SEARCH_ORDER;
    this->baseSqlFrom = SEARCH_FROM;

//  Resets count and gets number of items and executes search
    this->currentSearchString = searchString;
    this->baseSearch();

    QModelIndex a = QModelIndex();
    this->beginInsertRows(a,this->items.size(), this->items.size() + 2 );

    QMap<QString,QVariant> newActor;
    newActor["name"] = this->currentSearchString;
    newActor["TableName"] = QString("New Actor");
    this->items.append(newActor);

    QMap<QString,QVariant> newTag;
    newTag["name"] = this->currentSearchString;
    newTag["TableName"] = QString("New Tag");
    this->items.append(newTag);

    QMap<QString,QVariant> newWebsite;
    newTag["name"] = this->currentSearchString;
    newTag["TableName"] = QString("New Website");
    this->items.append(newWebsite);

    this->endInsertRows();

}

QHash<int, QByteArray> AutoCompleteModel::roleNames() const
{
    QHash<int, QByteArray> roles;
            roles[IdRole] = "id";
            roles[ThumbRole] = "thumb";
            roles[ThumbRole64] = "thumb_64";
            roles[ThumbRole320] = "thumb_320";
            roles[NameRole] = "name";
            roles[TableNameRole] = "tableName";
            roles[AliasOfRole] = "aliasOf";
            roles[AliasOfIdRole] = "aliasOfId";
            return roles;
}
