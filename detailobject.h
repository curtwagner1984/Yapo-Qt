#ifndef ACTORDETAILOBJECT_H
#define ACTORDETAILOBJECT_H

#include <QObject>
#include <QMap>
#include "dbmanager.h"

class DetailObject : public QObject
{
    Q_OBJECT
public:
    explicit DetailObject(DbManager* dbManager, QObject *parent = 0);
    Q_INVOKABLE void setDetailObject (QString objectId, QString objectType);
    Q_INVOKABLE QString getDetailObjectAttrib(QString attribName);
    Q_INVOKABLE bool setDetailObjectAttrib(QString attribName, QString attribValue);

private:
    DbManager* dbManager;
    QMap<QString,QVariant> currentDetailObject;

signals:

public slots:
};

#endif // ACTORDETAILOBJECT_H
