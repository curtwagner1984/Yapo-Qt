#ifndef TREEFOLDERITEM_H
#define TREEFOLDERITEM_H

#include <QList>
#include <QVariant>

class TreeFolderItem
{
public:
    explicit TreeFolderItem(const QMap<QString,QVariant> &data, TreeFolderItem *parentItem = 0);
    ~TreeFolderItem();

    void appendChild(TreeFolderItem *child);

    TreeFolderItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(QString roleName) const;
    int row() const;
    TreeFolderItem *parentItem();

private:
    QList<TreeFolderItem*> m_childItems;
    QMap<QString,QVariant> m_itemData;
    TreeFolderItem *m_parentItem;
};

#endif // TREEFOLDERITEM_H
