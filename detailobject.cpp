#include "detailobject.h"
#include <actormodel.h>
#include <QDebug>

DetailObject::DetailObject(DbManager *dbManager, QObject *parent)
    : QObject(parent) {
  this->dbManager = dbManager;
}

void DetailObject::setDetailObject(QString objectId, QString objectType) {
  QString stmt;
  if (objectType == "Actor") {
    stmt = QString(
                "SELECT * ,"
//                "(SELECT COUNT(*) FROM Scene_Actor WHERE Scene_Actor.actor_id = Actor.id) as NumberOfScenes, "
//                "(SELECT COUNT(*) FROM Picture_Actor WHERE Picture_Actor.actor_id = Actor.id) as NumberOfPictures, "
//                "(SELECT COUNT(*) FROM Actor_tag WHERE Actor_tag.actor_id = Actor.id) as NumberOfTags, "
                "'Actor' as TableName "
                "FROM Actor WHERE Actor.id = %1")
               .arg(objectId);
//    TODO: Make this use a variable instead of copy pasted String
  } else if (objectType == "Tag") {
      stmt = QString(
                  "SELECT * ,"
//                  "(SELECT COUNT(*) FROM Scene_Tag WHERE Scene_Tag.tag_id = Tag.id) as "
//                  "NumberOfScenes, "
//                  "(SELECT COUNT(*) FROM Actor_Tag WHERE Actor_Tag.tag_id = Tag.id) as "
//                  "NumberOfActors, "
//                  "(SELECT COUNT(*) FROM Picture_Tag WHERE Picture_Tag.tag_id = Tag.id) as "
//                  "NumberOfPictures, "
                  "'Tag' as TableName "
                  "FROM Tag WHERE Tag.id = %1"
                 ).arg(objectId);
    }else if (objectType == "Website"){
      stmt = QString(
                  "SELECT * ,"
//                  "(SELECT COUNT(*) FROM Scene_Website WHERE Scene_Website.website_id = Website.id) as NumberOfScenes, "
//                  "(SELECT COUNT(*) FROM Picture_Website WHERE Picture_Website.website_id = Website.id) as NumberOfPictures, "
//                  "(SELECT COUNT(*) FROM Website_Tag WHERE Website_Tag.website_id = Website.id) as NumberOfTags, "
                  "'Website' as TableName "
                  "FROM Website WHERE Website.id = %1"
                  ).arg(objectId);
  }

  QList<QMap<QString, QVariant>> ans =
      this->dbManager->executeArbitrarySqlWithReturnValue(stmt);
  this->currentDetailObject = ans.at(0);
}

QString DetailObject::getDetailObjectAttrib(QString attribName) {
  return currentDetailObject[attribName].toString();
}

bool DetailObject::setDetailObjectAttrib(QString attribName,
                                         QString attribValue) {
  //    UPDATE employees
  //    SET city = 'Toronto',
  //        state = 'ON',
  //        postalcode = 'M5P 2N7'
  //    WHERE
  //        employeeid = 4;
  currentDetailObject[attribName] = attribValue;
  QString stmt = QString("UPDATE %1 SET %2 = '%3' WHERE id = %4")
                     .arg(currentDetailObject["TableName"].toString(), attribName,
                          attribValue, currentDetailObject["id"].toString());
  qDebug() << stmt;
  return this->dbManager->executeArbitrarySqlWithoutReturnValue(stmt);
}
