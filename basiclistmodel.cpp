#include "basiclistmodel.h"
#include <QDebug>

BasicListModel::BasicListModel(DbManager *dbManager) {
  this->dbManager = dbManager;
  this->items = QList<QMap<QString,QVariant>>();
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
  this->beginInsertRows(parent, this->items.size(),
                        this->items.size() + itmesToAppend.size() - 1);
  this->items.append(itmesToAppend);
  this->endInsertRows();
}

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
  if (!this->isAutoComplete)
  {
    this->currentPageNumber = 0;
    this->generateSqlLimit();

    QString countStmt = this->countSqlStmt();
    this->itemCount =
        this->dbManager->executeArbitrarySqlWithReturnValue(countStmt);
    this->count = itemCount.at(0)["COUNT(*)"].toInt();
  };

  this->beginResetModel();
  this->clear();
  this->items =
      this->dbManager->executeArbitrarySqlWithReturnValue(this->sqlStmt());

  if (this->isAutoComplete){

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

void BasicListModel::clear()
{
    if (this->items.size() > 0){
        this->items.clear();
    }

}
