#include "tagger.h"

#include "qdebug.h"

#include <QElapsedTimer>

Tagger::Tagger(DbManager* dbManager, QObject *parent) : QObject(parent)
{
    this->dbManager = dbManager;
}

void Tagger::beginTagging()
{
    QList<QMap<QString, QVariant>> allScenes = this->dbManager->getAllScenes();
    QList<QMap<QString, QVariant>> allPictures = this->dbManager->getAllPictures();            ;
    QList<QMap<QString, QVariant>> taggingElemets = this->dbManager->getTaggingElements();

    QList<QMap<QString, QVariant>> preparedTaggingElemets;


    QList<QStringList> sceneActors;
    QList<QStringList> sceneTags;
    QList<QStringList> sceneWebsites;

    QList<QStringList> pictureActors;
    QList<QStringList> pictureTags;
    QList<QStringList> pictureWebsites;

    QElapsedTimer timer;
    QElapsedTimer timerLoop;


    for (int i = 0 ; i < taggingElemets.size() ; i++)
    {
        timerLoop.start();

        timer.start();
        QMap<QString, QVariant> currentTaggingElement =  taggingElemets.at(i);
        qDebug () << "fetching tagging element at index i took " << timer.elapsed() << " ms";

        QString elementName = taggingElemets.at(i)["name"].toString();

        timer.start();
        QString newName = this->prepareForComparison(elementName);
        qDebug () << "this->prepareForComparison(elementName) took " << timer.elapsed() << " ms";


        timer.start();
        bool tagIsActorNameWithMoreThanOneWord = (elementName.count(" ") > 0) &&
                (currentTaggingElement["table_name"] == "actor" || currentTaggingElement["table_name"] == "actorAlias");
        bool tagIsTagOrWebsiteWithMoreThanOneWord = (elementName.count(".") > 0) &&
                (currentTaggingElement["table_name"] == "tag" || currentTaggingElement["table_name"] == "website");

        bool shouldAddToList = newName != "" &&
                (taggingElemets.at(i)["is_one_word_exempt"].toBool() ||
                tagIsActorNameWithMoreThanOneWord ||
                tagIsTagOrWebsiteWithMoreThanOneWord) &&
                elementName.size() >= 5;

        qDebug () << "Evaluating booleans took " << timer.elapsed() << " ms";

        if (shouldAddToList)
        {
            timer.start();
            currentTaggingElement["name"] = newName;
            qDebug () << "currentTaggingElement[\"name\"] = newName took " << timer.elapsed() << " ms";
            timer.start();
            preparedTaggingElemets.append(currentTaggingElement);
            qDebug () << "preparedTaggingElemets.append(currentTaggingElement); " << timer.elapsed() << " ms";
        }

        qDebug() << "Loop took " << timerLoop.elapsed() << " ms";


    }

    this->tag(allScenes,preparedTaggingElemets,sceneActors,sceneTags,sceneWebsites,"Scene");
    this->tag(allPictures,preparedTaggingElemets,pictureActors,pictureTags,pictureWebsites,"Picture");

    qDebug() << "Finished tagging scenes ...";

    this->dbManager->addSceneActors(sceneActors);
    this->dbManager->addSceneTags(sceneTags);
    this->dbManager->addSceneWebsites(sceneWebsites);

    this->dbManager->addPictureActors(pictureActors);
    this->dbManager->addPictureTags(pictureTags);
    this->dbManager->addPictureWebsite(pictureWebsites);



//    for (int i = 0 ; i < allScenes.size() ; i++)
//    {
//        QMap<QString, QVariant> currentScene =  allScenes.at(i);
//        QString sceneName = this->prepareForComparison(allScenes.at(i)["path_to_file"].toString());
//        qDebug() << "Checking " << i << " Out of " << allScenes.size() << " Scenes:  " << sceneName << " for tags...";
//        for (int j = 0 ; j < preparedTaggingElemets.size() ; j++)
//        {

//            QString nameToCheck = preparedTaggingElemets.at(j)["name"].toString();
//            QMap<QString, QVariant> currentTaggingElement =  preparedTaggingElemets.at(j);

//            if (sceneName.contains(nameToCheck)){

//                qDebug() << "Found " << nameToCheck << "in" << sceneName;

//                if (currentTaggingElement["table_name"] == "actor")
//                {
//                    QStringList tempSceneActor;
//                    tempSceneActor.append(currentScene["id"].toString());
//                    tempSceneActor.append(currentTaggingElement["id"].toString());
//                    sceneActors.append(tempSceneActor);

//                }else if (currentTaggingElement["table_name"] == "tag")
//                {
//                    QStringList tempSceneActor;
//                    tempSceneActor.append(currentScene["id"].toString());
//                    tempSceneActor.append(currentTaggingElement["id"].toString());

//                    sceneTags.append(tempSceneActor);
//                }else if (currentTaggingElement["table_name"] == "website")
//                {
//                    QStringList tempSceneActor;
//                    tempSceneActor.append(currentScene["id"].toString());
//                    tempSceneActor.append(currentTaggingElement["id"].toString());

//                    sceneWebsites.append(tempSceneActor);
//                }else if (currentTaggingElement["table_name"] == "actorAlias")
//                {
//                    QStringList tempSceneActor;
//                    tempSceneActor.append(currentScene["id"].toString());
//                    tempSceneActor.append(currentTaggingElement["alias_of"].toString());

//                    sceneActors.append(tempSceneActor);
//                }else if (currentTaggingElement["table_name"] == "tagAlias")
//                {
//                    QStringList tempSceneActor;
//                    tempSceneActor.append(currentScene["id"].toString());
//                    tempSceneActor.append(currentTaggingElement["alias_of"].toString());

//                    sceneTags.append(tempSceneActor);
//                }else if (currentTaggingElement["table_name"] == "websiteAlias")
//                {
//                    QStringList tempSceneActor;
//                    tempSceneActor.append(currentScene["id"].toString());
//                    tempSceneActor.append(currentTaggingElement["alias_of"].toString());

//                    sceneWebsites.append(tempSceneActor);
//                }
//            }
//            sceneName = sceneName.replace(nameToCheck,"");

//        }
//    }




}

QString Tagger::prepareForComparison(QString stringToPrepare)
{
    QString source = stringToPrepare;
    QString temp = stringToPrepare.toLower();
//    QString stringNormalized = temp.normalized (QString::NormalizationForm_KD);
    QString badChars = ":!@#$%^&*()_+ ,/\\.-";

    for (int i = 0 ; i < badChars.size() ; i++)
    {
        temp = temp.replace(badChars.at(i),"");
    }

//    temp = temp.replace(QRegExp("[^0-9a-zA-Z]+"),"");
//    stringNormalized.remove(QRegExp("[^a-zA-Z\\s]"));

    return temp;
}

void Tagger::tag(QList<QMap<QString, QVariant> > thingsToTag,
                 QList<QMap<QString, QVariant> > preparedTaggingElemets,
                 QList<QStringList> &actors, QList<QStringList> &tags, QList<QStringList> &websites, QString thingToTagType)
{
    for (int i = 0 ; i < thingsToTag.size() ; i++)
    {
        QMap<QString, QVariant> currentThingToTag =  thingsToTag.at(i);
        QString thingToTagName = this->prepareForComparison(currentThingToTag["path_to_file"].toString());

        qDebug() << "Checking " << i << " Out of " << thingsToTag.size() << " " + thingToTagType +  " " << thingToTagName << " for tags...";
        for (int j = 0 ; j < preparedTaggingElemets.size() ; j++)
        {
            QMap<QString, QVariant> currentTaggingElement =  preparedTaggingElemets.at(j);
            QString nameToCheck = currentTaggingElement["name"].toString();


            if (thingToTagName.contains(nameToCheck)){

                qDebug() << "Found " << nameToCheck << "in" << thingToTagName;

                if (currentTaggingElement["table_name"] == "actor")
                {
                    QStringList temp;
                    temp.append(currentThingToTag["id"].toString());
                    temp.append(currentTaggingElement["id"].toString());
                    actors.append(temp);

                }else if (currentTaggingElement["table_name"] == "tag")
                {
                    QStringList temp;
                    temp.append(currentThingToTag["id"].toString());
                    temp.append(currentTaggingElement["id"].toString());
                    tags.append(temp);

                }else if (currentTaggingElement["table_name"] == "website")
                {
                    QStringList temp;
                    temp.append(currentThingToTag["id"].toString());
                    temp.append(currentTaggingElement["id"].toString());
                    websites.append(temp);


                }else if (currentTaggingElement["table_name"] == "actorAlias")
                {
                    QStringList temp;
                    temp.append(currentThingToTag["id"].toString());
                    temp.append(currentTaggingElement["alias_of"].toString());
                    actors.append(temp);


                }else if (currentTaggingElement["table_name"] == "tagAlias")
                {
                    QStringList temp;
                    temp.append(currentThingToTag["id"].toString());
                    temp.append(currentTaggingElement["alias_of"].toString());
                    tags.append(temp);

                }else if (currentTaggingElement["table_name"] == "websiteAlias")
                {
                    QStringList temp;
                    temp.append(currentThingToTag["id"].toString());
                    temp.append(currentTaggingElement["alias_of"].toString());
                    websites.append(temp);
                }
            }
            thingToTagName = thingToTagName.replace(nameToCheck,"");

        }
    }

}
