#ifndef SCENEPREVIEWMODEL_H
#define SCENEPREVIEWMODEL_H

#include <QAbstractListModel>
#include <QSettings>


class ScenePreviewModel : public QAbstractListModel
{
        Q_OBJECT
    public:
        ScenePreviewModel(QSettings* settings);

        Q_INVOKABLE void getPreviewPaths(int sceneId);
        int rowCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;

        QHash<int, QByteArray> roleNames() const;
        Q_INVOKABLE void clear();

        enum PreviewRoles
        {
            PreviewPathRole = Qt::UserRole + 1,
            PreviewTimeStampRole = Qt::UserRole + 2,
            PreviewOrderRole = Qt::UserRole + 3

        };

    private:
        QList<QMap<QString, QVariant>> items;
        QSettings* _settings;
        QString _homeDir;
        static bool variantLessThan(const QMap<QString, QVariant >& v1, const QMap<QString, QVariant >& v2);
};

#endif // SCENEPREVIEWMODEL_H
