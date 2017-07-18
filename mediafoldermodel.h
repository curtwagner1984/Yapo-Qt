#ifndef MEDIAFOLDER_MODEL_H
#define MEDIAFOLDER_MODEL_H

#include <QModelIndex>
#include <QSqlQuery>
#include <QList>
#include <QHash>


#include "dbmanager.h"
#include "basiclistmodel.h"

class MediafolderModel : public BasicListModel
{
        Q_OBJECT
    public:
        enum MediaFolderRoles
        {
            IdRole = Qt::UserRole + 1,
            PathRole = Qt::UserRole + 2,
            IsVideoRole = Qt::UserRole + 3,
            IsPictureRole = Qt::UserRole + 4
        };

        MediafolderModel();
        QVariant data(const QModelIndex& index, int role) const;
        Q_INVOKABLE void search(const QString searchString);
        QHash<int, QByteArray> roleNames() const;

    private:


};

#endif // MEDIAFOLDER_MODEL_H
