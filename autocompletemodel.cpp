#include "autocompletemodel.h"
#include <QDebug>
#include <QFileInfo>
#include <QModelIndex>
#include <QRegularExpression>





AutoCompleteModel::AutoCompleteModel()
    :BasicListModel()
{
    qDebug() << "Making test autocomplete search ...";
    this->isAutoComplete = true;

    this->baseSqlSelect =  SEARCH_SELECT;

    this->baseSqlFrom = SEARCH_FROM;

    this->baseSqlLimit = SEARCH_LIMIT;



    QObject::connect(this->super(), &BasicListModel::baseSearchFinished, this,
                     &AutoCompleteModel::searchReturned);

}

QVariant AutoCompleteModel::data(const QModelIndex &index, int role) const
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
            }else if (role == ThumbRole320){
                QString currentThumbPath = currentItem["thumbnail"].toString();
                QFileInfo fInfo = QFileInfo(currentThumbPath);
                QString path = fInfo.path();
                path = path + "/profile_320.jpg";
                return path;
            }else if (role == NameRole){                
                return currentItem["name"].toString();
            }else if (role == TableNameRole){
                return currentItem["TableName"];
            }else if (role == AliasOfRole){
                return currentItem["alias_of"];
            }else if (role == SelectedRole){
                return currentItem["isSelected"];
            }else if (role == AliasOfIdRole){
                return currentItem["alias_of_id"];
            }else{
                return QVariant();
            }
}

bool AutoCompleteModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug() << "Set data in ActorModel was called "
             << "index" << index << "Value " << value << "Role " << role;
    bool success = false;
    if (!index.isValid()) {
      return success;
    }

//    QMap<QString, QVariant>* currentItem = &(this->items[index.row()]);

    if (role == SelectedRole) {
      this->items[index.row()]["isSelected"] = value;
      success = true;
    }

    if (success){
        this->dataChanged(index,index);
    }

    return success;

}

void AutoCompleteModel::search(const QString searchString , QString searchType)
{
   if (searchType == "Tag"){
       this->tagSearch(searchString);
   }else if (searchType == "MultiSearch"){
       this->generalMultiSearch(searchString);
   }else{
       this->generalSearch(searchString);
   }



}



void AutoCompleteModel::tagSearch(const QString searchString)
{

    QString escapedSearchString = this->escaleSqlChars(searchString);
    this->baseSqlWhere = SEARCH_WHERE.arg(escapedSearchString);
    this->baseSqlOrder = SEARCH_ORDER;
    this->baseSqlFrom = TAG_SEARCH_FROM;

//  Resets count and gets number of items and executes search
    this->currentSearchString = searchString;
    this->baseSearch();
    this->waitingForTagSearchToReturn = true;



}

void AutoCompleteModel::searchReturned()
{
    if(waitingForGenearlSearchToReturn)

    {
        QModelIndex a = QModelIndex();
        this->beginInsertRows(a,this->items.size(), this->items.size() + 2 );

        QString newName = this->currentSearchString.replace("%"," ");
        newName = newName.replace("_",".");

        QMap<QString,QVariant> newActor;
        newActor["name"] = newName;
        newActor["TableName"] = QString("New Actor");
        this->items.append(newActor);

        QMap<QString,QVariant> newTag;
        newTag["name"] = newName;
        newTag["TableName"] = QString("New Tag");
        this->items.append(newTag);

        QMap<QString,QVariant> newWebsite;
        newWebsite["name"] = newName;
        newWebsite["TableName"] = QString("New Website");
        this->items.append(newWebsite);

        this->endInsertRows();

    }else if (waitingForTagSearchToReturn)
    {

        QModelIndex a = QModelIndex();
        this->beginInsertRows(a,this->items.size(), this->items.size());


        QMap<QString,QVariant> newTag;
        newTag["name"] = this->currentSearchString;
        newTag["TableName"] = QString("New Tag");
        this->items.append(newTag);


        this->endInsertRows();

    }else if (waitingForGeneralMultiSearchToReturn)
    {
        for (int i = 0 ; i < generalMultisearchTempList.length() ; i++)
        {
            int singleItemIndex = getIndexOfSingleItemPerSearchTerm(generalMultisearchTempList[i]);
            if (singleItemIndex != -1)
            {

                QModelIndex ix = this->index(singleItemIndex);
                this->setData(ix,true,SelectedRole);
                qDebug() << "Selected : " << this->items[singleItemIndex]["name"].toString();
            }
        }

    }

    this->waitingForGenearlSearchToReturn = false;
    this->waitingForGeneralMultiSearchToReturn = false;
    this->waitingForTagSearchToReturn = false;
}

void AutoCompleteModel::generalSearch(const QString searchString)
{
    QString escapedSearchString = this->escaleSqlChars(searchString);
    this->baseSqlWhere = SEARCH_WHERE.arg(escapedSearchString);
    this->baseSqlOrder = SEARCH_ORDER;
    this->baseSqlFrom = SEARCH_FROM;

//  Resets count and gets number of items and executes search
    this->currentSearchString = escapedSearchString;
    this->baseSearch();
    this->waitingForGenearlSearchToReturn = true;



}

void AutoCompleteModel::generalMultiSearch(const QString searchString)
{
    generalMultisearchTempList.clear();
    generalMultisearchTempList = searchString.split(",");
    QString generatedWhereTemplate = " name LIKE '%%1%' ";
    QString generatedWhereHeader = " WHERE ";
    QString generatedWhereStmt = "";
    bool first = true;
    for (int i = 0 ; i < generalMultisearchTempList.length(); i ++)
    {
        QString tempString = generalMultisearchTempList[i];
        tempString = tempString.simplified();
        QString escapedSearchString = this->escaleSqlChars(tempString);

        if (first){
            generatedWhereStmt = generatedWhereStmt + generatedWhereTemplate.arg(escapedSearchString);
            first = false;
        }else{
            generatedWhereStmt = generatedWhereStmt + " OR " +  generatedWhereTemplate.arg(escapedSearchString);
        }


    }

    generatedWhereStmt = generatedWhereHeader + generatedWhereStmt;

    this->baseSqlWhere = generatedWhereStmt;
    this->baseSqlOrder = SEARCH_ORDER;
    this->baseSqlFrom = SEARCH_FROM;

//  Resets count and gets number of items and executes search
    this->currentSearchString = searchString;
    this->baseSearch();
    this->waitingForGeneralMultiSearchToReturn = true;



}

int AutoCompleteModel::getIndexOfSingleItemPerSearchTerm(QString searchTerm)
{
    int numberOfFound = 0;
    int foundIndex = -1;
    QString searchTermReg = ".*" + searchTerm.replace(" ",".*") + ".*";
    QRegularExpression re(searchTermReg, QRegularExpression::CaseInsensitiveOption);

    for (int i = 0 ; i < this->items.length() ; i++)
    {
        QRegularExpressionMatch match = re.match(items.at(i)["name"].toString());
//        qDebug() << "getIndexOfSingleItemPerSearchTerm: looking at item name: " << items.at(i)["name"].toString();
        if (match.hasMatch()){
            numberOfFound++;
            foundIndex = i;
        }
    }

    if (!(numberOfFound == 1)){
        foundIndex = -1;
    }

    return foundIndex;

}

QHash<int, QByteArray> AutoCompleteModel::roleNames() const
{
    QHash<int, QByteArray> roles;
            roles[IdRole] = "id";
            roles[ThumbRole] = "thumb";
            roles[ThumbRole64] = "thumb_64";
            roles[ThumbRole320] = "thumb_320";
            roles[NameRole] = "name";
            roles[TableNameRole] = "tableName";
            roles[AliasOfRole] = "aliasOf";
            roles[AliasOfIdRole] = "aliasOfId";
            roles[SelectedRole] = "isSelected";
            return roles;
}
