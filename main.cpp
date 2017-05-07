#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQuickView>
#include "qmlcomm.h"
#include "dbmanager.h"
#include "mediafoldermodel.h"
#include "actormodel.h"
#include <QDebug>
#include <QThread>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQuickView view;


    QmlComm* qmlComm = new QmlComm(view);

    view.rootContext()->setContextProperty("qmlComm", qmlComm);
    view.setSource(QUrl(QLatin1String("qrc:/main.qml")));






//    view.show();

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
