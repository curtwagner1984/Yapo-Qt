#ifndef AUTOCOMPLETEMODEL_H
#define AUTOCOMPLETEMODEL_H

#include <QObject>
#include "basiclistmodel.h"
#include "dbmanager.h"


class AutoCompleteModel: public BasicListModel
{
    Q_OBJECT
public:
    AutoCompleteModel(DbManager* dbManager);
    QVariant data(const QModelIndex &index, int role) const;
    void search(const QString searchString);
    QHash<int, QByteArray> roleNames() const;
private:
    QString currentSearchString = "";


    enum ActorRoles {
            IdRole = Qt::UserRole + 1,
            ThumbRole = Qt::UserRole + 2,
            ThumbRole64 = Qt::UserRole + 3,
            ThumbRole320 = Qt::UserRole + 4,
            NameRole = Qt::UserRole + 5,
            TableNameRole = Qt::UserRole + 6,
            AliasOfRole = Qt::UserRole + 7,
            AliasOfIdRole = Qt::UserRole + 8,


        };
private:

};

#endif // AUTOCOMPLETEMODEL_H
