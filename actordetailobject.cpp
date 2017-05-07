#include "actordetailobject.h"
#include <QDebug>


ActorDetailObject::ActorDetailObject(DbManager *dbManager, QObject *parent)
                  : QObject(parent)
{
    this->dbManager = dbManager;

}

void ActorDetailObject::setActor(QString actorId)
{

    QString stmt = QString("SELECT *,"
                           "(SELECT COUNT(*) FROM Scene_Actor WHERE Scene_Actor.actor_id = Actor.id) as NumberOfScenes, "
                           "(SELECT COUNT(*) FROM Picture_Actor WHERE Picture_Actor.actor_id = Actor.id) as NumberOfPictures, "
                           "'Actor' as TableName "
                           "FROM Actor WHERE Actor.id = %1").arg(actorId);
    QList<QMap<QString, QVariant>> ans = this->dbManager->executeArbitrarySqlWithReturnValue(stmt);
    this->currentActor = ans.at(0);

}

QString ActorDetailObject::getActorAttrib(QString attribName)
{
    return currentActor[attribName].toString();
}

bool ActorDetailObject::setActorAttrib(QString attribName, QString attribValue)
{
//    UPDATE employees
//    SET city = 'Toronto',
//        state = 'ON',
//        postalcode = 'M5P 2N7'
//    WHERE
//        employeeid = 4;
    currentActor[attribName] = attribValue;
    QString stmt = QString("UPDATE %1 SET %2 = '%3' WHERE id = %4").arg(currentActor["TableName"].toString(),attribName,attribValue,currentActor["id"].toString());
    qDebug() << stmt;
    return this->dbManager->executeArbitrarySqlWithoutReturnValue(stmt);

}
