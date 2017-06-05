#ifndef TREEFOLDERMODEL_H
#define TREEFOLDERMODEL_H

// class TreeItem;

////! [0]
// class TreeModel : public QAbstractItemModel
//{
//    Q_OBJECT

// public:
//    explicit TreeModel(const QString &data, QObject *parent = 0);
//    ~TreeModel();

//    QVariant data(const QModelIndex &index, int role) const override;
//    Qt::ItemFlags flags(const QModelIndex &index) const override;
//    QVariant headerData(int section, Qt::Orientation orientation,
//                        int role = Qt::DisplayRole) const override;
//    QModelIndex index(int row, int column,
//                      const QModelIndex &parent = QModelIndex()) const
//                      override;
//    QModelIndex parent(const QModelIndex &index) const override;
//    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
//    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

// private:
//    void setupModelData(const QStringList &lines, TreeItem *parent);

//    TreeItem *rootItem;
//};

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "dbmanager.h"

class TreeFolderItem;

class TreeFolderModel : public QAbstractItemModel {

  Q_OBJECT

 public:

  explicit TreeFolderModel(QObject *parent = 0);
  ~TreeFolderModel();

  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QHash<int, QByteArray> roleNames() const;


  enum TreeFolderRoles {
          IdRole = Qt::UserRole + 1,
          PathToDirRole = Qt::UserRole + 2,

      };

 private:
  void setupModelData(TreeFolderItem *parent);

  DbManager* dbManager;
  TreeFolderItem *rootItem;
};

#endif  // TREEFOLDERMODEL_H
