#include "dbmanager.h"

#include <QDateTime>
#include <QtDebug>
#include <QList>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariantList>
#include <QElapsedTimer>

//QmlComm::QmlComm(DbManager* dbManager,QObject *parent) : QObject(parent)
DbManager::DbManager(const QString& path, QObject *parent):QObject(parent)
{
  //    ignoring path just for testing
  this->m_db = QSqlDatabase::addDatabase("QSQLITE");
  //       this->m_db.setDatabaseName(path);
  this->m_db.setDatabaseName("test.db");

  if (!this->m_db.open()) {
    qDebug() << "Error: connection with database fail";
  } else {
    qDebug() << "Database: connection ok";
    this->m_db.transaction();
    this->createTables();
    if (this->m_db.commit()){
        qDebug() << "Tables created ...";
    }else{
        qDebug() << "Error while trying to create tables: " << this->m_db.lastError();
    }
  }
}


void DbManager::createTables() {
  QList<QString> createStatmentList;
  QString actorTableCreateStatment =
      "CREATE TABLE IF NOT EXISTS \"Actor\" ( "
      "id INTEGER PRIMARY KEY, "
      "name VARCHAR NOT NULL UNIQUE, "
      "gender INTEGER, "
      "description VARCHAR, "
      "thumbnail VARCHAR, "
      "ethnicity VARCHAR, "
      "country_of_origin VARCHAR, "
      "imdb_id VARCHAR, "
      "tmdb_id VARCHAR, "
      "official_pages VARCHAR, "
      "tattoos VARCHAR, "
      "measurements VARCHAR, "
      "weight VARCHAR, "
      "height VARCHAR, "
      "rating INTEGER, "
      "play_count INTEGER, "
      "is_fav BOOLEAN, "
      "is_runner_up BOOLEAN, "
      "is_exempt_from_one_word_search BOOLEAN DEFAULT 0, "
      "is_mainstream BOOLEAN, "
      "date_fav DATETIME, "
      "date_runner_up DATETIME, "
      "date_of_birth DATETIME, "
      "date_last_lookup DATETIME, "
      "created_at DATETIME DEFAULT (DATETIME('now','localtime')), "
      "updated_at DATETIME)";

  createStatmentList.append(actorTableCreateStatment);

  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"Scene\" ( id INTEGER PRIMARY KEY, name "
      "VARCHAR, path_to_file VARCHAR NOT NULL UNIQUE, description VARCHAR, "
      "thumbnail VARCHAR, codec_name VARCHAR, rating INTEGER, play_count "
      "INTEGER, bit_rate INTEGER, duration INTEGER, size INTEGER, framerate "
      "FLOAT, width INTEGER, height INTEGER, is_fav BOOLEAN, is_runner_up "
      "BOOLEAN, date_fav DATETIME, date_runner_up DATETIME, date_last_lookup DATETIME, created_at DEFAULT (DATETIME('now','localtime')), updated_at "
      "DATETIME)");
  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"Picture\" ( id INTEGER PRIMARY KEY, name "
      "VARCHAR, path_to_file VARCHAR NOT NULL UNIQUE, description VARCHAR, "
      "thumbnail VARCHAR, rating INTEGER, play_count INTEGER, size INTEGER, "
      "megapixel FLOAT, width INTEGER, height INTEGER, is_fav BOOLEAN, "
      "is_runner_up BOOLEAN, date_fav DATETIME, date_runner_up DATETIME, "
      "date_last_lookup DATETIME, created_at DEFAULT (DATETIME('now','localtime')), "
      "updated_at DATETIME)");
  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"Tag\" ( id INTEGER PRIMARY KEY, name "
      "VARCHAR NOT NULL UNIQUE, description VARCHAR, thumbnail VARCHAR, rating "
      "INTEGER, play_count INTEGER, is_fav BOOLEAN, is_runner_up BOOLEAN, "
      "is_exempt_from_one_word_search BOOLEAN DEFAULT 0, is_mainstream "
      "BOOLEAN, date_fav DATETIME, date_runner_up DATETIME, created_at "
      "DEFAULT (DATETIME('now','localtime')), updated_at DATETIME)");
  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"Website\" ( id INTEGER PRIMARY KEY, name "
      "VARCHAR NOT NULL UNIQUE, description VARCHAR, thumbnail VARCHAR, rating "
      "INTEGER, play_count INTEGER, is_fav BOOLEAN, is_runner_up BOOLEAN, "
      "is_exempt_from_one_word_search BOOLEAN DEFAULT 0, is_mainstream "
      "BOOLEAN, date_fav DATETIME, date_runner_up DATETIME, created_at "
      "DEFAULT (DATETIME('now','localtime')), updated_at DATETIME)");
  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"MediaFolder\" ( id INTEGER PRIMARY KEY, "
      "name VARCHAR, path_to_dir VARCHAR UNIQUE, is_picture BOOLEAN, is_video "
      "BOOLEAN, created_at DEFAULT (DATETIME('now','localtime')), updated_at DATETIME)");

  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"ActorAlias\" ( id INTEGER PRIMARY KEY, "
      "name VARCHAR NOT NULL, actor_id INTEGER, is_exempt_from_one_word_search "
      "BOOLEAN DEFAULT 0, created_at DEFAULT (DATETIME('now','localtime')), updated_at DATETIME, FOREIGN "
      "KEY(actor_id) REFERENCES \"Actor\" (id), UNIQUE(name, actor_id) )");
  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"TagAlias\" ( id INTEGER PRIMARY KEY, name "
      "VARCHAR NOT NULL, tag_id INTEGER, is_exempt_from_one_word_search "
      "BOOLEAN DEFAULT 0, created_at DEFAULT (DATETIME('now','localtime')), updated_at DATETIME, FOREIGN "
      "KEY(tag_id) REFERENCES \"Tag\" (id), UNIQUE(name, tag_id))");
  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"WebsiteAlias\" ( id INTEGER PRIMARY KEY, "
      "name VARCHAR NOT NULL, website_id INTEGER, "
      "is_exempt_from_one_word_search BOOLEAN DEFAULT 0, created_at DEFAULT (DATETIME('now','localtime')), "
      "updated_at DATETIME, FOREIGN KEY(website_id) REFERENCES \"Website\" "
      "(id), UNIQUE(name, website_id))");

  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"Actor_Tag\" ( actor_id INTEGER, tag_id "
      "INTEGER, FOREIGN KEY(actor_id) REFERENCES \"Actor\" (id), FOREIGN "
      "KEY(tag_id) REFERENCES \"Tag\" (id), UNIQUE(actor_id, tag_id) )");
  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"Picture_Actor\" ( picture_id INTEGER, "
      "actor_id INTEGER, FOREIGN KEY(picture_id) REFERENCES \"Picture\" (id), "
      "FOREIGN KEY(actor_id) REFERENCES \"Actor\" (id), UNIQUE(picture_id, actor_id) )");
  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"Picture_Tag\" ( picture_id INTEGER, tag_id "
      "INTEGER, FOREIGN KEY(picture_id) REFERENCES \"Picture\" (id), FOREIGN "
      "KEY(tag_id) REFERENCES \"Tag\" (id), UNIQUE(picture_id, tag_id) )");
  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"Picture_Website\" ( picture_id INTEGER, "
      "website_id INTEGER, FOREIGN KEY(picture_id) REFERENCES \"Picture\" "
      "(id), FOREIGN KEY(website_id) REFERENCES \"Website\" (id) ,UNIQUE(picture_id, website_id) )");
  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"Scene_Actor\" ( scene_id INTEGER, actor_id "
      "INTEGER, FOREIGN KEY(scene_id) REFERENCES \"Scene\" (id), FOREIGN "
      "KEY(actor_id) REFERENCES \"Actor\" (id) ,UNIQUE(scene_id, actor_id) )");
  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"Scene_Tag\" ( scene_id INTEGER, tag_id "
      "INTEGER, FOREIGN KEY(scene_id) REFERENCES \"Scene\" (id), FOREIGN "
      "KEY(tag_id) REFERENCES \"Tag\" (id) ,UNIQUE(scene_id, tag_id) )");
  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"Scene_Website\" ( scene_id INTEGER, "
      "website_id INTEGER, FOREIGN KEY(scene_id) REFERENCES \"Scene\" (id), "
      "FOREIGN KEY(website_id) REFERENCES \"Website\" (id) ,UNIQUE(scene_id, website_id) )");
  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"Website_Tag\" ( website_id INTEGER, tag_id "
      "INTEGER, FOREIGN KEY(website_id) REFERENCES \"Website\" (id), FOREIGN "
      "KEY(tag_id) REFERENCES \"Tag\" (id) ,UNIQUE(website_id, tag_id) )");

  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"TreeFolder\" ( id INTEGER PRIMARY KEY , "
      "path_to_dir VARCHAR NOT NULL UNIQUE, level INTEGER NOT NULL,created_at DEFAULT (DATETIME('now','localtime')), "
      "updated_at DATETIME)");

  createStatmentList.append(
      "CREATE TABLE IF NOT EXISTS \"LastLookup\" ( id INTEGER PRIMARY KEY , "
      "Last_Lookup DATETIME)");

  //  CREATE [UNIQUE] INDEX index_name ON table_name(indexed_column);
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Actor_id ON Actor(id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Scene_id ON Scene(id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Picture_id ON Picture(id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Website_id ON Website(id)");
  createStatmentList.append("CREATE INDEX IF NOT EXISTS ix_Tag_id ON Tag(id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_MediaFolder_id ON MediaFolder(id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_TreeFolder_id ON TreeFolder(id)");

  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_ActorAlias_actor_id ON "
      "ActorAlias(actor_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_TagAlias_tag_id ON TagAlias(tag_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_WebsiteAlias_website_id ON "
      "WebsiteAlias(website_id)");

  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Actor_Tag_actor_id ON "
      "Actor_Tag(actor_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Actor_Tag_tag_id ON Actor_Tag(tag_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Picture_Actor_picture_id ON "
      "Picture_Actor(picture_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Picture_Actor_actor_id ON "
      "Picture_Actor(actor_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Picture_Tag_picture_id ON "
      "Picture_Tag(picture_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Picture_Tag_tag_id ON "
      "Picture_Tag(tag_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Picture_Website_picture_id ON "
      "Picture_Website(picture_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Picture_Website_website_id ON "
      "Picture_Website(website_id)");

  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Scene_Actor_scene_id ON "
      "Scene_Actor(scene_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Scene_Actor_actor_id ON "
      "Scene_Actor(actor_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Scene_Tag_scene_id ON "
      "Scene_Tag(scene_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Scene_Tag_tag_id ON "
      "Scene_Tag(tag_id)");

  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Scene_Website_scene_id ON "
      "Scene_Website(scene_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Scene_Website_website_id ON "
      "Scene_Website(website_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Website_Tag_website_id ON "
      "Website_Tag(website_id)");
  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Website_Tag_tag_id ON "
      "Website_Tag(tag_id)");

  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_TreeFolder_path_to_dir ON "
      "TreeFolder(path_to_dir)");

  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Scene_path_to_file ON "
      "Scene(path_to_file)");

  createStatmentList.append(
      "CREATE INDEX IF NOT EXISTS ix_Picture_path_to_file ON "
      "Picture(path_to_file)");



  for (int i = 0; i < createStatmentList.size(); i++) {
    QSqlQuery query;
    if (!query.exec(createStatmentList.at(i))){
        qDebug() << "SQL ERROR: " << query.lastError();
    }

  }
}


QList<QMap<QString, QVariant>> DbManager::actorSearch(const QString searchString)
{

  QString escapedSearchString = this->escapeSqlChars(searchString);
  QString stmt = "SELECT *, "
                 "'actor' as type, "
                 "(SELECT COUNT(*) FROM Scene_Actor WHERE Scene_Actor.actor_id = Actor.id) as NumberOfScenes, "
                 "(SELECT COUNT(*) FROM Picture_Actor WHERE Picture_Actor.actor_id = Actor.id) as NumberOfPictures "
                 "FROM Actor WHERE Actor.name LIKE '%" + escapedSearchString  +"%'";

  return this->executeFetchQueryWrapper(stmt,"Actor Search");
}

QList<QMap<QString, QVariant> > DbManager::sceneSearch(const QString searchString)
{

    QString escapedSearchString = this->escapeSqlChars(searchString);
    QString stmt = "SELECT * FROM Scene WHERE Scene.path_to_file LIKE '%" + escapedSearchString  +"%' ";

    return this->executeFetchQueryWrapper(stmt,"Scene Search");

}

QList<QMap<QString, QVariant> > DbManager::pictureSearch(const QString searchString)
{
    QString escapedSearchString = this->escapeSqlChars(searchString);
    QString stmt = "SELECT * FROM Picture WHERE Picture.path_to_file LIKE '%" + escapedSearchString  +"%' ";

    return this->executeFetchQueryWrapper(stmt,"Picture Search");

}

QList<QMap<QString, QVariant>> DbManager::mediaFolderSearch(const QString searchString)
{
    QString escapedSearchString = this->escapeSqlChars(searchString);
    QString stmt = "SELECT *, 'mediafolder' as type FROM MediaFolder WHERE path_to_dir LIKE '%" + escapedSearchString  +"%'";
    return this->executeFetchQueryWrapper(stmt,"Media Folder Search");

}

QList<QMap<QString, QVariant> > DbManager::getScenesBeforeFFprobe()
{
    QString stmt = "SELECT * FROM Scene WHERE Scene.date_last_lookup is NULL";
    return this->executeFetchQueryWrapper(stmt,"Get Scenes Before FFPROBE");

}

QList<QMap<QString, QVariant> > DbManager::getActorsBeforeScrape()
{
    QString stmt = "SELECT * FROM Actor WHERE Actor.date_last_lookup is NULL ORDER BY Actor.name";
    return this->executeFetchQueryWrapper(stmt,"Get Actors Before Scrape");
}

QList<QMap<QString, QVariant> > DbManager::getAllScenes()
{
    QString stmt = "SELECT * FROM Scene";
    return this->executeFetchQueryWrapper(stmt,"Get All Scenes");

}

QList<QMap<QString, QVariant> > DbManager::getAllPictures()
{
    QString stmt = "SELECT * FROM Picture";
    return this->executeFetchQueryWrapper(stmt,"Get All Pictures");
}

QList<QMap<QString, QVariant> > DbManager::getTaggingElements()
{
    QString stmt = "select * from "
                   "( select Actor.id as id, Actor.name as name, 'actor' as table_name, 'na' as alias_of, Actor.is_exempt_from_one_word_search as is_one_word_exempt from Actor "
                   "union all "
                   "select Tag.id as id, Tag.name as name, 'tag' as table_name, 'na' as alias_of, Tag.is_exempt_from_one_word_search as is_one_word_exempt from Tag "
                   "union all "
                   "select Website.id as id, Website.name as name , 'website' as table_name, 'na' as alias_of, Website.is_exempt_from_one_word_search as is_one_word_exempt from Website "
                   "union all "
                   "select ActorAlias.id as id, ActorAlias.name as name , 'actorAlias' as table_name, ActorAlias.actor_id as alias_of, ActorAlias.is_exempt_from_one_word_search as is_one_word_exempt  from ActorAlias )"
                   " order by LENGTH(name) DESC";
    return this->executeFetchQueryWrapper(stmt,"Get Tagging elements");
}

QList<QMap<QString, QVariant> > DbManager::executeArbitrarySqlWithReturnValue(const QString sqlStatment)
{
    qDebug() << "DbManager: Executing: " << sqlStatment;
    return this->executeFetchQueryWrapper(sqlStatment,sqlStatment);
}

bool DbManager::executeArbitrarySqlWithoutReturnValue(const QString sqlStatment)
{
    qDebug() << "DbManager: Executing: " << sqlStatment;
    return this->executeQuery(sqlStatment,"executeArbitrarySqlWithoutReturnValue");

}

bool DbManager::executeArbitrarySqlWithoutReturnValueForTransaction(QSqlQuery query)
{

    qDebug() << "DbManager: Executing: " << query.lastQuery();

//    QMapIterator<QString, QVariant> i(query.boundValues());
//          while (i.hasNext()) {
//              i.next();
//              qDebug() << i.key().toUtf8().data() << ": "
//                   << i.value().toString().toUtf8().data() << endl;
//          }

    QList<QVariant> list = query.boundValues().values();
          for (int i = 0; i < list.size(); ++i)
              qDebug() << i << ": " << list.at(i).toString().toUtf8().data() << endl;

    bool ans = query.exec();
    if (ans){
        qDebug() << "Success";
    }else{
        qDebug() << "Sql error: " << query.lastError();
    }

    return ans;
//    return true;
//    return this->executeQueryForTransaction(sqlStatment,"executeArbitrarySqlWithoutReturnValue");

}

bool DbManager::beginTransaction()
{
    return this->m_db.transaction();
}

bool DbManager::commitTransaction()
{
    bool success = false;

        if (this->m_db.commit()){
            success = true;

            qDebug() << "Succesfully commited transaction "  ;
    //        qDebug() << "Last query error" << this->m_db.lastError();
    //        qDebug() << "Last query" << query.lastQuery();
        }else{
            qDebug() << "SQL error in ";
//            qDebug() << "Executed Query: ";

        }
        return success;
}

//QSqlQuery DbManager::getNewScenes()
//{
//    QString stmt = "select * from Scene where (Scene.thumbnail is NULL and Scene.date_last_lookup is NULL)";
//    this->executeQuery(stmt,"Get New Scenes");

//}

bool DbManager::addActor(QString actorName, bool isMainstream) {
  bool success = false;
  QString stmt = "INSERT OR IGNORE INTO Actor (name,is_mainstream) VALUES ('"+ actorName +"','" + int(isMainstream) + "')";
  QSqlQuery query (stmt);
  if (query.exec()) {
    success = true;
    qDebug() << "added Actor :  " << actorName << " To database...";
  } else {
    qDebug() << "add Actor error:  " << query.lastError();
  }
  return success;
}



bool DbManager::addTagWithRelation(QString tagId, QString tagName, QString tagType, QString tagOfId)
{
    bool success = false;
    QString tableName;
    QString firstColumnName;
    QString secondColumnName;

    if (tagId == "")
    {
        QString stmt = "INSERT OR IGNORE INTO Tag (name) VALUES ('%1')";
        stmt  = stmt.arg(tagName);

        this->executeArbitrarySqlWithoutReturnValue(stmt);

        QList<QMap<QString, QVariant>> res = this->executeArbitrarySqlWithReturnValue("SELECT last_insert_rowid();");

        tagId = res[0]["last_insert_rowid()"].toString();

    }

    if (tagType == "Actor")
    {
         tableName = "Actor_Tag";
         firstColumnName = "actor_id";
         secondColumnName = "tag_id";
    }else if (tagType == "Scene")
    {
         tableName = "Scene_Tag";
         firstColumnName = "scene_id";
         secondColumnName = "tag_id";
    }else if (tagType == "Website")
    {
         tableName = "Website_Tag";
         firstColumnName = "website_id";
         secondColumnName = "tag_id";
    }else if (tagType == "Picture")
    {
         tableName = "Picture_Tag";
         firstColumnName = "picture_id";
         secondColumnName = "tag_id";
    }

    QString firstValue = tagOfId;
    QString secondValue = tagId;

    QString stmt2 = "INSERT OR IGNORE INTO %1 (%2,%3) VALUES ('%4','%5')";
    stmt2 = stmt2.arg(tableName,firstColumnName,secondColumnName,firstValue,secondValue);



    return this->executeArbitrarySqlWithoutReturnValue(stmt2);



}

bool DbManager::addTag(QString tagName)
{
    bool success = false;
    QString stmt = "INSERT OR IGNORE INTO Tag (name) VALUES ('"+ tagName +"')";
    QSqlQuery query (stmt);
    if (query.exec()) {
      success = true;
      qDebug() << "added Tag :  " << tagName << " To database...";
    } else {
      qDebug() << "add Tag error:  " << query.lastError();
    }
    return success;

}

bool DbManager::addWebsite(QString websiteName)
{

    QString stmt = "INSERT OR IGNORE INTO Website (name) VALUES ('"+ websiteName +"')";
    return this->executeQuery(stmt,"Add Website");


}

bool DbManager::addScene(QString name, QString path)
{

    name = this->escapeSqlChars(name);
    path = this->escapeSqlChars(path);
    QString stmt = "INSERT OR IGNOR INTO SCENE (name,path_to_file) VALUES ('" +name + "','"+ path + "')";
    return this->executeQuery(stmt,"Add Scene");

}

bool DbManager::addPicture(QString name, QString path)
{

    name = this->escapeSqlChars(name);
    path = this->escapeSqlChars(path);
    QString stmt = "INSERT OR IGNOR INTO Picture (name,path_to_file) VALUES ('" +name + "','"+ path + "')";
    return this->executeQuery(stmt,"Add Picture");

}

bool DbManager::addMediaFolder(QString folderPath, bool isPicture, bool isVideo)
{

    QString currentfolderPath = folderPath;
    currentfolderPath.replace(QString("'"),QString("''"));
    QString stmt = "INSERT OR IGNORE INTO MediaFolder (path_to_dir,is_picture,is_video) VALUES "
                   "('"+ currentfolderPath +"','" + QString::number(isPicture) + "','" + QString::number(isVideo) + "')";

    return this->executeQuery(stmt,"Add Media Folder");


}

bool DbManager::addActors(QList<QStringList> actorsToAdd)
{

    QString stmt = "INSERT OR IGNORE INTO Actor (name,is_mainstream) VALUES ";
    QString temp = this->generateBulkInsertQueryString(actorsToAdd);
    stmt = stmt.append(temp);
    if (temp != ""){
        return this->executeQuery(stmt, "Add Actors");
    }else{
        return false;
    }

}

bool DbManager::addActorsAliases(QList <QStringList> aliasesToAdd)
{
    QString stmt = "INSERT OR IGNORE INTO ActorAlias (name,actor_id) VALUES ";
    QString temp = this->generateBulkInsertQueryString(aliasesToAdd);
    stmt = stmt.append(temp);
    if (temp != ""){
        return this->executeQuery(stmt, "Add Actors Aliases");
    }else{
        return false;
    }

}

bool DbManager::addSceneActors(QList<QStringList> sceneActorsToAdd)
{
    QString stmt = "INSERT OR IGNORE INTO Scene_Actor (scene_id,actor_id) VALUES ";
    QString temp = this->generateBulkInsertQueryString(sceneActorsToAdd);
    stmt = stmt.append(temp);
    if (temp != ""){
        return this->executeQuery(stmt, "Add addSceneActors");
    }else{
        return false;
    }

}

bool DbManager::addSceneTags(QList<QStringList> sceneTagsToAdd)
{
    QString stmt = "INSERT OR IGNORE INTO Scene_Tag (scene_id,tag_id) VALUES ";
    QString temp = this->generateBulkInsertQueryString(sceneTagsToAdd);
    stmt = stmt.append(temp);
    if (temp != ""){
        return this->executeQuery(stmt, "Add addSceneTags");
    }else{
        return false;
    }

}

bool DbManager::addSceneWebsites(QList<QStringList> sceneWebsitesToAdd)
{
    QString stmt = "INSERT OR IGNORE INTO Scene_Website (scene_id,website_id) VALUES ";
    QString temp = this->generateBulkInsertQueryString(sceneWebsitesToAdd);
    stmt = stmt.append(temp);
    if (temp != ""){
        return this->executeQuery(stmt, "Add addSceneWebsites");
    }else{
        return false;
    }

}

bool DbManager::addPictureActors(QList<QStringList> pictureActorsToAdd)
{
    QString stmt = "INSERT OR IGNORE INTO Picture_Actor (picture_id,actor_id) VALUES ";
    QString temp = this->generateBulkInsertQueryString(pictureActorsToAdd);
    stmt = stmt.append(temp);
    if (temp != ""){
        return this->executeQuery(stmt, "Add addPictureActors");
    }else{
        return false;
    }

}

bool DbManager::addPictureTags(QList<QStringList> pictureTagsToAdd)
{
    QString stmt = "INSERT OR IGNORE INTO Picture_Tag (picture_id,tag_id) VALUES ";
    QString temp = this->generateBulkInsertQueryString(pictureTagsToAdd);
    stmt = stmt.append(temp);
    if (temp != ""){
        return this->executeQuery(stmt, "Add addPictureTags");
    }else{
        return false;
    }

}

bool DbManager::addPictureWebsite(QList<QStringList> pictureWebsitesToAdd)
{
    QString stmt = "INSERT OR IGNORE INTO Picture_Tag (picture_id,website_id) VALUES ";
    QString temp = this->generateBulkInsertQueryString(pictureWebsitesToAdd);
    stmt = stmt.append(temp);
    if (temp != ""){
        return this->executeQuery(stmt, "Add addPictureWebsite");
    }else{
        return false;
    }

}

bool DbManager::addTags(QList<QStringList> tagsToAdd)
{
    QString stmt = "INSERT OR IGNORE INTO Tag (name) VALUES ";
    QString temp = this->generateBulkInsertQueryString(tagsToAdd);
    stmt = stmt.append(temp);
    if (temp != ""){
        return this->executeQuery(stmt, "Add Tags");
    }else{
        return false;
    }


}

bool DbManager::addWebsites(QList<QStringList> websitesToAdd)
{
    QString stmt = "INSERT OR IGNORE INTO Website (name) VALUES ";
    QString temp = this->generateBulkInsertQueryString(websitesToAdd);
    stmt = stmt.append(temp);
    if (temp != ""){
        return this->executeQuery(stmt, "Add Sites");
    }else{
        return false;
    }

}

bool DbManager::addScenes(QList<QStringList> scenesToAdd)
{
    QString stmt = "INSERT OR IGNORE INTO Scene (name,path_to_file) VALUES ";
    QString temp = this->generateBulkInsertQueryString(scenesToAdd);
    stmt = stmt.append(temp);
    if (temp != ""){
        return this->executeQuery(stmt, "Add Scenes");
    }else{
        return false;
    }



}

bool DbManager::addPictures(QList<QStringList> picturesToAdd)
{
    QString stmt = "INSERT OR IGNORE INTO Picture (name,path_to_file) VALUES ";
    QString temp = this->generateBulkInsertQueryString(picturesToAdd);
    stmt = stmt.append(temp);

    if (temp != ""){
        return this->executeQuery(stmt, "Add Pictures");
    }else{
        return false;
    }


}

bool DbManager::addTreeFolders(QList<QStringList> treeFoldersToAdd)
{
    QString stmt = "INSERT OR IGNORE INTO TreeFolder (path_to_dir,level) VALUES ";
    QString temp = this->generateBulkInsertQueryString(treeFoldersToAdd);
    stmt = stmt.append(temp);

    if (temp != ""){
        return this->executeQuery(stmt, "Add TreeFolders");
    }else{
        return false;
    }

}

bool DbManager::updateScenes(QList<QMap<QString, QVariant> > scenesToUpdate)
{

    QString updateStmtHeader = "UPDATE Scene ";

    this->m_db.transaction();

    for (int i = 0 ; i < scenesToUpdate.size() ; i++){
        QString updateStmt = this->generateUpdateQueryString(scenesToUpdate.at(i));
//        qDebug () << "Update stmt is: " << updateStmtHeader + updateStmt;
        QString finalStmt = updateStmtHeader + updateStmt;
        QSqlQuery query (finalStmt);
//        query.exec();
    }

    if (this->m_db.commit()){
        qDebug() << "Succesfully ffprobed scenes";
        return true;
    }else{
        qDebug() << "Error in saving ffprobing scene information" << this->m_db.lastError();
        return false;
    }





}

bool DbManager::updateActors(QList<QMap<QString, QVariant> > actorsToUpdate)
{
    QString updateStmtHeader = "UPDATE Actor ";

    this->m_db.transaction();

    for (int i = 0 ; i < actorsToUpdate.size() ; i++){
        QString updateStmt = this->generateUpdateQueryString(actorsToUpdate.at(i));
//        qDebug () << "Update stmt is: " << updateStmtHeader + updateStmt;
        QString finalStmt = updateStmtHeader + updateStmt;
        QSqlQuery query (finalStmt);
//        qDebug() << query.lastQuery();
//        query.exec();
    }

    if (this->m_db.commit()){
        qDebug() << "Succesfully update actors";
        qDebug() << "Last error: " << this->m_db.lastError();

        return true;
    }else{
        qDebug() << "Error in saving ffprobing scene information" << this->m_db.lastError();
        return false;
    }

}

QString DbManager::generateBulkInsertQueryString(QList<QStringList> bulkInsertArgumentList)
{
    bool first = true;
    QString ans = "";
    for (int i = 0 ; i < bulkInsertArgumentList.size() ; i ++)
    {
        QString row = "(";
        bool firstInRow = true;
        for (int j = 0 ; j < bulkInsertArgumentList.at(i).size() ; j++){
            QString stringToInsert = this->escapeSqlChars(bulkInsertArgumentList.at(i).at(j));
            if (firstInRow){
                row = row + "'" + stringToInsert + "'";
                firstInRow = false;
            }else{
                row = row + ",'" + stringToInsert + "'";
            }
        }
        row = row + ")";

        if (first){
            ans = ans + row;
            first = false;
        }else{
            ans = ans + "," + row;
        }
    }

    return ans;
}

QString DbManager::generateUpdateQueryString(QMap<QString, QVariant> updatedObject)
{
    //  UPDATE employees
    //  SET lastname = 'Smith'
    //  WHERE
    //   employeeid = 3;
    QMapIterator<QString, QVariant> i(updatedObject);
    bool first = true;
    QString csvUpdateStatment = "";
    while(i.hasNext()){
        i.next();
        if (i.key() != "id"){
            QString value = i.value().toString();
            QString escapedValue = this->escapeSqlChars(value);
            if (first){
                if (i.value().type() == QVariant::String && !i.value().toString().contains("DATETIME", Qt::CaseSensitive)){
                    csvUpdateStatment = csvUpdateStatment + i.key()  + " = '" + escapedValue + "'";
                }else{
                    csvUpdateStatment = csvUpdateStatment + i.key()  + " = " + value;
                }

                first = false;
            }else{
                if (i.value().type() == QVariant::String && !i.value().toString().contains("DATETIME", Qt::CaseSensitive) ){
                    csvUpdateStatment = csvUpdateStatment + "," + i.key()  + " = '" + escapedValue + "'";
                }else{
                    csvUpdateStatment = csvUpdateStatment + "," + i.key()  + " = " + value;
                }

            }
        }
    }

    QString fullStmt = "SET " + csvUpdateStatment + " WHERE id = " + updatedObject["id"].toString();
    return fullStmt;
}

QString DbManager::escapeSqlChars(QString string)
{
    QString escapedString = string;
    escapedString.replace(QString("'"),QString("''"));
    return escapedString;
}



bool DbManager::executeQuery(QString sqlStmt, QString sendingFunction)
{
    this->m_db.transaction();
    QSqlQuery query(sqlStmt);
    query.exec();
    bool success = false;
    QElapsedTimer timer;
    timer.start();
    if (this->m_db.commit()){
        success = true;
        qDebug() << "Succesfully executed query for "<< sendingFunction <<" in " << timer.elapsed() << " ms";
//        qDebug() << "Last query error" << this->m_db.lastError();
//        qDebug() << "Last query" << query.lastQuery();
    }else{
        qDebug() << "SQL error in " << sendingFunction  << query.lastError();
        qDebug() << "Executed Query: " << query.executedQuery();

    }
    return success;

}

bool DbManager::executeQueryForTransaction(QString sqlStmt, QString sendingFunction)
{
//    this->m_db.transaction();
    QSqlQuery query(sqlStmt);
    query.exec();

    return true;

}

bool DbManager::executeQuery(QSqlQuery query, QString sendingFunction)
{
    bool success = false;
    if (query.exec()){
        success = true;
        qDebug() << "Succesfully executed query for "<< sendingFunction;
    }else{
        qDebug() << "SQL error in " << sendingFunction  << query.lastError();
        qDebug() << "Executed Query: " << query.executedQuery();

    }
    return success;

}



QList<QMap<QString, QVariant>> DbManager::parseQueryResult(QSqlQuery query)
{
    QList<QMap<QString, QVariant>> result;
    while(query.next())
    {
        QMap<QString, QVariant> tempMap;

        QSqlRecord r = query.record();
        for (int i = 0 ; i < r.count(); i++){
            tempMap[r.fieldName(i)] = query.value(i);
        }
        result.append(tempMap);

    }
    return result;
}

QList<QMap<QString, QVariant> > DbManager::executeFetchQueryWrapper(QString sqlStmt, QString callingFunction)
{
    QSqlQuery query(sqlStmt);
    if (this->executeQuery(query,callingFunction)){
        return this->parseQueryResult(query);
    }else{
        qDebug() << "DB Manager: Error in " << callingFunction << " error:" << this->m_db.lastError();
    }

}




