#include "actormodel.h"
#include <qdebug.h>
#include <QFileInfo>
#include <QModelIndex>
#include <QDate>



ActorModel::ActorModel()
    : BasicListModel()
{
    this->MODEL_TYPE = "Actor";
    this->baseSqlSelect =  SEARCH_SELECT;
    this->baseSqlFrom = SEARCH_FROM ;
    this->generateSqlLimit();
    //    this->search("");
    qDebug() << "Actor Model initialized ...";
}



QVariant ActorModel::data(const QModelIndex& index, int role) const
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
    else if (role == NumberOfTags)
    {
        return currentItem["NumberOfTags"];
    }
    else if (role == NameRole)
    {
        return currentItem["name"];
    }
    else if (role == DobRole)
    {
        return currentItem["date_of_birth"];
    }
    else if (role == SelectedRole)
    {
        return currentItem["isSelected"];
    }
    else if (role == AgeRole)
    {
        QString dob = currentItem["date_of_birth"].toString();
        QDate dateDOB = QDate::fromString(dob, "yyyy-MM-dd");
        QDate dateNow = QDate::currentDate();
        qint64 days = dateDOB.daysTo(dateNow);
        //            QDate dateAge = QDate::fromJulianDay(days);
        //            dateAge.addDays(days);
        QVariant ageString = days / 365;
        return ageString;
    }
    else if (role == RatingRole)
    {
        return currentItem["rating"];
    }
    else
    {
        return QVariant();
    }
}

bool ActorModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    qDebug() << "Set data in ActorModel was called "
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



void ActorModel::search(const QString searchString)
{
    QString escapedSearchString = this->escaleSqlChars(searchString);
    this->baseSqlSelect =  SEARCH_SELECT;
    this->baseSqlFrom = SEARCH_FROM ;
    this->baseSqlWhere = SEARCH_WHERE.arg(escapedSearchString);
    //  Resets count and gets number of items and executes search
    this->baseSearch();
}

void ActorModel::searchById(const QString actorId)
{
    QString escapedSearchString = this->escaleSqlChars(actorId);
    this->baseSqlSelect =  SEARCH_SELECT;
    this->baseSqlFrom = SEARCH_FROM ;
    this->baseSqlWhere = SEARCH_BY_ID_WHERE.arg(escapedSearchString);
    //  Resets count and gets number of items and executes search
    this->baseSearch();
}

void ActorModel::getTagActors(const QString tagId)
{
    this->baseSqlFrom = TAG_SEARCH_FROM.arg(tagId);
    this->baseSqlWhere = "";
    //  Resets count and gets number of items and executes search
    this->baseSearch();
}


QHash<int, QByteArray> ActorModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[ThumbRole] = "thumb";
    roles[ThumbRole64] = "thumb_64";
    roles[ThumbRole320] = "thumb_320";
    roles[NumberOfScenesRole] = "numberOfScenes";
    roles[NumberOfPicturesRole] = "numberOfPictures";
    roles[NumberOfTags] = "numberOfTags";
    roles[NameRole] = "name";
    roles[GenderRole] = "gender";
    roles[DobRole] = "DOB";
    roles[AgeRole] = "age";
    roles[RatingRole] = "rating";
    roles[SelectedRole] = "isSelected";
    return roles;
}

void ActorModel::getSceneActorsForTagger(const QString sceneId)
{
    this->baseSqlSelect = SEARCH_SELECT;
    this->baseSqlWhere = SCENE_SEARCH_WHERE.arg(sceneId);
    this->baseSqlFrom = SCENE_SEARCH_FROM;
    this->baseSqlOrder = SCENE_ORDER_BY;
    this->baseSearch();
}

void ActorModel::getPictureActorsForTagger(const QString pictureId)
{
    this->baseSqlSelect = SEARCH_SELECT;
    this->baseSqlWhere = PICTURE_SEARCH_WHERE .arg(pictureId);
    this->baseSqlFrom = PICTURE_SEARCH_FROM;
    this->baseSqlOrder = PICTURE_ORDER_BY;
    this->baseSearch();
}



void ActorModel::setOrder(QString orderBy, QString orderDirection)
{
    BasicListModel::setOrder(orderBy, orderDirection);
}








