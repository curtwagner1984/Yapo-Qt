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
    bool setData(const QModelIndex &index, const QVariant &value, int role);


    Q_INVOKABLE void search(const QString aliasOfId, const QString aliasOf);

    bool deleteAlias(const QString aliasToDeleteId, const QString aliasOf);

    void updateAlias(QString aliasOf, QString updatedField, QString updatedValue);


    QHash<int, QByteArray> roleNames() const;


    enum AliasRoles {
            IdRole = Qt::UserRole + 1,
            NameRole = Qt::UserRole + 2,
            ExemptFromOneWordRole = Qt::UserRole + 3,
            TableNameRole = Qt::UserRole + 4

        };
private:

    QString SEARCH_SELECT = "SELECT * ,"
                            "'%1' as TableName ";

    QString SEARCH_FROM = "FROM %1";

    QString SEARCH_WHERE = "WHERE %1.%2 = %3";

    QString tableName;
    QString columnName;

};

#endif // GENERALALIAS_H
