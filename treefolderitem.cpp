#include <QStringList>
#include "treefolderitem.h"

//! [0]
TreeFolderItem::TreeFolderItem(const QMap<QString,QVariant> &data, TreeFolderItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}
//! [0]

//! [1]
TreeFolderItem::~TreeFolderItem()
{
    qDeleteAll(m_childItems);
}
//! [1]

//! [2]
void TreeFolderItem::appendChild(TreeFolderItem *item)
{
    m_childItems.append(item);
}
//! [2]

//! [3]
TreeFolderItem *TreeFolderItem::child(int row)
{
    return m_childItems.value(row);
}
//! [3]

//! [4]
int TreeFolderItem::childCount() const
{
    return m_childItems.count();
}
//! [4]

//! [5]
int TreeFolderItem::columnCount() const
{
    return m_itemData.count();
}
//! [5]

//! [6]
QVariant TreeFolderItem::data(QString roleName) const
{
    return m_itemData[roleName];
}
//! [6]

//! [7]
TreeFolderItem *TreeFolderItem::parentItem()
{
    return m_parentItem;
}
//! [7]

//! [8]
int TreeFolderItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeFolderItem*>(this));

    return 0;
}
//! [8]
