#ifndef AUTOCOMPLETEMODEL_H
#define AUTOCOMPLETEMODEL_H

#include <QObject>
#include "basiclistmodel.h"
#include "dbmanager.h"

class AutoCompleteModel : public BasicListModel {
  Q_OBJECT
 public:
  AutoCompleteModel(DbManager* dbManager);
  QVariant data(const QModelIndex& index, int role) const;
  Q_INVOKABLE void search(const QString searchString, QString searchType);
  void tagSearch(const QString searchString);
  void generalSearch(const QString searchString);
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
  QString SEARCH_SELECT = "SELECT *";

  QString SEARCH_FROM =
      "FROM ( "
      "SELECT Actor.id as id, Actor.name as name, 'Actor' as TableName, 'na' "
      "as alias_of, 'na' as alias_of_id, Actor.thumbnail as thumbnail FROM "
      "Actor "
      "UNION ALL "
      "SELECT Tag.id as id, Tag.name as name, 'Tag' as TableName, 'na' as "
      "alias_of,'na' as alias_of_id, Tag.thumbnail as thumbnail FROM Tag "
      "UNION ALL "
      "SELECT Website.id as id, Website.name as name , 'Website' as TableName, "
      "'na' as alias_of,'na' as alias_of_id, Website.thumbnail as thumbnail "
      "FROM Website "
      "UNION ALL "
      "SELECT ActorAlias.id as id, ActorAlias.name as name, 'ActorAlias' as "
      "TableName, Actor.name as alias_of, Actor.id as alias_of_id, "
      "Actor.thumbnail as thumbnail FROM ActorAlias "
      "JOIN Actor on Actor.id = ActorAlias.actor_id)";

  QString SEARCH_LIMIT = "LIMIT 15";

  QString SEARCH_WHERE = "WHERE name LIKE '%%1%'";
  QString SEARCH_ORDER = "ORDER BY name";

  QString TAG_SEARCH_FROM =
      "FROM ( "
      "SELECT Tag.id as id, Tag.name as name, 'Tag' as TableName, 'na' as "
      "alias_of,'na' as alias_of_id, Tag.thumbnail as thumbnail FROM Tag "
      ")";
};

#endif  // AUTOCOMPLETEMODEL_H
