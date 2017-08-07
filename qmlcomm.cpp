#include "qmlcomm.h"
#include "ffmpeghandler.h"
#include "tagger.h"
#include "tmdbscraper.h"
#include "scenepreviewmodel.h"

#include <QDir>
#include <QProcess>
#include <QQmlContext>
#include <QQuickView>

QmlComm::QmlComm(QQmlApplicationEngine& view, DbManager* dbManager,  QObject* parent) : QObject(parent)
{
    this->dbManager = dbManager;
    this->fileImporterMutex = new QMutex;
    this->settings = new QSettings("settings.ini", QSettings::IniFormat);
    this->fileImporter =
        new FileImporter(this->dbManager, this->fileImporterMutex, this->settings);
    this->scraper = new TmdbScraper(this->dbManager, this->settings);
    this->tagger = new Tagger(this->dbManager);
    //    models
    /*
    This part adds three global models (MediafolderModel,AutoCompleteModel) to the QML root context
    All other models are seperate instances and are instanciated inside the QML code.
    Maybe will need to do this with thoese models as well.

    */
    this->mediaFolderModel = new MediafolderModel();
    this->mediaFolderModel->init(this->dbManager);
    this->autoCompleteModel = new AutoCompleteModel();
    this->autoCompleteModel->init(this->dbManager);
    this->scenePreview = new ScenePreviewModel(this->settings);
    //  view.rootContext()->setContextProperty("dbManager", this->dbManager);
    view.rootContext()->setContextProperty("mediaFolderModel",
                                           this->mediaFolderModel);
    view.rootContext()->setContextProperty("autoCompleteModel",
                                           this->autoCompleteModel);
    view.rootContext()->setContextProperty("scenePreview",
                                           this->scenePreview);
    view.rootContext()->setContextProperty("fileImporter",
                                           this->fileImporter);
}

bool QmlComm::addMediaFolder(QString folderPath, bool isPicture, bool isVideo)
{
    return this->dbManager->addMediaFolder(folderPath, isPicture, isVideo);
}

bool QmlComm::addActors(QString actorList, bool isMainstream)
{
    QList<QStringList> actorsToAdd;
    QStringList csvSeperated = this->splitCsv(actorList);

    for (int i = 0; i < csvSeperated.size(); i++)
    {
        QStringList temp;
        temp.append(csvSeperated.at(i).trimmed());
        temp.append(isMainstream ? "1" : "0");
        actorsToAdd.append(temp);
    }

    return this->dbManager->addActors(actorsToAdd);
}

bool QmlComm::addTags(QString tagList)
{
    QList<QStringList> itemsToAdd;
    QStringList csvSeperated = this->splitCsv(tagList);

    for (int i = 0; i < csvSeperated.size(); i++)
    {
        QStringList temp;
        temp.append(csvSeperated.at(i).trimmed());
        itemsToAdd.append(temp);
    }

    return this->dbManager->addTags(itemsToAdd);
}

bool QmlComm::addWebsites(QString websiteList)
{
    QList<QStringList> itemsToAdd;
    QStringList csvSeperated = this->splitCsv(websiteList);

    for (int i = 0; i < csvSeperated.size(); i++)
    {
        QStringList temp;
        temp.append(csvSeperated.at(i).trimmed());
        itemsToAdd.append(temp);
    }

    return this->dbManager->addWebsites(itemsToAdd);
}

void QmlComm::scanMediaFolder(QString path, bool isVideo, bool isPicture)
{
    //    TODO: make it a signal that connects to a slot in fileImporter to avoid
    //    using mutex.
    //    this->fileImporterMutex->lock();
    this->fileImporter->addMediaFolderToQueue(path, isVideo, isPicture);
    //    this->fileImporterMutex->unlock();

    if (!this->fileImporter->isRunning())
    {
        this->fileImporter->start();
    }
}

void QmlComm::ffmpegTest()
{
    //  FfmpegHandler::ffprobeTest("");
}

void QmlComm::scrapeActors()
{
    this->scraper->setActorsToScrape(this->dbManager->getActorsBeforeScrape());
    this->scraper->startScrape();
}

void QmlComm::tagAll()
{
    this->tagger->beginTagging();
}

void QmlComm::actorSearch(QString searchTerm)
{
    this->actorModel->search(searchTerm);
}

void QmlComm::sceneSearch(QString searchTerm)
{
    this->sceneModel->search(searchTerm);
}

void QmlComm::pictureSearch(QString searchTerm)
{
    this->pictureModel->search(searchTerm);
}

void QmlComm::tagSearch(QString searchTerm)
{
    this->tagModel->search(searchTerm);
}

void QmlComm::websiteSearch(QString searchTerm)
{
    this->websiteModel->search(searchTerm);
}

void QmlComm::autoCompleteSearch(QString searchTerm, QString searchType)
{
    this->autoCompleteModel->search(searchTerm, searchType);
}

void QmlComm::aliasSearch(QString parentId, QString aliasType)
{
    this->generalAlias->search(parentId, aliasType);
}

void QmlComm::getActorScenes(QString actorId)
{
    this->sceneModel->getActorScenes(actorId);
}

void QmlComm::getActorPictures(QString actorId)
{
    this->pictureModel->getActorPictures(actorId);
}

void QmlComm::getTagsOfItem(QString itemId, QString tagType)
{
    if (tagType == "Actor")
    {
        this->tagModel->getActorTags(itemId);
    }
    else if (tagType == "Website")
    {
        this->tagModel->getWebsiteTags(itemId);
    }
}


void QmlComm::playScene(QString scenePath)
{
    QString scenePathNativeSep = QDir::toNativeSeparators(scenePath);
    QString VLC_PATH = "C:/Program Files (x86)/VideoLAN/VLC/vlc.exe";
    QStringList arguments;
    arguments << "--fullscreen" << scenePathNativeSep;
    QProcess* myProcess = new QProcess();
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    QString command = VLC_PATH + " " + scenePathNativeSep;
    qDebug() << "Starting playScene process... command: " + command;
    myProcess->start(VLC_PATH, arguments);
    //    myProcess->deleteLater();
}

//void QmlComm::prepareDetailView(QString detailObjectId,
//                                QString detailObjectType) {
//  if (detailObjectType == "Actor") {
//    this->getActorScenes(detailObjectId);
//    this->getActorPictures(detailObjectId);
//    this->tagModel->getActorTags(detailObjectId);
//    this->aliasSearch(detailObjectId, "Actor");
//    this->detailObject->setDetailObject(detailObjectId, detailObjectType);

//  } else if (detailObjectType == "Tag") {
//    this->actorModel->getTagActor(detailObjectId);
//    this->pictureModel->getTagPictures(detailObjectId);
//    this->sceneModel->getTagScenes(detailObjectId);
//    this->websiteModel->getTagWebsites(detailObjectId);
//    this->generalAlias->search(detailObjectId, detailObjectType);
//    this->detailObject->setDetailObject(detailObjectId, detailObjectType);

//  } else if (detailObjectType == "Website") {

//    this->pictureModel->getWebsitePictures (detailObjectId);
//    this->sceneModel->getWebsiteScenes(detailObjectId);

//    this->tagModel->getWebsiteTags(detailObjectId);

//    this->generalAlias->search(detailObjectId, detailObjectType);
//    this->detailObject->setDetailObject(detailObjectId, detailObjectType);
//  }
//}

void QmlComm::addAlias(QString aliasName, QString aliasType,
                       QString aliasOfId)
{
    QString tableName;
    QString columnName;

    if (aliasType == "Actor")
    {
        tableName = "ActorAlias";
        columnName = "actor_id";
    }
    else if (aliasType == "Tag")
    {
        tableName = "TagAlias";
        columnName = "tag_id";
    }
    else if (aliasType == "Website")
    {
        tableName = "WebsiteAlias";
        columnName = "website_id";
    }

    QString stmt = "INSERT OR IGNORE INTO %1 (name,%2) VALUES ('%3','%4')";
    this->dbManager->executeArbitrarySqlWithoutReturnValue(
        stmt.arg(tableName, columnName, aliasName, aliasOfId));
}

void QmlComm::addTag(QString tagId, QString tagName, QString tagType,
                     QString tagOfId)
{
    this->dbManager->addTagWithRelation(tagId, tagName, tagType, tagOfId);
}

bool QmlComm::deleteAlias(QString aliasToDeleteId, QString aliasOf)
{
    return this->generalAlias->deleteAlias(aliasToDeleteId, aliasOf);
}

void QmlComm::setSetting(QString key, QVariant value)
{
    settings->setValue(key, value);
}

QVariant QmlComm::getSetting(QString key)
{
    return settings->value(key);
}

void QmlComm::init()
{
    this->dbManager->connectToDatabase(settings->value("general/dbpath").toString());
}

QStringList QmlComm::splitCsv(QString csvString)
{
    QStringList splitList = csvString.split(",", QString::SkipEmptyParts);
    return splitList;
}

QmlComm::~QmlComm()
{
    this->fileImporter->addMediaFolderToQueue("poison", 1, 1);
    delete this->scraper;
    delete this->tagger;
    delete this->fileImporterMutex;
    delete this->fileImporter;
    delete this->settings;
}
