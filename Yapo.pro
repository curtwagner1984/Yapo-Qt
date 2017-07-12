QT += qml quick sql script network

CONFIG += c++11

SOURCES += main.cpp \
    qmlcomm.cpp \
    dbmanager.cpp \
    mediafoldermodel.cpp \
    fileimporter.cpp \
    basiclistmodel.cpp \
    ffmpeghandler.cpp \
    tmdbscraper.cpp \
    tagger.cpp \
    actormodel.cpp \
    scenemodel.cpp \
    picturemodel.cpp \
    tagmodel.cpp \
    websitemodel.cpp \
    generalalias.cpp \
    autocompletemodel.cpp \
    detailobject.cpp \
    treefolderitem.cpp \
    treefoldermodel.cpp \
    constants.cpp

RESOURCES += qml.qrc \
    images.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    qmlcomm.h \
    dbmanager.h \
    mediafoldermodel.h \
    db_models.h \
    fileimporter.h \
    basiclistmodel.h \
    ffmpeghandler.h \
    tmdbscraper.h \
    tagger.h \
    actormodel.h \
    scenemodel.h \
    picturemodel.h \
    tagmodel.h \
    websitemodel.h \
    generalalias.h \
    autocompletemodel.h \
    detailobject.h \
    treefolderitem.h \
    treefoldermodel.h \
    constants.h

DISTFILES +=
