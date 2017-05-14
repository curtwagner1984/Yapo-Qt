#include "scenemodel.h"
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>




SceneModel::SceneModel(DbManager *dbManager)
           : BasicListModel(dbManager)
{
    qDebug() << "Making test scene search ...";
    this->MODEL_TYPE = "SceneModel";

    this->baseSqlSelect =  SEARCH_SELECT;
    this->baseSqlFrom = SEARCH_FROM;

    this->generateSqlLimit();
    this->search("");
}

QVariant SceneModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
            return QVariant();
        }

        QMap<QString,QVariant> currentItem = this->items.at(index.row());

        if (role == IdRole){
            return currentItem["id"];
        }else if (role == ThumbRole){
            return currentItem["thumbnail"];
        }else if (role == ThumbRole64){
            QString currentThumbPath = currentItem["thumbnail"].toString();
            QFileInfo fInfo = QFileInfo(currentThumbPath);
            QString path = fInfo.path();
            path = path + "/profile_64.jpg";
            return path;
        }else if (role == PathRole){
            return currentItem["path_to_file"];
        }else if (role == NameRole){
            return currentItem["name"];
        }else if (role == DurationRole){
            int dur = currentItem["duration"].toInt();
            return QDateTime::fromTime_t(dur).toUTC().toString("hh:mm:ss");
        }else{
            return QVariant();
        }
}

void SceneModel::search(const QString searchString)
{
    QString escapedSearchString = this->escaleSqlChars(searchString);
    this->baseSqlWhere = SEARCH_WHERE.arg(escapedSearchString);
    this->baseSqlFrom = SEARCH_FROM;
//    http://stackoverflow.com/questions/1253561/sqlite-order-by-rand
//    SELECT * FROM table WHERE id IN (SELECT id FROM table ORDER BY RANDOM() LIMIT x)

    this->baseSqlOrder = SEARCH_ORDER;


//  Resets count and gets number of items and executes search
    this->baseSearch();
}

void SceneModel::getActorScenes(const QString actorId)
{
    this->baseSqlWhere = "";
    this->baseSqlOrder = "";
    this->baseSqlFrom = ACTOR_SEARCH_FROM.arg(actorId);
    this->baseSqlSelect = ACTOR_SEARCH_SELECT;
    this->baseSearch();
}

void SceneModel::getTagScenes(const QString tagId)
{
    this->baseSqlWhere = "";
    this->baseSqlOrder = "";
    this->baseSqlFrom = TAG_SEARCH_FROM.arg(tagId);
    this->baseSqlSelect = TAG_SEARCH_SELECT;
    this->baseSearch();

}

void SceneModel::getWebsiteScenes(const QString websiteId)
{
        this->baseSqlWhere = this->WEBSITE_SEARCH_WHERE.arg(websiteId);;
        this->baseSqlOrder = "";
        this->baseSqlFrom = this->WEBSITE_SEARCH_FROM;
        this->baseSearch();

}

QHash<int, QByteArray> SceneModel::roleNames() const
{
    QHash<int, QByteArray> roles;
            roles[IdRole] = "id";
            roles[ThumbRole] = "thumb";
            roles[ThumbRole64] = "thumb_64";
            roles[ThumbRole360] = "thumb_360";
            roles[PathRole] = "path";
            roles[NameRole] = "name";
            roles[DurationRole] = "duration";
            return roles;
}
