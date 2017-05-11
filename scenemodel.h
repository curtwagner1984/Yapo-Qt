#ifndef SCENEMODEL_H
#define SCENEMODEL_H

#include "basiclistmodel.h"
#include "dbmanager.h"

// class ActorModel : public BasicListModel
//{
//    Q_OBJECT
// public:
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

class SceneModel : public BasicListModel {
  Q_OBJECT
 public:
  SceneModel(DbManager* dbManager);
  QVariant data(const QModelIndex& index, int role) const;
  void search(const QString searchString);
  void getActorScenes(const QString actorId);
  void getTagScenes(const QString tagId);
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

 private:
  QString SEARCH_SELECT = "SELECT * ";

  QString SEARCH_FROM = "FROM Scene";
  QString SEARCH_WHERE = "WHERE Scene.path_to_file LIKE '%%1%'";
  QString SEARCH_ORDER = "ORDER BY Scene.id DESC";

  QString ACTOR_SEARCH_SELECT = "SELECT * ";
  QString ACTOR_SEARCH_FROM =
      "FROM (SELECT * FROM Scene) AS T1 "
      "JOIN (SELECT * FROM Scene_Actor WHERE Scene_Actor.actor_id = %1) AS T2 "
      "ON T1.id = T2.scene_id";

  QString TAG_SEARCH_SELECT = "SELECT * ";
  QString TAG_SEARCH_FROM =
      "FROM ( "
      "SELECT T1.* "
      "FROM (SELECT * FROM Scene) AS T1 "
      "JOIN (SELECT * FROM Scene_Tag WHERE Scene_Tag.tag_id = %1) AS T2 "
      "ON T1.id = T2.scene_id "
      "UNION "
      "SELECT Scene.* FROM Scene "
      "JOIN Scene_Actor ON Scene.id = Scene_Actor.scene_id "
      "JOIN Actor ON Actor.id = Scene_Actor.actor_id "
      "JOIN Actor_Tag ON Actor.id = Actor_Tag.actor_id "
      "JOIN Tag ON Tag.id = Actor_Tag.tag_id "
      "WHERE Tag.id = %1 "
      "UNION "
      "SELECT Scene.* FROM Scene "
      "JOIN Scene_Website ON Scene.id = Scene_Website.scene_id "
      "JOIN Website ON Website.id = Scene_Website.website_id "
      "JOIN Website_Tag ON Website.id = Website_Tag.website_id "
      "JOIN Tag ON Tag.id = Website_Tag.tag_id "
      "WHERE Tag.id = %1 "
      ") ";
};

#endif  // SCENEMODEL_H
