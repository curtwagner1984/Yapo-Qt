#ifndef TAGMODEL_H
#define TAGMODEL_H

#include <QObject>
#include "basiclistmodel.h"
#include "dbmanager.h"


class TagModel : public BasicListModel
{
    Q_OBJECT
public:
    TagModel(DbManager* dbManager);
    QVariant data(const QModelIndex &index, int role) const;
    void search(const QString searchString);
    void getActorTags(const QString actorId);
    QHash<int, QByteArray> roleNames() const;


    enum TagRoles {
            IdRole = Qt::UserRole + 1,
            ThumbRole = Qt::UserRole + 2,
            ThumbRole64 = Qt::UserRole + 3,
            ThumbRole320 = Qt::UserRole + 4,
            NameRole = Qt::UserRole + 5,
            NumberOfScenesRole = Qt::UserRole + 6,
            NumberOfPicturesRole = Qt::UserRole + 7,
            NumberOfActorsRole = Qt::UserRole + 8


        };
private:

};

#endif  // TAGMODEL_H
