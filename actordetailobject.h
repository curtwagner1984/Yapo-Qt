#ifndef ACTORDETAILOBJECT_H
#define ACTORDETAILOBJECT_H

#include <QObject>
#include <QMap>
#include "dbmanager.h"

class ActorDetailObject : public QObject
{
    Q_OBJECT
public:
    explicit ActorDetailObject(DbManager* dbManager, QObject *parent = 0);
    Q_INVOKABLE void setActor (QString actorId);
    Q_INVOKABLE QString getActorAttrib(QString attribName);
    Q_INVOKABLE bool setActorAttrib(QString attribName, QString attribValue);

private:
    DbManager* dbManager;
    QMap<QString,QVariant> currentActor;

signals:

public slots:
};

#endif // ACTORDETAILOBJECT_H
