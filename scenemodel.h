#ifndef SCENEMODEL_H
#define SCENEMODEL_H

#include "basiclistmodel.h"
#include "dbmanager.h"

//class ActorModel : public BasicListModel
//{
//    Q_OBJECT
//public:
//    ActorModel(DbManager* dbManager);
//    QVariant data(const QModelIndex &index, int role) const;
//    void search(const QString searchString);
//    QHash<int, QByteArray> roleNames() const;

//    enum ActorRoles {
//            IdRole = Qt::UserRole + 1,
//            ThumbRole = Qt::UserRole + 2,
//            ThumbRole64 = Qt::UserRole + 3,
//            NumberOfScenesRole = Qt::UserRole + 4,
//            NumberOfPicturesRole = Qt::UserRole + 5,
//            NameRole = Qt::UserRole + 6

//        };
//};

class SceneModel : public BasicListModel
{
    Q_OBJECT
public:
    SceneModel(DbManager* dbManager);
    QVariant data(const QModelIndex &index, int role) const;
    void search(const QString searchString);
    void getActorScenes(const QString actorId);
    QHash<int, QByteArray> roleNames() const;


    enum SceneRoles {
                IdRole = Qt::UserRole + 1,
                ThumbRole = Qt::UserRole + 2,
                ThumbRole64 = Qt::UserRole + 3,
                ThumbRole360 = Qt::UserRole + 4,
                PathRole = Qt::UserRole + 5,
                NameRole = Qt::UserRole + 6,
                DurationRole = Qt::UserRole + 7

            };
};

#endif // SCENEMODEL_H
