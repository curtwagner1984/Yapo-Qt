#include "tagger.h"

#include "qdebug.h"

#include <QElapsedTimer>
#include <QTime>

Tagger::Tagger(DbManager* dbManager, QObject *parent) : QObject(parent)
{
    this->dbManager = dbManager;
}

void Tagger::beginTagging()
{
    this->updatedPicturesCount = 0;
    this->updatedScenesCount = 0;

    QTime timer;
    timer.start();

    QString lastLookupStmt = "SELECT * FROM LastLookup WHERE id = 1";
    QString updateLastLookupStmt = "UPDATE LastLookup SET Last_Lookup = DATETIME('now','localtime') WHERE id = 1";
    QList<QMap<QString, QVariant>> last_lookup = this->dbManager->executeArbitrarySqlWithReturnValue(lastLookupStmt);
    bool isFirstScan = false;


    if (last_lookup.size() == 0)
    {
        isFirstScan = true;
        QString insertLastLookup = "INSERT INTO LastLookup (Last_Lookup) VALUES (DATETIME('now','localtime'))";
        this->dbManager->executeArbitrarySqlWithoutReturnValue(insertLastLookup);
        last_lookup = this->dbManager->executeArbitrarySqlWithReturnValue(lastLookupStmt);

    }
    QString lastLookupFromDb = last_lookup.at(0)["Last_Lookup"].toString();


    QString newScenesStmt = "SELECT * FROM Scene WHERE Scene.created_at > '%1'";
    QString newPicturesStmt = "SELECT * FROM Picture WHERE Picture.created_at > '%1'";


    QString allTaggingElementsStmt = "select * from "
                                     "( select Actor.id as id, Actor.name as name, 'actor' as table_name, 'na' as alias_of, Actor.is_exempt_from_one_word_search as is_one_word_exempt from Actor  "
                                     "union all "
                                     "select Tag.id as id, Tag.name as name, 'tag' as table_name, 'na' as alias_of, Tag.is_exempt_from_one_word_search as is_one_word_exempt from Tag  "
                                     "union all "
                                     "select Website.id as id, Website.name as name , 'website' as table_name, 'na' as alias_of, Website.is_exempt_from_one_word_search as is_one_word_exempt from Website  "
                                     "union all "
                                     "select ActorAlias.id as id, ActorAlias.name as name , 'actorAlias' as table_name, ActorAlias.actor_id as alias_of, ActorAlias.is_exempt_from_one_word_search as is_one_word_exempt  from ActorAlias "
                                     "union all "
                                     "select TagAlias.id as id, TagAlias.name as name , 'TagAlias' as table_name, TagAlias.tag_id as alias_of, TagAlias.is_exempt_from_one_word_search as is_one_word_exempt  from TagAlias "
                                     "union all "
                                     "select WebsiteAlias.id as id, WebsiteAlias.name as name , 'WebsiteAlias' as table_name, WebsiteAlias.website_id as alias_of, WebsiteAlias.is_exempt_from_one_word_search as is_one_word_exempt  from WebsiteAlias )"
                                     " order by LENGTH(name) DESC";


    QString newTaggingElementsOnlyStmt = "select * from "
                                     "( select Actor.id as id, Actor.name as name, 'actor' as table_name, 'na' as alias_of, Actor.is_exempt_from_one_word_search as is_one_word_exempt from Actor WHERE Actor.created_at > '%1' "
                                     "union all "
                                     "select Tag.id as id, Tag.name as name, 'tag' as table_name, 'na' as alias_of, Tag.is_exempt_from_one_word_search as is_one_word_exempt from Tag WHERE Tag.created_at > '%1' "
                                     "union all "
                                     "select Website.id as id, Website.name as name , 'website' as table_name, 'na' as alias_of, Website.is_exempt_from_one_word_search as is_one_word_exempt from Website WHERE Website.created_at > '%1' "
                                     "union all "
                                     "select ActorAlias.id as id, ActorAlias.name as name , 'actorAlias' as table_name, ActorAlias.actor_id as alias_of, ActorAlias.is_exempt_from_one_word_search as is_one_word_exempt  from ActorAlias WHERE ActorAlias.created_at > '%1' "
                                     "union all "
                                     "select TagAlias.id as id, TagAlias.name as name , 'TagAlias' as table_name, TagAlias.tag_id as alias_of, TagAlias.is_exempt_from_one_word_search as is_one_word_exempt  from TagAlias WHERE TagAlias.created_at > '%1' "
                                     "union all "
                                     "select WebsiteAlias.id as id, WebsiteAlias.name as name , 'WebsiteAlias' as table_name, WebsiteAlias.website_id as alias_of, WebsiteAlias.is_exempt_from_one_word_search as is_one_word_exempt  from WebsiteAlias WHERE WebsiteAlias.created_at > '%1' )"
                                     " order by LENGTH(name) DESC";

    QSqlQuery query;



    QList<QMap<QString, QVariant>> allScenes = this->dbManager->getAllScenes();
    QList<QMap<QString, QVariant>> newScenesOnly = this->dbManager->executeArbitrarySqlWithReturnValue(newScenesStmt.arg(lastLookupFromDb));

    QList<QMap<QString, QVariant>> allPictures = this->dbManager->getAllPictures();
    QList<QMap<QString, QVariant>> newPicturesOnly = this->dbManager->executeArbitrarySqlWithReturnValue(newPicturesStmt.arg(lastLookupFromDb));

    QList<QMap<QString, QVariant>> allTaggingElemets = this->dbManager->executeArbitrarySqlWithReturnValue(allTaggingElementsStmt);
    QList<QMap<QString, QVariant>> newTaggingElemetsOnly = this->dbManager->executeArbitrarySqlWithReturnValue(newTaggingElementsOnlyStmt.arg(lastLookupFromDb));

    QList<QMap<QString, QVariant>> preparedTaggingElemets = this->prepareTaggingElements(allTaggingElemets);
    QList<QMap<QString, QVariant>> preparedNewTaggingElemets = this->prepareTaggingElements(newTaggingElemetsOnly);


//    QList<QStringList> sceneActors;
//    QList<QStringList> sceneTags;
//    QList<QStringList> sceneWebsites;

//    QList<QStringList> pictureActors;
//    QList<QStringList> pictureTags;
//    QList<QStringList> pictureWebsites;




//    for (int i = 0 ; i < taggingElemets.size() ; i++)
//    {
//        timerLoop.start();

//        timer.start();
//        QMap<QString, QVariant> currentTaggingElement =  taggingElemets.at(i);
//        qDebug () << "fetching tagging element at index i took " << timer.elapsed() << " ms";

//        QString elementName = taggingElemets.at(i)["name"].toString();

//        timer.start();
//        QString newName = this->prepareForComparison(elementName);
//        qDebug () << "this->prepareForComparison(elementName) took " << timer.elapsed() << " ms";


//        timer.start();
//        bool tagIsActorNameWithMoreThanOneWord = (elementName.count(" ") > 0) &&
//                (currentTaggingElement["table_name"] == "actor" || currentTaggingElement["table_name"] == "actorAlias") && elementName.size() > 4;
//        bool tagIsTagOrWebsiteWithMoreThanOneWord = (elementName.count(".") > 0) &&
//                (currentTaggingElement["table_name"] == "tag" || currentTaggingElement["table_name"] == "website");



//        bool shouldAddToList = newName != "" &&
//                (taggingElemets.at(i)["is_one_word_exempt"].toBool() ||
//                tagIsActorNameWithMoreThanOneWord ||
//                tagIsTagOrWebsiteWithMoreThanOneWord);

//        qDebug () << "Evaluating booleans took " << timer.elapsed() << " ms";

//        if (shouldAddToList)
//        {
//            timer.start();
//            currentTaggingElement["name"] = newName;
//            qDebug () << "currentTaggingElement[\"name\"] = newName took " << timer.elapsed() << " ms";
//            timer.start();
//            preparedTaggingElemets.append(currentTaggingElement);
//            qDebug () << "preparedTaggingElemets.append(currentTaggingElement); " << timer.elapsed() << " ms";
//        }

//        qDebug() << "Loop took " << timerLoop.elapsed() << " ms";


//    }

    if (isFirstScan)
    {
//        If this is the first scan, we scann all scenes\pictures against all tag elements
        qDebug() << "This is a first scan ... It would take a while...";
        this->dbManager->beginTransaction();
        this->tag(allScenes,preparedTaggingElemets,"Scene",query);
        this->dbManager->commitTransaction();

        this->dbManager->beginTransaction();
        this->tag(allPictures,preparedTaggingElemets,"Picture",query);
        this->dbManager->commitTransaction();
    }else
    {

        this->dbManager->beginTransaction();
        this->tag(allScenes,preparedNewTaggingElemets,"Scene",query);
        this->dbManager->commitTransaction();

        this->dbManager->beginTransaction();
        this->tag(allPictures,preparedNewTaggingElemets,"Picture",query);
        this->dbManager->commitTransaction();

        this->dbManager->beginTransaction();
        this->tag(newScenesOnly,preparedTaggingElemets,"Scene",query);
        this->dbManager->commitTransaction();

        this->dbManager->beginTransaction();
        this->tag(newPicturesOnly,preparedTaggingElemets,"Picture",query);
        this->dbManager->commitTransaction();

    }



    this->dbManager->executeArbitrarySqlWithoutReturnValue(updateLastLookupStmt);
    qDebug() << "Finished tagging scenes ...";

//    this->dbManager->addSceneActors(sceneActors);
//    this->dbManager->addSceneTags(sceneTags);
//    this->dbManager->addSceneWebsites(sceneWebsites);

//    this->dbManager->addPictureActors(pictureActors);
//    this->dbManager->addPictureTags(pictureTags);
//    this->dbManager->addPictureWebsite(pictureWebsites);
    int taggingTime = timer.elapsed();
    QString taggingTimeString = timer.toString("hh:mm:ss");
    qDebug() << "Tagger is finished!";
    qDebug() << QString("Tagging took %1 ms .# Of Scenes updated: %2, # Of Pictures updated: %3").arg(QString::number(taggingTime),this->updatedScenesCount,this->updatedPicturesCount);





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

QList<QMap<QString, QVariant> > Tagger::prepareTaggingElements(QList<QMap<QString, QVariant> > taggingElements)
{
    QList<QMap<QString, QVariant>> preparedTaggingElemets;

    QElapsedTimer timer;
    QElapsedTimer timerLoop;

    for (int i = 0 ; i < taggingElements.size() ; i++)
    {
        timerLoop.start();

        timer.start();
        QMap<QString, QVariant> currentTaggingElement =  taggingElements.at(i);
        qDebug () << "fetching tagging element at index i took " << timer.elapsed() << " ms";

        QString elementName = taggingElements.at(i)["name"].toString();

        timer.start();
        QString newName = this->prepareForComparison(elementName);
        qDebug () << "this->prepareForComparison(elementName) took " << timer.elapsed() << " ms";


        timer.start();
//        QString preparedName = this->prepareForComparison(elementName);
        bool tagIsActorNameWithMoreThanOneWord = (elementName.count(" ") > 0) &&
                (currentTaggingElement["table_name"] == "actor" || currentTaggingElement["table_name"] == "actorAlias") && newName.size() > 3;
        bool tagIsTagOrWebsiteWithMoreThanOneWord = /*((elementName.count(".") > 0) || (elementName.count(" ") > 0)) &&*/
                currentTaggingElement["table_name"] == "tag" ||
                currentTaggingElement["table_name"] == "website" ||
                currentTaggingElement["table_name"] == "TagAlias" ||
                currentTaggingElement["table_name"] == "WebsiteAlias";



        bool shouldAddToList = (newName != "") &&
                (taggingElements.at(i)["is_one_word_exempt"].toBool() ||
                tagIsActorNameWithMoreThanOneWord ||
                tagIsTagOrWebsiteWithMoreThanOneWord);

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
    return preparedTaggingElemets;
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
                 QList<QMap<QString, QVariant> > preparedTaggingElemets, QString thingToTagType, QSqlQuery &query)
{


    for (int i = 0 ; i < thingsToTag.size() ; i++)
    {
        QMap<QString, QVariant> currentThingToTag =  thingsToTag.at(i);
        QString thingToTagName = this->prepareForComparison(currentThingToTag["path_to_file"].toString());
        bool updated = false;

        qDebug() << "Checking " << i << " Out of " << thingsToTag.size() << " " + thingToTagType +  " " << thingToTagName << " for tags...";
        for (int j = 0 ; j < preparedTaggingElemets.size() ; j++)
        {
            QMap<QString, QVariant> currentTaggingElement =  preparedTaggingElemets.at(j);
            QString nameToCheck = currentTaggingElement["name"].toString();


            if (thingToTagName.contains(nameToCheck)){

                qDebug() << "Found " << nameToCheck << "in" << thingToTagName;
                this->generateUpdateSql(query,thingToTagType,currentTaggingElement,currentThingToTag["id"].toString());
                if (this->dbManager->executeArbitrarySqlWithoutReturnValueForTransaction(query))
                {
                    if(thingToTagType == "Scene"){
                        this->updatedScenesCount++;
                    }else if (thingToTagType == "Picture"){
                        this->updatedPicturesCount++;
                    }
                }

//                if (this->dbManager->executeArbitrarySqlWithoutReturnValue(stmt)){
//                    updated = true;
//                }

//                if (currentTaggingElement["table_name"] == "actor")
//                {
////                    QString tableToInsertTo;
////                    QString firstColumn;
////                    QString secondColumn;
////                    if (thingToTagType == "Scene")
////                    {
////                        tableToInsertTo = "Scene_Actor";
////                        firstColumn = "scene_id";
////                        secondColumn =

////                    }else if (thingToTagType == "Picture"){
////                        tableToInsertTo = "Picture_Actor";
////                    }
//                    QStringList temp;
////                    this->dbManager->executeArbitrarySqlWithoutReturnValue(insertStmt.arg(tableToInsertTo,))
//                    temp.append(currentThingToTag["id"].toString());
//                    temp.append(currentTaggingElement["id"].toString());
//                    actors.append(temp);

//                }else if (currentTaggingElement["table_name"] == "tag")
//                {
//                    QStringList temp;
//                    temp.append(currentThingToTag["id"].toString());
//                    temp.append(currentTaggingElement["id"].toString());
//                    tags.append(temp);

//                }else if (currentTaggingElement["table_name"] == "website")
//                {
//                    QStringList temp;
//                    temp.append(currentThingToTag["id"].toString());
//                    temp.append(currentTaggingElement["id"].toString());
//                    websites.append(temp);


//                }else if (currentTaggingElement["table_name"] == "actorAlias")
//                {
//                    QStringList temp;
//                    temp.append(currentThingToTag["id"].toString());
//                    temp.append(currentTaggingElement["alias_of"].toString());
//                    actors.append(temp);


//                }else if (currentTaggingElement["table_name"] == "tagAlias")
//                {
//                    QStringList temp;
//                    temp.append(currentThingToTag["id"].toString());
//                    temp.append(currentTaggingElement["alias_of"].toString());
//                    tags.append(temp);

//                }else if (currentTaggingElement["table_name"] == "websiteAlias")
//                {
//                    QStringList temp;
//                    temp.append(currentThingToTag["id"].toString());
//                    temp.append(currentTaggingElement["alias_of"].toString());
//                    websites.append(temp);
//                }
            }
            thingToTagName = thingToTagName.replace(nameToCheck,"");

        }
        if (updated){
            QString lookupStmt = QString("UPDATE %1 SET date_last_lookup = DATETIME('now','localtime') WHERE id = %2").arg(thingToTagType,currentThingToTag["id"].toString());
            this->dbManager->executeArbitrarySqlWithoutReturnValue(lookupStmt);
        }

    }

}

void Tagger::generateUpdateSql(QSqlQuery &query, QString thingToTagType, QMap<QString, QVariant> currentTaggingElement, QString firstValue)
{


    QString tableName;
    QString columnOne;
    QString columnTwo;
    QString secondValue;


    if (thingToTagType == "Scene")
    {
        columnOne = "scene_id";
        if (currentTaggingElement["table_name"].toString() == "actor")
        {
            tableName = "Scene_Actor";
            columnTwo = "actor_id";
            secondValue = currentTaggingElement["id"].toString();

        }else if (currentTaggingElement["table_name"].toString() == "tag")
        {
            tableName = "Scene_Tag";
            columnTwo = "tag_id";
            secondValue = currentTaggingElement["id"].toString();

        }else if (currentTaggingElement["table_name"].toString() == "website")
        {
            tableName = "Scene_Website";
            columnTwo = "website_id";
            secondValue = currentTaggingElement["id"].toString();

        }else if (currentTaggingElement["table_name"].toString() == "ActorAlias")
        {
            tableName = "Scene_Actor";
            columnTwo = "actor_id";
            secondValue = currentTaggingElement["alias_of"].toString();

        }else if (currentTaggingElement["table_name"].toString() == "TagAlias")
        {
            tableName = "Scene_Tag";
            columnTwo = "tag_id";
            secondValue = currentTaggingElement["alias_of"].toString();
        }else if (currentTaggingElement["table_name"].toString() == "websiteAlias")
        {
            tableName = "Scene_Website";
            columnTwo = "website_id";
            secondValue = currentTaggingElement["alias_of"].toString();
        }

    }else if (thingToTagType == "Picture")
    {
        columnOne = "picture_id";

        if (currentTaggingElement["table_name"].toString() == "actor")
        {
            tableName = "Picture_Actor";
            columnTwo = "actor_id";
            secondValue = currentTaggingElement["id"].toString();

        }else if (currentTaggingElement["table_name"].toString() == "tag")
        {
            tableName = "Picture_Tag";
            columnTwo = "tag_id";
            secondValue = currentTaggingElement["id"].toString();

        }else if (currentTaggingElement["table_name"].toString() == "website")
        {
            tableName = "Picture_Website";
            columnTwo = "website_id";
            secondValue = currentTaggingElement["id"].toString();

        }else if (currentTaggingElement["table_name"].toString() == "ActorAlias")
        {
            tableName = "Picture_Actor";
            columnTwo = "actor_id";
            secondValue = currentTaggingElement["alias_of"].toString();

        }else if (currentTaggingElement["table_name"].toString() == "TagAlias")
        {
            tableName = "Picture_Tag";
            columnTwo = "tag_id";
            secondValue = currentTaggingElement["alias_of"].toString();
        }else if (currentTaggingElement["table_name"].toString() == "WebsiteAlias")
        {
            tableName = "Picture_Website";
            columnTwo = "website_id";
            secondValue = currentTaggingElement["alias_of"].toString();
        }
    }

//    QString insertStmt = "INSERT OR IGNORE INTO %1 (%2,%3) VALUES ('%4','%5')";

    query.prepare(QString("INSERT INTO %1 (%2,%3) VALUES (?,?)").arg(tableName,columnOne,columnTwo));
//    query.addBindValue(tableName);
//    query.addBindValue(columnOne);
//    query.addBindValue(columnTwo);
    query.addBindValue(firstValue);
    query.addBindValue(secondValue);




}
