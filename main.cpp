#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <QQuickView>
#include "qmlcomm.h"
#include "mediafoldermodel.h"

#include <QDebug>
#include <QThread>
#include <QQmlContext>


#include "dbmanager.h"

#include "actormodel.h"
#include "picturemodel.h"
#include "scenemodel.h"
#include "tagmodel.h"
#include "websitemodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);





//    QQuickView view;


//    QmlComm* qmlComm = new QmlComm(view);

//    view.rootContext()->setContextProperty("qmlComm", qmlComm);
//    view.setSource(QUrl(QLatin1String("qrc:/main.qml")));






//    view.show();

    qmlRegisterType<ActorModel>("com.me.qmlcomponents", 1, 0, "ActorModel");
    qmlRegisterType<TagModel>("com.me.qmlcomponents", 1, 0, "TagModel");
    qmlRegisterType<WebsiteModel>("com.me.qmlcomponents", 1, 0, "WebsiteModel");

    qmlRegisterType<SceneModel>("com.me.qmlcomponents", 1, 0, "SceneModel");
    qmlRegisterType<PictureModel>("com.me.qmlcomponents", 1, 0, "PictureModel");


    DbManager* dbManager = new DbManager("");

    QQmlApplicationEngine engine;



    QmlComm* qmlComm = new QmlComm(engine,dbManager);




    engine.rootContext()->setContextProperty("dbManager", dbManager);
    engine.rootContext()->setContextProperty("qmlComm", qmlComm);



    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
