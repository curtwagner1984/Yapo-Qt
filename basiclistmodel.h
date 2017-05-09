#ifndef BASICLISTMODEL_H
#define BASICLISTMODEL_H

#include <QAbstractListModel>
#include "dbmanager.h"


class BasicListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    BasicListModel(DbManager* dbManager);
    int rowCount(const QModelIndex &parent) const;
    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);
    ~BasicListModel();
protected:
    DbManager* dbManager;
    QList<QMap<QString,QVariant>> items;
    QList<QMap<QString,QVariant>> itemCount;
    QList<int> randomOrderList;

    QString sqlStmt ();
    QString countSqlStmt ();
    QString escaleSqlChars (QString unescapedString);
    void generateSqlLimit();
    void baseSearch();
    void noLimitSearch();

    QString countSqlSelect = "SELECT COUNT(*)";
    QString baseSqlSelect = "";
    QString baseSqlFrom = "";
    QString baseSqlWhere = "";
    QString baseSqlOrder = "";
    QString baseSqlLimit = "";

    int count = 0;
    int NUMBER_OF_ITEMS_PER_PAGE = 100;
    int currentPageNumber = 0;
    bool isAutoComplete = false;
    void clear();

};

#endif // BASICLISTMODEL_H
