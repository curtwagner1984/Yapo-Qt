#include "treefoldermodel.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include <QDebug>
#include "treefolderitem.h"
#include "treefoldermodel.h"

#include <QStringList>

TreeFolderModel::TreeFolderModel(QObject *parent)
    : QAbstractItemModel(parent)
{
  this->dbManager = dbManager;
  QMap<QString,QVariant> rootData;
  rootData["path_to_dir"] = -1;
  rootData["level"] = -1;

  rootItem = new TreeFolderItem(rootData);
  setupModelData(rootItem);
}

TreeFolderModel::~TreeFolderModel()
{
    delete rootItem;
}

QVariant TreeFolderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }else
    {
        TreeFolderItem *item = static_cast<TreeFolderItem*>(index.internalPointer());

        if (role == IdRole)
            return item->data("id");

        if (role == PathToDirRole)
            return item->data("path_to_dir");

//        if (role == Qt::DisplayRole)
//            return item->data("path_to_dir");

    }

}

Qt::ItemFlags TreeFolderModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant TreeFolderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        qDebug() << "TreeFolderModel::headerData section int is " << section;
    }
//        return rootItem->data(section);

    return QVariant();
}

QModelIndex TreeFolderModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeFolderItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeFolderItem*>(parent.internalPointer());

    TreeFolderItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();

}

QModelIndex TreeFolderModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeFolderItem *childItem = static_cast<TreeFolderItem*>(index.internalPointer());
    TreeFolderItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);

}

int TreeFolderModel::rowCount(const QModelIndex &parent) const
{
    TreeFolderItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeFolderItem*>(parent.internalPointer());

    return parentItem->childCount();

}

int TreeFolderModel::columnCount(const QModelIndex &parent) const
{
    qDebug() << "Called treeModel columncount";

    if (parent.isValid())
        return static_cast<TreeFolderItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QHash<int, QByteArray> TreeFolderModel::roleNames() const
{
    QHash<int, QByteArray> roles;
            roles[IdRole] = "id";
            roles[PathToDirRole] = "pathToDir";
            return roles;
}

void TreeFolderModel::setupModelData(TreeFolderItem *parent)
{
    QString stmt = "SELECT * FROM TreeFolder WHERE TreeFolder.level = 2";

    QList<QMap<QString,QVariant>> dbData = this->dbManager->executeArbitrarySqlWithReturnValue(stmt);

    for (int i = 0 ; i < dbData.count() ; i++)
    {
        TreeFolderItem* tempItem = new TreeFolderItem(dbData.at(i),parent);
        parent->appendChild(tempItem);
    }
}
