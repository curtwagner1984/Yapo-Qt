#include "generalalias.h"

GeneralAlias::GeneralAlias(DbManager* dbManager)
             :BasicListModel(dbManager)
{

}

QVariant GeneralAlias::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
            return QVariant();
        }

        QMap<QString,QVariant> currentItem = this->items.at(index.row());

        if (role == IdRole){
            return currentItem["id"];
        }else if (role == NameRole){
            return currentItem["name"];
        }else if (role == ExemptFromOneWordRole){
            return currentItem["is_exempt_from_one_word_search"];
        }else{
            return QVariant();
        }
}

void GeneralAlias::search(const QString searchString, const QString aliasOf)
{
//    SELECT * FROM ActorAlias WHERE ActorAlias.actor_id = 431


    QString tableName;
    QString columnName;

    if (aliasOf == "Actor"){
        tableName = "ActorAlias";
        columnName = "actor_id";
    }

    QString baseFrom = QString("FROM %1").arg(tableName);
    QString baseWhere = QString("WHERE %1.%2 = %3").arg(tableName,columnName,searchString);
    this->baseSqlSelect = "SELECT *";
    this->baseSqlFrom = baseFrom;
    this->baseSqlWhere = baseWhere;

    this->generateSqlLimit();
    this->baseSearch();

}

QHash<int, QByteArray> GeneralAlias::roleNames() const
{
    QHash<int, QByteArray> roles;
            roles[IdRole] = "id";
            roles[NameRole] = "name";
            roles[ExemptFromOneWordRole] = "isExemptFromOneWordSearch";
            return roles;
}
