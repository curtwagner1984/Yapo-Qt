#include "basiclistmodel.h"
#include <QDebug>
#include <QFuture>
#include <QFutureWatcher>
#include <QHash>
#include <QModelIndex>
#include <QtConcurrent/QtConcurrent>

void BasicListModel::dbActionCompletedSlot(QString value)
{
    qDebug() << this << ":BasicListModel::testSlot recived signal " << value;
}

BasicListModel::BasicListModel() : QAbstractListModel()
{
    this->items = QList<QMap<QString, QVariant>>();
}



bool BasicListModel::waitingForDbResponse()
{
    return this->m_waitingForDbResponse;
}

void BasicListModel::init(DbManager* dbManager)
{
    this->dbManager = dbManager;
    QObject::connect(this->dbManager, &DbManager::queryCompleted, this,
                     &BasicListModel::dbActionCompletedSlot);
    QObject::connect(&this->fetchMoreFutureWatcher, &QFutureWatcher<void>::finished, this,
                     &BasicListModel::fetchedMore);
    QObject::connect(&this->itemCountFutureWatcher, &QFutureWatcher<void>::finished, this,
                     &BasicListModel::baseSearchItemCountReturns);
    QObject::connect(&this->baseSearchFutureWatcher, &QFutureWatcher<void>::finished, this,
                     &BasicListModel::baseSearchQueryReturns);
}

int BasicListModel::rowCount(const QModelIndex& parent) const
{
    //    return this->itemCount.at(0)["COUNT(*)"].toInt();
    return this->items.size();
}

bool BasicListModel::canFetchMore(const QModelIndex& parent) const
{
    int itemsSize = this->items.size();

    if (itemsSize == 0 || this->isAutoComplete)
    {
        return false;
    }
    else
    {
        int itemsCount = this->itemCount.at(0)["COUNT(*)"].toInt();
        bool needFetchMore = itemsSize < itemsCount;
        qDebug() << "canFetchMoreTriggered answer is: " << needFetchMore;
        return needFetchMore;
    }
}

void BasicListModel::fetchMore(const QModelIndex& parent)
{
    qDebug() << "fetchmore triggered";
    this->currentParent = parent;
    this->currentPageNumber++;
    this->generateSqlLimit();
    QString stmt = this->sqlStmt();
    this->fetchMoreFuture = QtConcurrent::run(
                                this->dbManager, &DbManager::executeArbitrarySqlWithReturnValue, stmt);
    this->fetchMoreFutureWatcher.setFuture(this->fetchMoreFuture);
    this->startWaitingForDbResponse();
    qDebug() << "fetchmore returned";
    //  this->future.waitForFinished();
    //  QList<QMap<QString, QVariant>> itmesToAppend = this->future.result();
    //  QList<QMap<QString, QVariant>> itmesToAppend =
    //      this->dbManager->executeArbitrarySqlWithReturnValue(stmt);
    //  if (itmesToAppend.size() == 0) {
    //    return;
    //  }
    //  this->beginInsertRows(parent, this->items.size(),
    //                        this->items.size() + itmesToAppend.size() - 1);
    //  this->items.append(itmesToAppend);
    //  this->endInsertRows();
}

void BasicListModel::fetchedMore()
{
    //  this->future.waitForFinished();
    qDebug() << "fetchedMore slot triggered";
    QList<QMap<QString, QVariant>> itmesToAppend = this->fetchMoreFuture.result();

    //  QList<QMap<QString, QVariant>> itmesToAppend =
    //      this->dbManager->executeArbitrarySqlWithReturnValue(stmt);
    if (itmesToAppend.size() == 0)
    {
        return;
    }

    qDebug() << "inserting rows";
    this->beginInsertRows(this->currentParent, this->items.size(),
                          this->items.size() + itmesToAppend.size() - 1);
    this->items.append(itmesToAppend);
    this->endInsertRows();
    this->stopWaitingForDbResponse();
    qDebug() << "row insert finished";
}

bool BasicListModel::canFetchMore()
{
    bool ans = false;
    QModelIndex ix;

    if (canFetchMore(ix))
    {
        fetchMore(ix);
        ans = true;
    }

    return ans;
}

int BasicListModel::rowCount()
{
    QModelIndex ix;
    return rowCount(ix);
}

QVariant BasicListModel::directData(QString roleName, int index)
{
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
                             QString itemToAddType, QString itemRelationType)
{
    bool ans = false;
    QString createNewItemStmt = QString("INSERT INTO %1 (name) VALUES ('%2')")
                                .arg(itemToAddTableName, itemToAddName);

    if (itemToAddId == "")
    {
        if (this->dbManager->executeArbitrarySqlWithoutReturnValue(
                createNewItemStmt))
        {
            QString lastInsertIdStmt = "SELECT LAST_INSERT_ROWID() as lastId";
            QList<QMap<QString, QVariant>> b =
                                            this->dbManager->executeArbitrarySqlWithReturnValue(lastInsertIdStmt);
            itemToAddId = b.at(0)["lastId"].toString();
        }
    }

    if (itemToAddRelationTableName != "")
    {
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
}

bool BasicListModel::removeItem(QString itemToRemoveId,
                                QString itemToRemoveTableName,
                                QString itemToRemoveRelationTableName,
                                QString itemToRemoveRelationItemId,
                                QString itemToRemoveType,
                                QString itemRelationType, bool deleteFromDb)
{
    bool ans = false;
    //  finds index of a specific item by 'id', maybe move to a seperate function
    //  if needed elsewhere
    int removeIndex = -1;

    for (int i = 0; i < this->items.size(); i++)
    {
        QMap<QString, QVariant> temp = this->items.at(i);

        if (temp["id"].toString() == itemToRemoveId)
        {
            removeIndex = i;
            break;
        }
    }

    if (deleteFromDb)
    {
        QString deleteFromDbStmt = QString("DELETE FROM %1 WHERE id = %2")
                                   .arg(itemToRemoveTableName, itemToRemoveId);

        if (this->dbManager->executeArbitrarySqlWithoutReturnValue(
                deleteFromDbStmt))
        {
            ans = true;
        }
    }
    else
    {
        QString firstColumnName = QString("%1_id").arg(itemToRemoveType).toLower();
        QString secondColumnName = QString("%1_id").arg(itemRelationType).toLower();
        QString removeRelationStmt =
            QString("DELETE FROM %1 WHERE %2=%3 AND %4=%5")
            .arg(itemToRemoveRelationTableName, firstColumnName, itemToRemoveId,
                 secondColumnName, itemToRemoveRelationItemId);

        if (this->dbManager->executeArbitrarySqlWithoutReturnValue(
                removeRelationStmt))
        {
            ans = true;
        }
    }

    QModelIndex x = QModelIndex();
    this->beginRemoveRows(x, removeIndex, removeIndex);
    this->items.removeAt(removeIndex);
    this->endRemoveRows();
    return ans;
}

bool BasicListModel::removeItem(int indexOfItemToRemove, bool deleteFromDb)
{
    int removeIndex = indexOfItemToRemove;
    QMap <QString, QVariant> itemToRemove = this->items.at(removeIndex);

    if (deleteFromDb)
    {
        QString removeStmt = QString("DELETE FROM %1 WHERE id = %2")
                             .arg(this->MODEL_TYPE, itemToRemove["id"].toString());
        this->dbManager->executeArbitrarySqlWithoutReturnValue(removeStmt);
        qDebug() << "BasicListModel::removeItem: removeStmt: " << removeStmt;
    }

    QModelIndex x = QModelIndex();
    this->beginRemoveRows(x, removeIndex, removeIndex);
    this->items.removeAt(removeIndex);
    this->endRemoveRows();
    return true;
}

bool BasicListModel::removeSelected(bool deleteFromDb)
{
    QList<int> selected = this->getSelectedIndices();
    this->beginResetModel();
    QList<QMap<QString, QVariant>>::iterator it = this->items.begin();

    while (it != this->items.end())
    {
        if ((*it)["isSelected"].toBool())
        {
            it = this->items.erase(it);
        }
        else
        {
            it++;
        }
    }

    //    for (int i = 0 ; i < selected.size(); i++)
    //    {
    //        this->items.removeAt(selected.at(i));
    //    }
    this->endResetModel();
    return true;
}

void BasicListModel::selectAll()
{
    QList<QMap<QString, QVariant>>::iterator i;
    qDebug() << "Entered BasicListModel::selectAll()";
    this->beginResetModel();

    for (i = this->items.begin(); i != this->items.end(); ++i)
    {
        qDebug() << "Before change";
        qDebug() << this->MODEL_TYPE << " Item id is: " << (*i)["id"].toInt() << " Item Selected Status is *" << (*i)["isSelected"].toBool() << "*";
        (*i)["isSelected"] = true;
        qDebug() << "After change";
        qDebug() << this->MODEL_TYPE << " Item id is: " << (*i)["id"].toInt() << " Item Selected Status is *" << (*i)["isSelected"].toBool() << "*";
    }

    this->endResetModel();
}

void BasicListModel::selectNone()
{
    QList<QMap<QString, QVariant>>::iterator i;
    this->beginResetModel();

    for (i = this->items.begin(); i != this->items.end(); ++i)
    {
        (*i)["isSelected"] = false;
    }

    this->endResetModel();
}

QList<int> BasicListModel::getSelectedIndices()
{
    QList<int> ans;

    for (int i = 0; i < this->items.length(); i++)
    {
        if (this->items[i]["isSelected"].toBool())
        {
            ans.append(i);
        }
    }

    return ans;
}

int BasicListModel::getCount()
{
    return this->count;
}

BasicListModel::~BasicListModel()
{
    this->clear();
}

BasicListModel* BasicListModel::super()
{
    return this;
}

QString BasicListModel::sqlStmt()
{
    return this->baseSqlSelect + " " + this->baseSqlFrom + " " +
           this->baseSqlWhere + " " + this->baseSqlOrder + " " +
           this->baseSqlLimit;
}

QString BasicListModel::countSqlStmt()
{
    return this->countSqlSelect + " " + this->baseSqlFrom + " " +
           this->baseSqlWhere;
}

QString BasicListModel::escaleSqlChars(QString unescapedString)
{
    QString escapedString = unescapedString;
    escapedString.replace(QString("'"), QString("''"));
    escapedString.replace(QString(" "), QString("%"));
    escapedString.replace(QString("."), QString("_"));
    return escapedString;
}

void BasicListModel::generateSqlLimit()
{
    this->baseSqlLimit =
        "LIMIT " + QString::number(this->NUMBER_OF_ITEMS_PER_PAGE) + " OFFSET " +
        QString::number(this->NUMBER_OF_ITEMS_PER_PAGE * this->currentPageNumber);
}

void BasicListModel::baseSearch()
{
    if (!this->isAutoComplete)
    {
        qDebug() << "BasicListModel::baseSearch() entered function";
        this->currentPageNumber = 0;
        this->generateSqlLimit();
        QString countStmt = this->countSqlStmt();
        qDebug() << "BasicListModel::baseSearch() quering db from another thread ...";
        this->itemCountFuture = QtConcurrent::run(this->dbManager, &DbManager::executeArbitrarySqlWithReturnValue, countStmt);
        this->itemCountFutureWatcher.setFuture(this->itemCountFuture);
        qDebug() << "BasicListModel::baseSearch() returns";
        //    this->itemCount =
        //        this->dbManager->executeArbitrarySqlWithReturnValue(countStmt);
        //    if (this->itemCount.size() > 0) {
        //      this->count = itemCount.at(0)["COUNT(*)"].toInt();
        //      qDebug() << "item count is: " << this->count;
        //    } else {
        //      this->count = 0;
        //    }
    }
    else
    {
        this->baseSearchItemCountReturns();
    };

    this->startWaitingForDbResponse();

    //  this->beginResetModel();
    //  this->clear();
    //  this->items =
    //      this->dbManager->executeArbitrarySqlWithReturnValue(this->sqlStmt());
    //  if (this->isAutoComplete) {
    //  }
    //  this->endResetModel();
}

void BasicListModel::baseSearchItemCountReturns()
{
    //  this->future.waitForFinished();
    qDebug() << this->MODEL_TYPE << " BasicListModel::baseSearchItemCountReturns() slot triggered";

    if (!this->isAutoComplete)
    {
        this->itemCount = this->itemCountFuture.result();

        if (this->itemCount.size() > 0)
        {
            this->count = itemCount.at(0)["COUNT(*)"].toInt();
            qDebug() << "item count is: " << this->count;
        }
        else
        {
            this->count = 0;
        }
    }

    qDebug() << "BasicListModel::baseSearchItemCountReturns() querying database for actual info in another thread: ";
    this->baseSearchFuture = QtConcurrent::run(this->dbManager, &DbManager::executeArbitrarySqlWithReturnValue, this->sqlStmt());
    this->baseSearchFutureWatcher.setFuture(this->baseSearchFuture);
    qDebug() << "BasicListModel::baseSearchItemCountReturns() returns";
}

void BasicListModel::baseSearchQueryReturns()

{
    qDebug() << "BasicListModel::baseSearchQueryReturns() slot triggered";
    this->beginResetModel();
    this->clear();
    this->items = this->baseSearchFuture.result();
    this->endResetModel();
    qDebug() << "BasicListModel::baseSearchQueryReturns() returns";
    emit baseSearchFinished();
    this->stopWaitingForDbResponse();
}

void BasicListModel::noLimitSearch()
{
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

void BasicListModel::setOrder(QString orderBy, QString orderDirection)
{
    QString orderByStmt = "AND (%1 IS NOT NULL AND %1 IS NOT 0) ORDER BY %1 %2";
    orderByStmt = orderByStmt.arg(orderBy, orderDirection);

    if (this->baseSqlOrder != orderByStmt)
    {
        this->baseSqlOrder = orderByStmt;
        this->beginResetModel();
        this->baseSearch();
        this->endResetModel();
    }
}

void BasicListModel::startWaitingForDbResponse()
{
    this->m_waitingForDbResponse = true;
    emit waitingForDbResponseChanged();
}

void BasicListModel::stopWaitingForDbResponse()
{
    this->m_waitingForDbResponse = false;
    emit waitingForDbResponseChanged();
}

void BasicListModel::clear()
{
    this->beginResetModel();

    if (this->items.size() > 0)
    {
        this->items.clear();
    }

    this->endResetModel();
}
