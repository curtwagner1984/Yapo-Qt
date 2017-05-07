#include "autocompletemodel.h"
#include <QDebug>
#include <QFileInfo>
#include <QModelIndex>


AutoCompleteModel::AutoCompleteModel(DbManager *dbManager)
    :BasicListModel(dbManager)
{
    qDebug() << "Making test autocomplete search ...";
    this->isAutoComplete = true;

    this->baseSqlSelect =  "SELECT *";

    this->baseSqlFrom = "FROM ( "
                        "SELECT Actor.id as id, Actor.name as name, 'Actor' as TableName, 'na' as alias_of, 'na' as alias_of_id, Actor.thumbnail as thumbnail FROM Actor "
                        "UNION ALL "
                        "SELECT Tag.id as id, Tag.name as name, 'Tag' as TableName, 'na' as alias_of,'na' as alias_of_id, Tag.thumbnail as thumbnail FROM Tag "
                        "UNION ALL "
                        "SELECT Website.id as id, Website.name as name , 'Website' as TableName, 'na' as alias_of,'na' as alias_of_id, Website.thumbnail as thumbnail FROM Website "
                        "UNION ALL "
                        "SELECT ActorAlias.id as id, ActorAlias.name as name, 'ActorAlias' as TableName, Actor.name as alias_of, Actor.id as alias_of_id, Actor.thumbnail as thumbnail FROM ActorAlias "
                        "JOIN Actor on Actor.id = ActorAlias.actor_id)";

    this->baseSqlLimit = "LIMIT 15";
//    this->generateSqlLimit();
//    this->search("");
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
                QString temp = currentItem["name"].toString();
                return temp.replace(this->currentSearchString, "<b>" + this->currentSearchString + "</b>", Qt::CaseInsensitive);
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

void AutoCompleteModel::search(const QString searchString)
{
    QString escapedSearchString = this->escaleSqlChars(searchString);
    this->baseSqlWhere = "WHERE name LIKE '%" + escapedSearchString  +"%'";
    this->baseSqlOrder = "ORDER BY name";

//  Resets count and gets number of items and executes search
    this->currentSearchString = searchString;
    this->baseSearch();

    QModelIndex a = QModelIndex();
    this->beginInsertRows(a,this->items.size(), this->items.size() + 2 );

    QMap<QString,QVariant> newActor;
    newActor["name"] = QString("Create new Actor '%1'").arg(this->currentSearchString);
    newActor["TableName"] = QString("New Actor");
    this->items.append(newActor);

    QMap<QString,QVariant> newTag;
    newTag["name"] = QString("Create new Tag '%1'").arg(this->currentSearchString);
    newTag["TableName"] = QString("New Tag");
    this->items.append(newTag);

    QMap<QString,QVariant> newWebsite;
    newTag["name"] = QString("Create new Website '%1'").arg(this->currentSearchString);
    newTag["TableName"] = QString("New Website");
    this->items.append(newTag);


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
