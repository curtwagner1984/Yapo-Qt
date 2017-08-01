#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include "scenepreviewmodel.h"
#include <algorithm>


ScenePreviewModel::ScenePreviewModel(QSettings* settings)
{
    this->_settings = settings;
    this->_homeDir = _settings->value("general/homepath").toString();
}

void ScenePreviewModel::getPreviewPaths(int sceneId)
{
    QString pathOfPreview = QString("%1/scene/%2/contactSheet").arg(_homeDir, QString::number(sceneId));
    QDirIterator it(pathOfPreview, QStringList() << "*.jpg", QDir::Files);

    while (it.hasNext())
    {
        QString fileToCheck = it.next();
        QFile f(fileToCheck);
        QFileInfo fileInfo(f.fileName());
        QString baseName = fileInfo.baseName(); //filename without extention ex:cs_T_00-00-00_O_0
        QRegExp rx("^cs_T_(.*)_O_(.*)");
        int order;
        QString timeStamp;

        if (rx.indexIn(baseName) != -1)
        {
            timeStamp = rx.cap(1).replace('-', ':');
            order = rx.cap(2).toInt();
        }

        QMap<QString, QVariant > item;
        item["imagePath"] = fileToCheck;
        item["order"] = order;
        item["timeStamp"] = timeStamp;
        this->items.append(item);
    }

    this->beginResetModel();
    std::sort(items.begin(), items.end(), &ScenePreviewModel::variantLessThan);
    this->endResetModel();
}

int ScenePreviewModel::rowCount(const QModelIndex& parent) const
{
    return this->items.size();
}

QVariant ScenePreviewModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    QMap<QString, QVariant> currentItem = this->items.at(index.row());

    if (role == PreviewPathRole)
    {
        return currentItem["imagePath"];
    }
    else if (role == PreviewOrderRole)
    {
        return currentItem["order"];
    }
    else if (role == PreviewTimeStampRole)
    {
        return currentItem["timeStamp"];
    }
    else
    {
        return QVariant();
    }
}

QHash<int, QByteArray> ScenePreviewModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PreviewPathRole] = "previewPath";
    roles[PreviewTimeStampRole] = "previewTimestamp";
    roles[PreviewOrderRole] = "previewOrder";
    return roles;
}

void ScenePreviewModel::clear()
{
    this->beginResetModel();

    if (this->items.size() > 0)
    {
        this->items.clear();
    }

    this->endResetModel();
}

bool ScenePreviewModel::variantLessThan(const QMap<QString, QVariant >&  v1, const QMap<QString, QVariant >&  v2)
{
    return v1["order"].toInt() < v2["order"].toInt();
}
