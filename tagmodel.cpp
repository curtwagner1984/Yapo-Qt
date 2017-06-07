#include "tagmodel.h"
#include <QDebug>
#include <QFileInfo>

TagModel::TagModel() : BasicListModel() {
  qDebug() << "Making test tag search ...";

  this->MODEL_TYPE = "TagModel";

  this->baseSqlSelect = SEARCH_SELECT;
  this->baseSqlFrom = SEARCH_FROM;
  this->generateSqlLimit();
  qDebug() << "Tag Model initialized ...";
  //  this->search("");
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
  } else if (role == NumberOfWebsitesRole) {
      return currentItem["NumberOfWebsites"];
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

void TagModel::searchById(const QString tagId)
{
    QString escapedSearchString = this->escaleSqlChars(tagId);
    this->baseSqlWhere = SEARCH_BY_ID_WHERE.arg(escapedSearchString);
    this->baseSqlSelect = SEARCH_SELECT;
    this->baseSqlFrom = SEARCH_FROM;

    //  Resets count and gets number of items and executes search
    this->baseSearch();
}

void TagModel::getActorTags(const QString actorId) {
  this->baseSqlSelect = SEARCH_SELECT;
  this->baseSqlWhere = ACTOR_SEARCH_WHERE.arg(actorId);
  this->baseSqlFrom = ACTOR_SEARCH_FROM;
  this->baseSearch();
}

void TagModel::getActorTagsForTagger(const QString actorId) {
  this->baseSqlSelect = ACTOR_SEARCH_SELECT;
  this->baseSqlWhere = ACTOR_SEARCH_WHERE.arg(actorId);
  this->baseSqlFrom = ACTOR_SEARCH_FROM;
  this->baseSqlOrder = ACTOR_ORDER_BY;
    this->baseSearch();
}

void TagModel::getSceneTagsForTagger(const QString sceneId)
{
    this->baseSqlSelect = SEARCH_SELECT;
    this->baseSqlWhere = SCENE_SEARCH_WHERE.arg(sceneId);
    this->baseSqlFrom = SCENE_SEARCH_FROM;
    this->baseSqlOrder = SCENE_ORDER_BY;
    this->baseSearch();

}

//bool TagModel::addTag(QString tagToAddId, QString tagToAddName,
//                      QString tagOfType, QString tagOfId) {
//  bool ans = false;
//  if (tagToAddId == "") {
//    QString createStmt = "INSERT INTO Tag (name) VALUES ('%1')";
//    createStmt = createStmt.arg(tagToAddName);
//    if (this->dbManager->executeArbitrarySqlWithoutReturnValue(createStmt)) {
//      QString lastInsertIdStmt = "SELECT LAST_INSERT_ROWID() as lastId";
//      QList<QMap<QString, QVariant>> b =
//          this->dbManager->executeArbitrarySqlWithReturnValue(lastInsertIdStmt);
//      tagToAddId = b.at(0)["lastId"].toString();
//    }
//  }

//  QString tableName = QString("%1_Tag").arg(tagOfType);
//  QString firstColumnName = QString("%1_id").arg(tagOfType).toLower();

//  QString insetRelationStmt = "INSERT INTO %1 (%2,%3) VALUES (%4,%5)";
//  insetRelationStmt = insetRelationStmt.arg(tableName, firstColumnName, "tag_id",
//                                            tagOfId, tagToAddId);
//  if (this->dbManager->executeArbitrarySqlWithoutReturnValue(
//          insetRelationStmt)) {
//    QString selectStmt = "SELECT * FROM Tag WHERE Tag.id = %1";
//    selectStmt = selectStmt.arg(tagToAddId);
//    QList<QMap<QString, QVariant>> tagToAdd =
//        this->dbManager->executeArbitrarySqlWithReturnValue(selectStmt);

//    QModelIndex x = QModelIndex();
//    this->beginInsertRows(x, this->items.size(), this->items.size());
//    this->items.append(tagToAdd.at(0));
//    this->endInsertRows();
//    ans = true;
//  }

//  return ans;
//}

//bool TagModel::removeTag(QString tagToRemoveId, QString tagOfType, QString tagOfId, bool removeFromDb)
//{
//    QString tableName = QString("%1_Tag").arg(tagOfType);
//    QString firstColumnName = QString("%1_id").arg(tagOfType).toLower();

//    QString removeFromDbStmt = QString("DELETE FROM Tag WHERE id = %2").arg(tagToRemoveId);

//    if (removeFromDb){
//        return this->dbManager->executeArbitrarySqlWithoutReturnValue(removeFromDbStmt);
//    }else{
//        QString removeRelationStmt = QString("DELETE FROM %1 WHERE %2=%3 AND tag_id=%4").arg(tableName,firstColumnName,tagOfId,tagToRemoveId);
//        if (this->dbManager->executeArbitrarySqlWithoutReturnValue(removeRelationStmt)){

//            int removeIndex = -1;
//            for (int i = 0 ; i < this->items.size() ; i++)
//            {
//                QMap<QString,QVariant> temp = this->items.at(i);
//                if (temp["id"].toString() == tagToRemoveId ){
//                    removeIndex = i;
//                    break;
//                }
//            }

//            QModelIndex x = QModelIndex();
//            this->beginRemoveRows(x,removeIndex,removeIndex);
//            this->items.removeAt(removeIndex);
//            this->endRemoveRows();
//            return true;

//        }else{
//            return false;
//        }


//    }
//}

void TagModel::getWebsiteTags(const QString websiteId) {
  this->baseSqlSelect = SEARCH_SELECT;
  this->baseSqlWhere = WEBSITE_SEARCH_WHERE.arg(websiteId);
  this->baseSqlFrom = this->WEBSITE_SEARCH_FROM;
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
  roles[NumberOfWebsitesRole] = "numberOfWebsites";
  roles[NameRole] = "name";

  return roles;
}
