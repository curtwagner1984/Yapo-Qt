#ifndef GENERALALIAS_H
#define GENERALALIAS_H


#include <QObject>
#include "basiclistmodel.h"
#include "dbmanager.h"

class GeneralAlias : public BasicListModel
{
    Q_OBJECT
public:
    GeneralAlias(DbManager* dbManager);
    QVariant data(const QModelIndex &index, int role) const;
    void search(const QString searchString, const QString aliasOf);
    QHash<int, QByteArray> roleNames() const;


    enum AliasRoles {
            IdRole = Qt::UserRole + 1,
            NameRole = Qt::UserRole + 2,
            ExemptFromOneWordRole = Qt::UserRole + 3

        };
private:

};

#endif // GENERALALIAS_H
