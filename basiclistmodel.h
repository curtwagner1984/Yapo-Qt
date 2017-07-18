/*
 *
 *
*/

#ifndef BASICLISTMODEL_H
#define BASICLISTMODEL_H

#include <QAbstractListModel>
#include <QFuture>
#include <QFutureWatcher>
#include "dbmanager.h"

class BasicListModel : public QAbstractListModel
{
        Q_OBJECT
        Q_PROPERTY(bool waitingForDbResponse READ waitingForDbResponse NOTIFY waitingForDbResponseChanged)

    public slots:
        void dbActionCompletedSlot(QString value);
        void fetchedMore();
        void baseSearchItemCountReturns();
        void baseSearchQueryReturns();

    signals:
        void baseSearchFinished();
        void waitingForDbResponseChanged();


    public:
        BasicListModel();

        bool waitingForDbResponse();

        Q_INVOKABLE void init(DbManager* dbManager);


        int rowCount(const QModelIndex& parent) const;
        bool canFetchMore(const QModelIndex& parent) const;
        void fetchMore(const QModelIndex& parent);

        virtual QHash<int, QByteArray> roleNames() const = 0;
        virtual QVariant data(const QModelIndex& index, int role) const = 0;

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


        Q_INVOKABLE bool removeItem(int indexOfItemToRemove,
                                    bool deleteFromDb
                                   );

        Q_INVOKABLE bool removeSelected(bool deleteFromDb
                                       );


        Q_INVOKABLE void selectAll();
        Q_INVOKABLE void selectNone();

        Q_INVOKABLE QList<int> getSelectedIndices();
        Q_INVOKABLE int getCount();
        Q_INVOKABLE void clear();
        ~BasicListModel();

    protected:
        DbManager* dbManager;
        QList<QMap<QString, QVariant>> items;
        QList<QMap<QString, QVariant>> itemCount;
        QList<int> randomOrderList;

        BasicListModel* super();

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


        //  Qt Concurrent stuff
        QFuture<QList<QMap<QString, QVariant>>> fetchMoreFuture;
        QFutureWatcher <void> fetchMoreFutureWatcher;

        QFuture<QList<QMap<QString, QVariant>>> itemCountFuture;
        QFutureWatcher <void> itemCountFutureWatcher;

        QFuture<QList<QMap<QString, QVariant>>> baseSearchFuture;
        QFutureWatcher <void> baseSearchFutureWatcher;

        QModelIndex currentParent;



        int count = 0;
        int NUMBER_OF_ITEMS_PER_PAGE = 100;
        int currentPageNumber = 0;
        bool isAutoComplete = false;

    private:

        void startWaitingForDbResponse();
        void stopWaitingForDbResponse();

        bool m_waitingForDbResponse = false;

};

#endif  // BASICLISTMODEL_H
