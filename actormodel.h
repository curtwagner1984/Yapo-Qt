#ifndef ACTORMODEL_H
#define ACTORMODEL_H

#include <QObject>
#include "basiclistmodel.h"
#include "dbmanager.h"


class ActorModel : public BasicListModel
{
    Q_OBJECT
public:
    ActorModel(DbManager* dbManager);
    QVariant data(const QModelIndex &index, int role) const;
    void search(const QString searchString);
    QHash<int, QByteArray> roleNames() const;


    enum ActorRoles {
            IdRole = Qt::UserRole + 1,
            ThumbRole = Qt::UserRole + 2,
            ThumbRole64 = Qt::UserRole + 3,
            ThumbRole320 = Qt::UserRole + 4,
            NumberOfScenesRole = Qt::UserRole + 5,
            NumberOfPicturesRole = Qt::UserRole + 6,
            NameRole = Qt::UserRole + 7,
            GenderRole = Qt::UserRole + 8

        };
private:

};

#endif // ACTORMODEL_H
