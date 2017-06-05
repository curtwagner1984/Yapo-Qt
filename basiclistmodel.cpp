#include "basiclistmodel.h"
#include <QDebug>
#include <QHash>
#include <QModelIndex>

BasicListModel::BasicListModel() {

    this->items = QList<QMap<QString, QVariant>>();
}

void BasicListModel::init(DbManager *dbManager)
{
    this->dbManager = dbManager;
}

int BasicListModel::rowCount(const QModelIndex &parent) const {
  //    return this->itemCount.at(0)["COUNT(*)"].toInt();
  return this->items.size();
}

bool BasicListModel::canFetchMore(const QModelIndex &parent) const {
  int itemsSize = this->items.size();
  if (itemsSize == 0 || this->isAutoComplete) {
    return false;
  } else {
    int itemsCount = this->itemCount.at(0)["COUNT(*)"].toInt();
    bool needFetchMore = itemsSize < itemsCount;
    qDebug() << "canFetchMoreTriggered answer is: " << needFetchMore;
    return needFetchMore;
  }
}

void BasicListModel::fetchMore(const QModelIndex &parent) {
  qDebug() << "fetchmore triggered" << parent.row();

  this->currentPageNumber++;
  this->generateSqlLimit();
  QString stmt = this->sqlStmt();
  QList<QMap<QString, QVariant>> itmesToAppend =
      this->dbManager->executeArbitrarySqlWithReturnValue(stmt);
  if (itmesToAppend.size() == 0) {
    return;
  }
  this->beginInsertRows(parent, this->items.size(),
                        this->items.size() + itmesToAppend.size() - 1);
  this->items.append(itmesToAppend);
  this->endInsertRows();
}

QVariant BasicListModel::directData(QString roleName, int index) {
  QByteArray temp = roleName.toLatin1();
  QHash<int, QByteArray> roles = roleNames();
  int currentRole = roles.key(temp);
  QModelIndex ix = this->index(index);
  QVariant ans = this->data(ix, currentRole);
  return ans;
}

bool BasicListModel::addItem(QString itemToAddId, QString itemToAddName,
                             QString itemToAddTableName,
                             QString itemToAddRelationTableName,
                             QString itemToAddRelationItemId,
                             QString itemToAddType, QString itemRelationType) {
  bool ans = false;
  QString createNewItemStmt = QString("INSERT INTO %1 (name) VALUES ('%2')")
                                  .arg(itemToAddTableName, itemToAddName);

  if (itemToAddId == "") {
    if (this->dbManager->executeArbitrarySqlWithoutReturnValue(
            createNewItemStmt)) {
      QString lastInsertIdStmt = "SELECT LAST_INSERT_ROWID() as lastId";
      QList<QMap<QString, QVariant>> b =
          this->dbManager->executeArbitrarySqlWithReturnValue(lastInsertIdStmt);
      itemToAddId = b.at(0)["lastId"].toString();
    }
  }

  if (itemToAddRelationTableName != "") {
    QString firstColumnName = QString("%1_id").arg(itemToAddType).toLower();
    QString secondColumnName = QString("%1_id").arg(itemRelationType).toLower();
    QString relationInsertStmt =
        QString("INSERT INTO %1 (%2,%3) VALUES (%4,%5)")
            .arg(itemToAddRelationTableName, firstColumnName, secondColumnName,
                 itemToAddId, itemToAddRelationItemId);
    this->dbManager->executeArbitrarySqlWithoutReturnValue(relationInsertStmt);
  }

  QString getInsetedItemStmt = QString("SELECT * FROM %1 WHERE id = %2")
                                   .arg(itemToAddTableName, itemToAddId);

  QList<QMap<QString, QVariant>> addedItem =
      this->dbManager->executeArbitrarySqlWithReturnValue(getInsetedItemStmt);

  QModelIndex x = QModelIndex();
  this->beginInsertRows(x, this->items.size(), this->items.size());
  this->items.append(addedItem.at(0));
  this->endInsertRows();
  ans = true;

  return ans;

  //    bool ans = false;
  //    if (tagToAddId == "") {
  //      QString createStmt = "INSERT INTO Tag (name) VALUES ('%1')";
  //      createStmt = createStmt.arg(tagToAddName);
  //      if
  //      (this->dbManager->executeArbitrarySqlWithoutReturnValue(createStmt)) {
  //        QString lastInsertIdStmt = "SELECT LAST_INSERT_ROWID() as lastId";
  //        QList<QMap<QString, QVariant>> b =
  //            this->dbManager->executeArbitrarySqlWithReturnValue(lastInsertIdStmt);
  //        tagToAddId = b.at(0)["lastId"].toString();
  //      }
  //    }

  //    QString tableName = QString("%1_Tag").arg(tagOfType);
  //    QString firstColumnName = QString("%1_id").arg(tagOfType).toLower();

  //    QString insetRelationStmt = "INSERT INTO %1 (%2,%3) VALUES (%4,%5)";
  //    insetRelationStmt = insetRelationStmt.arg(tableName, firstColumnName,
  //    "tag_id",
  //                                              tagOfId, tagToAddId);
  //    if (this->dbManager->executeArbitrarySqlWithoutReturnValue(
  //            insetRelationStmt)) {
  //      QString selectStmt = "SELECT * FROM Tag WHERE Tag.id = %1";
  //      selectStmt = selectStmt.arg(tagToAddId);
  //      QList<QMap<QString, QVariant>> tagToAdd =
  //          this->dbManager->executeArbitrarySqlWithReturnValue(selectStmt);

  //      QModelIndex x = QModelIndex();
  //      this->beginInsertRows(x, this->items.size(), this->items.size());
  //      this->items.append(tagToAdd.at(0));
  //      this->endInsertRows();
  //      ans = true;
  //    }

  //    return ans;
}

bool BasicListModel::removeItem(QString itemToRemoveId,
                                QString itemToRemoveTableName,
                                QString itemToRemoveRelationTableName,
                                QString itemToRemoveRelationItemId,
                                QString itemToRemoveType,
                                QString itemRelationType, bool deleteFromDb) {
  bool ans = false;

//  finds index of a specific item by 'id', maybe move to a seperate function if needed elsewhere
  int removeIndex = -1;
  for (int i = 0; i < this->items.size(); i++) {
    QMap<QString, QVariant> temp = this->items.at(i);
    if (temp["id"].toString() == itemToRemoveId) {
      removeIndex = i;
      break;
    }
  }

  if (deleteFromDb) {
    QString deleteFromDbStmt = QString("DELETE FROM %1 WHERE id = %2")
                                   .arg(itemToRemoveTableName, itemToRemoveId);
    if (this->dbManager->executeArbitrarySqlWithoutReturnValue(
            deleteFromDbStmt)) {
        ans = true;

    }
  } else {
      QString firstColumnName = QString("%1_id").arg(itemToRemoveType).toLower();
      QString secondColumnName = QString("%1_id").arg(itemRelationType).toLower();
      QString removeRelationStmt = QString("DELETE FROM %1 WHERE %2=%3 AND %4=%5").arg(itemToRemoveRelationTableName,firstColumnName,itemToRemoveId,secondColumnName,itemToRemoveRelationItemId);

      if (this->dbManager->executeArbitrarySqlWithoutReturnValue(
              removeRelationStmt)) {
          ans = true;

      }
  }


  QModelIndex x = QModelIndex();
  this->beginRemoveRows(x,removeIndex,removeIndex);
  this->items.removeAt(removeIndex);
  this->endRemoveRows();
  return ans;
}

int BasicListModel::getCount() { return this->count; }

BasicListModel::~BasicListModel() { this->clear(); }

QString BasicListModel::sqlStmt() {
  return this->baseSqlSelect + " " + this->baseSqlFrom + " " +
         this->baseSqlWhere + " " + this->baseSqlOrder + " " +
         this->baseSqlLimit;
}

QString BasicListModel::countSqlStmt() {
  return this->countSqlSelect + " " + this->baseSqlFrom + " " +
         this->baseSqlWhere;
}

QString BasicListModel::escaleSqlChars(QString unescapedString) {
  QString escapedString = unescapedString;
  escapedString.replace(QString("'"), QString("''"));
  return escapedString;
}

void BasicListModel::generateSqlLimit() {
  this->baseSqlLimit =
      "LIMIT " + QString::number(this->NUMBER_OF_ITEMS_PER_PAGE) + " OFFSET " +
      QString::number(this->NUMBER_OF_ITEMS_PER_PAGE * this->currentPageNumber);
}

void BasicListModel::baseSearch() {
  if (!this->isAutoComplete) {
    this->currentPageNumber = 0;
    this->generateSqlLimit();

    QString countStmt = this->countSqlStmt();
    this->itemCount =
        this->dbManager->executeArbitrarySqlWithReturnValue(countStmt);
    if (this->itemCount.size() > 0) {
      qDebug() << "item count is: " << this->itemCount.size();
      this->count = itemCount.at(0)["COUNT(*)"].toInt();
    } else {
      this->count = 0;
    }
  };

  this->beginResetModel();
  this->clear();
  this->items =
      this->dbManager->executeArbitrarySqlWithReturnValue(this->sqlStmt());

  if (this->isAutoComplete) {
  }

  this->endResetModel();
}

void BasicListModel::noLimitSearch() {
  this->currentPageNumber = 0;
  //    this->generateSqlLimit();

  QString countStmt = this->countSqlStmt();
  this->itemCount =
      this->dbManager->executeArbitrarySqlWithReturnValue(countStmt);
  this->count = itemCount.at(0)["COUNT(*)"].toInt();

  this->beginResetModel();
  this->clear();
  this->items =
      this->dbManager->executeArbitrarySqlWithReturnValue(this->sqlStmt());
  this->endResetModel();
}

void BasicListModel::setOrder(QString orderBy, QString orderDirection) {
  QString orderByStmt = "AND (%1 IS NOT NULL AND %1 IS NOT 0) ORDER BY %1 %2";
  orderByStmt = orderByStmt.arg(orderBy, orderDirection);

  if (this->baseSqlOrder != orderByStmt) {
    this->baseSqlOrder = orderByStmt;
    this->beginResetModel();
    this->baseSearch();
    this->endResetModel();
  }
}

void BasicListModel::clear() {
  this->beginResetModel();
  if (this->items.size() > 0) {
    this->items.clear();
  }
  this->endResetModel();
}
