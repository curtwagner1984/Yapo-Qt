#include "generalalias.h"
#include <QDebug>

GeneralAlias::GeneralAlias() : BasicListModel()
{
    this->MODEL_TYPE = "GeneralAlias";
    qDebug() << "General Alias Model initialized ...";
}

QVariant GeneralAlias::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }

  QMap<QString, QVariant> currentItem = this->items.at(index.row());

  if (role == IdRole) {
    return currentItem["id"];
  } else if (role == NameRole) {
    return currentItem["name"];
  } else if (role == ExemptFromOneWordRole) {
    return currentItem["is_exempt_from_one_word_search"];
  } else if (role == TableNameRole) {
    return currentItem["TableName"];
  } else {
    return QVariant();
  }
}

bool GeneralAlias::setData(const QModelIndex &index, const QVariant &value,
                           int role) {
  qDebug() << "Set data was called "
           << "index" << index << "Value " << value << "Role " << role;
  bool success = false;
  if (!index.isValid()) {
    return success;
  }

  QMap<QString, QVariant> currentItem = this->items.at(index.row());

  QString stmt = "UPDATE %1 SET %2 = '%3' WHERE id = %4";


  if (role == NameRole) {
    currentItem["name"] = value.toString();
    success = this->dbManager->executeArbitrarySqlWithoutReturnValue(
        stmt.arg(tableName, "name", currentItem["name"].toString(),
                 currentItem["id"].toString()));
  } else if (role == ExemptFromOneWordRole) {
    currentItem["is_exempt_from_one_word_search"] = value.toInt();
    success = this->dbManager->executeArbitrarySqlWithoutReturnValue(
        stmt.arg(tableName, "is_exempt_from_one_word_search",
                 currentItem["is_exempt_from_one_word_search"].toString(),
                 currentItem["id"].toString()));
  }

  return success;
}

void GeneralAlias::search(const QString aliasOfId, const QString aliasOf) {
  //    SELECT * FROM ActorAlias WHERE ActorAlias.actor_id = 431

  //    QString tableName;
  //    QString columnName;

  if (aliasOf == "Actor") {
    tableName = "ActorAlias";
    columnName = "actor_id";
  } else if (aliasOf == "Tag") {
    tableName = "TagAlias";
    columnName = "tag_id";
  } else if (aliasOf == "Website"){
    tableName = "WebsiteAlias";
    columnName = "website_id";
  }

  //    QString baseFrom = QString("FROM %1").arg(tableName);
  //    QString baseWhere = QString("WHERE %1.%2 =
  //    %3").arg(tableName,columnName,searchString);

  this->baseSqlSelect = SEARCH_SELECT.arg(tableName);
  this->baseSqlFrom = SEARCH_FROM.arg(tableName);
  this->baseSqlWhere = SEARCH_WHERE.arg(tableName, columnName, aliasOfId);

  this->generateSqlLimit();
  this->baseSearch();
}

bool GeneralAlias::deleteAlias(const QString aliasToDeleteId, const QString aliasOf)
{

    if (aliasOf == "Actor") {
      tableName = "ActorAlias";
      columnName = "actor_id";
    } else if (aliasOf == "Tag") {
      tableName = "TagAlias";
      columnName = "tag_id";
    } else if (aliasOf == "Website"){
      tableName = "WebsiteAlias";
      columnName = "website_id";
    }

    QString stmt = "DELETE FROM %1 WHERE id = %2";
    if (this->dbManager->executeArbitrarySqlWithoutReturnValue(stmt.arg(tableName,aliasToDeleteId)))
    {
        this->generateSqlLimit();
        this->baseSearch();
        return true;
    }else{
        return false;
    }

}

void GeneralAlias::updateAlias(QString aliasOf, QString updatedField,
                               QString updatedValue) {
  //    QString tableName;
  //    QString columnName;

  //    if (aliasOf == "Actor"){
  //        tableName = "ActorAlias";
  //        columnName = "actor_id";
  //    }else if (aliasOf == "Tag"){
  //        tableName = "TagAlias";
  //        columnName = "tag_id";
  //    }

  //    QString stmt = "UPDATE %1 SET %2 = '%3' WHERE id = %4";
  //    stmt = stmt.arg(tableName,updatedField,updatedValue,);

  //    QString stmt = QString("UPDATE %1 SET %2 = '%3' WHERE id = %4")
  //                       .arg(currentDetailObject["TableName"].toString(),
  //                       attribName,
  //                            attribValue,
  //                            currentDetailObject["id"].toString());
}

QHash<int, QByteArray> GeneralAlias::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[IdRole] = "id";
  roles[NameRole] = "name";
  roles[ExemptFromOneWordRole] = "isExemptFromOneWordSearch";
  roles[TableNameRole] = "tableName";
  return roles;
}
