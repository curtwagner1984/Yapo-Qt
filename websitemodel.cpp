#include "websitemodel.h"
#include <QDebug>
#include <QFileInfo>



WebsiteModel::WebsiteModel()
    : BasicListModel()
{
    qDebug() << "Making test website search ...";
    this->MODEL_TYPE = "Website";
    this->baseSqlSelect =  SEARCH_SELECT;
    this->baseSqlFrom = SEARCH_FROM;
    this->generateSqlLimit();
    qDebug() << "Website Model initialized ...";
    //    this->search("");
}

QVariant WebsiteModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    QMap<QString, QVariant> currentItem = this->items.at(index.row());

    if (role == IdRole)
    {
        return currentItem["id"];
    }
    else if (role == ThumbRole)
    {
        return currentItem["thumbnail"];
    }
    else if (role == ThumbRole64)
    {
        QString currentThumbPath = currentItem["thumbnail"].toString();
        QFileInfo fInfo = QFileInfo(currentThumbPath);
        QString path = fInfo.path();
        path = path + "/profile_64.jpg";
        return path;
    }
    else if (role == ThumbRole320)
    {
        QString currentThumbPath = currentItem["thumbnail"].toString();
        QFileInfo fInfo = QFileInfo(currentThumbPath);
        QString path = fInfo.path();
        path = path + "/profile_320.jpg";
        return path;
    }
    else if (role == NumberOfScenesRole)
    {
        return currentItem["NumberOfScenes"];
    }
    else if (role == NumberOfPicturesRole)
    {
        return currentItem["NumberOfPictures"];
    }
    else if (role == NumberOfTagsRole)
    {
        return currentItem["NumberOfTags"];
    }
    else if (role == NameRole)
    {
        return currentItem["name"];
    }
    else if (role == RatingRole)
    {
        return currentItem["rating"];
    }
    else if (role == SelectedRole)
    {
        return currentItem["isSelected"];
    }
    else
    {
        return QVariant();
    }
}

bool WebsiteModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    qDebug() << "Set data in WebsiteModel was called "
             << "index" << index << "Value " << value << "Role " << role;
    bool success = false;

    if (!index.isValid())
    {
        return success;
    }

    //    QMap<QString, QVariant>* currentItem = &(this->items[index.row()]);

    if (role == SelectedRole)
    {
        this->items[index.row()]["isSelected"] = value;
        success = true;
    }

    return success;
}

void WebsiteModel::search(const QString searchString)
{
    QString escapedSearchString = this->escaleSqlChars(searchString);
    this->baseSqlWhere = SEARCH_WHERE.arg(escapedSearchString);
    this->baseSqlSelect = SEARCH_SELECT;
    this->baseSqlFrom = SEARCH_FROM;
    //  Resets count and gets number of items and executes search
    this->baseSearch();
}

void WebsiteModel::searchById(const QString websiteId)
{
    QString escapedSearchString = this->escaleSqlChars(websiteId);
    this->baseSqlWhere = SEARCH_BY_ID_WHERE.arg(escapedSearchString);
    this->baseSqlSelect = SEARCH_SELECT;
    this->baseSqlFrom = SEARCH_FROM;
    //  Resets count and gets number of items and executes search
    this->baseSearch();
}

void WebsiteModel::getTagWebsites(const QString tagId)
{
    this->baseSqlWhere = "";
    this->baseSqlSelect = TAG_SEARCH_SELECT;
    this->baseSqlFrom = TAG_SEARCH_FROM.arg(tagId);
    this->baseSearch();
}

QHash<int, QByteArray> WebsiteModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[ThumbRole] = "thumb";
    roles[ThumbRole64] = "thumb_64";
    roles[ThumbRole320] = "thumb_320";
    roles[NumberOfScenesRole] = "numberOfScenes";
    roles[NumberOfPicturesRole] = "numberOfPictures";
    roles[NumberOfTagsRole] = "numberOfTags";
    roles[NameRole] = "name";
    roles[RatingRole] = "rating";
    roles[SelectedRole] = "isSelected";
    return roles;
}

void WebsiteModel::getSceneWebsitesForTagger(const QString sceneId, bool isMulti)
{
    this->baseSqlSelect = SCENE_SEARCH_SELECT;

    if (isMulti)
    {
        this->baseSqlWhere = SCENE_SEARCH_WHERE_MULTI.arg(sceneId);
    }
    else
    {
        this->baseSqlWhere = SCENE_SEARCH_WHERE.arg(sceneId);
    }

    this->baseSqlFrom = SCENE_SEARCH_FROM;
    this->baseSqlOrder = SCENE_ORDER_BY;
    this->baseSearch();
}

void WebsiteModel::getPictureWebsitesForTagger(const QString pictureId, bool isMulti)
{
    this->baseSqlSelect = PICTURE_SEARCH_SELECT;

    if (isMulti)
    {
        this->baseSqlWhere = PICTURE_SEARCH_WHERE_MULTI.arg(pictureId);
    }
    else
    {
        this->baseSqlWhere = PICTURE_SEARCH_WHERE.arg(pictureId);
    }

    this->baseSqlFrom = PICTURE_SEARCH_FROM;
    this->baseSqlOrder = PICTURE_ORDER_BY;
    this->baseSearch();
}
