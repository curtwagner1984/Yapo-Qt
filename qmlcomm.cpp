#include "qmlcomm.h"
#include "ffmpeghandler.h"
#include "tmdbscraper.h"
#include "tagger.h"

#include <QQuickView>
#include <QQmlContext>
#include <QProcess>
#include <QDir>

QmlComm::QmlComm(QQuickView &view, QObject *parent) : QObject(parent)
{
    this->dbManager = new DbManager("");
    this->fileImporterMutex = new QMutex;
    this->fileImporter = new FileImporter(this->dbManager,this->fileImporterMutex);
    this->scraper = new TmdbScraper(this->dbManager);
    this->tagger = new Tagger(this->dbManager);

//    models
    this->mediaFolderModel = new MediafolderModel(this->dbManager);
    this->actorModel = new ActorModel(this->dbManager);
    this->sceneModel = new SceneModel(this->dbManager);
    this->pictureModel = new PictureModel(this->dbManager);
    this->tagModel = new TagModel(this->dbManager);
    this->websiteModel = new WebsiteModel(this->dbManager);

    this->generalAlias = new GeneralAlias(this->dbManager);
    this->autoCompleteModel = new AutoCompleteModel(this->dbManager);

    this->actorDetailObject = new ActorDetailObject(this->dbManager);



     view.rootContext()->setContextProperty("dbManager", this->dbManager);
     view.rootContext()->setContextProperty("mediaFolderModel", this->mediaFolderModel);
     view.rootContext()->setContextProperty("actorModel", this->actorModel);
     view.rootContext()->setContextProperty("sceneModel", this->sceneModel);
     view.rootContext()->setContextProperty("pictureModel", this->pictureModel);
     view.rootContext()->setContextProperty("tagModel", this->tagModel);
     view.rootContext()->setContextProperty("websiteModel", this->websiteModel);

     view.rootContext()->setContextProperty("generalAlias", this->generalAlias);

     view.rootContext()->setContextProperty("actorDetailObject", this->actorDetailObject);
     view.rootContext()->setContextProperty("autoCompleteModel", this->autoCompleteModel);


}

bool QmlComm::addMediaFolder(QString folderPath, bool isPicture, bool isVideo)
{
    return this->dbManager->addMediaFolder(folderPath,isPicture,isVideo);

}

bool QmlComm::addActors(QString actorList, bool isMainstream)
{
    QList<QStringList> actorsToAdd;
    QStringList csvSeperated = this->splitCsv(actorList);

    for (int i = 0 ; i < csvSeperated.size() ; i++){
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

    for (int i = 0 ; i < csvSeperated.size() ; i++){
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

    for (int i = 0 ; i < csvSeperated.size() ; i++){
        QStringList temp;
        temp.append(csvSeperated.at(i).trimmed());
        itemsToAdd.append(temp);
    }

    return this->dbManager->addWebsites(itemsToAdd);

}

void QmlComm::scanMediaFolder(QString path, bool isVideo, bool isPicture)
{
//    TODO: make it a signal that connects to a slot in fileImporter to avoid using mutex.
    //    this->fileImporterMutex->lock();
    this->fileImporter->addMediaFolderToQueue(path,isVideo,isPicture);
//    this->fileImporterMutex->unlock();

    if (!this->fileImporter->isRunning()){
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
    this->autoCompleteModel->search(searchTerm,searchType);
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

void QmlComm::getActorTags(QString actorId)
{
    this->tagModel->getActorTags(actorId);
}

void QmlComm::playScene(QString scenePath)
{
    QString scenePathNativeSep = QDir::toNativeSeparators( scenePath );
    QString VLC_PATH = "C:/Program Files (x86)/VideoLAN/VLC/vlc.exe";
    QStringList arguments;
    arguments << "--fullscreen" << scenePathNativeSep;
    QProcess *myProcess = new QProcess();
    myProcess->setProcessChannelMode(QProcess::MergedChannels);

    QString command = VLC_PATH + " " + scenePathNativeSep;
    qDebug() << "Starting playScene process... command: " + command;
    myProcess->start(VLC_PATH, arguments);
    //    myProcess->deleteLater();
}

void QmlComm::prepareActorDetailView(QString actorId)
{
    this->getActorScenes(actorId);
    this->getActorPictures(actorId);
    this->getActorTags(actorId);
    this->aliasSearch(actorId,"Actor");
    this->actorDetailObject->setActor(actorId);



}

void QmlComm::addAlias(QString aliasName, QString aliasType, QString aliasOfId)
{
    if (aliasType == "Actor"){
        QList< QStringList > tempList;
        QStringList temp;

        temp.append(aliasName);
        temp.append(aliasOfId);
        tempList.append(temp);

        this->dbManager->addActorsAliases(tempList);
    }

}

void QmlComm::addTag(QString tagId, QString tagName, QString tagType, QString tagOfId)
{
    if (tagType == "Actor")
    {
        this->dbManager->addActorTag(tagId,tagName,tagOfId);

    }

}

QStringList QmlComm::splitCsv(QString csvString)
{
    QStringList splitList = csvString.split(",", QString::SkipEmptyParts);
    return splitList;
}

QmlComm::~QmlComm()
{
    this->fileImporter->addMediaFolderToQueue("poison",1,1);
    delete this->scraper;
    delete this->tagger;
    delete this->fileImporterMutex;
    delete this->fileImporter;

}
