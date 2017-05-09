#include "tagmodel.h"
#include <QDebug>
#include <QFileInfo>








TagModel::TagModel(DbManager *dbManager) : BasicListModel(dbManager) {
  qDebug() << "Making test tag search ...";

  this->baseSqlSelect = SEARCH_SELECT;
  this->baseSqlFrom = SEARCH_FROM;
  this->generateSqlLimit();
  this->search("");
}

QVariant TagModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }

  QMap<QString, QVariant> currentItem = this->items.at(index.row());

  if (role == IdRole) {
    return currentItem["id"];
  } else if (role == ThumbRole) {
    return currentItem["thumbnail"];
  } else if (role == ThumbRole64) {
    QString currentThumbPath = currentItem["thumbnail"].toString();
    QFileInfo fInfo = QFileInfo(currentThumbPath);
    QString path = fInfo.path();
    path = path + "/profile_64.jpg";
    return path;
  } else if (role == ThumbRole320) {
    QString currentThumbPath = currentItem["thumbnail"].toString();
    QFileInfo fInfo = QFileInfo(currentThumbPath);
    QString path = fInfo.path();
    path = path + "/profile_320.jpg";
    return path;
  } else if (role == NumberOfScenesRole) {
    return currentItem["NumberOfScenes"];
  } else if (role == NumberOfPicturesRole) {
    return currentItem["NumberOfPictures"];
  } else if (role == NumberOfActorsRole) {
    return currentItem["NumberOfActors"];
  } else if (role == NameRole) {
    return currentItem["name"];
  } else {
    return QVariant();
  }
}

void TagModel::search(const QString searchString) {
  QString escapedSearchString = this->escaleSqlChars(searchString);
  this->baseSqlWhere = SEARCH_WHERE.arg(escapedSearchString);
  this->baseSqlSelect = SEARCH_SELECT;
  this->baseSqlFrom = SEARCH_FROM;


  //  Resets count and gets number of items and executes search
  this->baseSearch();
}

void TagModel::getActorTags(const QString actorId) {

  this->baseSqlSelect ="SELECT * ";
  this->baseSqlWhere = "";
  this->baseSqlOrder = "";
  this->baseSqlFrom = ACTOR_SEARCH_FROM.arg(SEARCH_SELECT,actorId);
  this->baseSearch();
}

QHash<int, QByteArray> TagModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[IdRole] = "id";
  roles[ThumbRole] = "thumb";
  roles[ThumbRole64] = "thumb_64";
  roles[ThumbRole320] = "thumb_320";
  roles[NumberOfScenesRole] = "numberOfScenes";
  roles[NumberOfPicturesRole] = "numberOfPictures";
  roles[NumberOfActorsRole] = "numberOfActors";
  roles[NameRole] = "name";
  return roles;
}
