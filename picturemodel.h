#ifndef PICTUREMODEL_H
#define PICTUREMODEL_H

#include "basiclistmodel.h"
#include "dbmanager.h"

class PictureModel : public BasicListModel
{
    Q_OBJECT
public:
    PictureModel(DbManager* dbManager);
    QVariant data(const QModelIndex &index, int role) const;
    void search(const QString searchString);
    void getActorPictures(const QString actorId);
    QHash<int, QByteArray> roleNames() const;

    enum PictureRoles {
                IdRole = Qt::UserRole + 1,
                ThumbRole = Qt::UserRole + 2,
                ThumbRole64 = Qt::UserRole + 3,
                ThumbRole360 = Qt::UserRole + 4,
                PathRole = Qt::UserRole + 5,
                NameRole = Qt::UserRole + 6

            };
};

#endif // PICTUREMODEL_H
