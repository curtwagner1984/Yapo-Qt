#ifndef BASICLISTMODEL_H
#define BASICLISTMODEL_H

#include <QAbstractListModel>
#include "dbmanager.h"

class BasicListModel : public QAbstractListModel {
  Q_OBJECT
 public:
  BasicListModel();

  Q_INVOKABLE void init(DbManager *dbManager);


  int rowCount(const QModelIndex &parent) const;
  bool canFetchMore(const QModelIndex &parent) const;
  void fetchMore(const QModelIndex &parent);

  virtual QHash<int, QByteArray> roleNames() const = 0;
  virtual QVariant data(const QModelIndex &index, int role) const = 0;

  Q_INVOKABLE bool canFetchMore();
  Q_INVOKABLE int rowCount();
  Q_INVOKABLE QVariant directData(QString roleName, int index);

  Q_INVOKABLE bool addItem(QString itemToAddId, QString itemToAddName,
                           QString itemToAddTableName,
                           QString itemToAddRelationTableName,
                           QString itemToAddRelationItemId,
                           QString itemToAddType,
                           QString itemRelationType
                           );


  Q_INVOKABLE bool removeItem(QString itemToRemoveId,
                           QString itemToRemoveTableName,
                           QString itemToRemoveRelationTableName,
                           QString itemToRemoveRelationItemId,
                           QString itemToRemoveType,
                           QString itemRelationType,
                           bool deleteFromDb
                           );

  Q_INVOKABLE int getCount();
  Q_INVOKABLE void clear();
  ~BasicListModel();

 protected:
  DbManager *dbManager;
  QList<QMap<QString, QVariant>> items;
  QList<QMap<QString, QVariant>> itemCount;
  QList<int> randomOrderList;

  QString sqlStmt();
  QString countSqlStmt();
  QString escaleSqlChars(QString unescapedString);
  QString MODEL_TYPE;
  void generateSqlLimit();
  void baseSearch();
  void noLimitSearch();

  void setOrder(QString orderBy, QString orderDirection);

  QString countSqlSelect = "SELECT COUNT(*)";
  QString baseSqlSelect = "";
  QString baseSqlFrom = "";
  QString baseSqlWhere = "";
  QString baseSqlOrder = "";
  QString baseSqlLimit = "";

  int count = 0;
  int NUMBER_OF_ITEMS_PER_PAGE = 30;
  int currentPageNumber = 0;
  bool isAutoComplete = false;
};

#endif  // BASICLISTMODEL_H
