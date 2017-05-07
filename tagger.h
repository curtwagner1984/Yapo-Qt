#ifndef TAGGER_H
#define TAGGER_H

#include <QObject>
#include "dbmanager.h"

class Tagger : public QObject {
  Q_OBJECT
 public:
  explicit Tagger(DbManager* dbManager, QObject* parent = 0);
  void beginTagging();

 private:
  DbManager* dbManager;
  QString prepareForComparison(QString stringToPrepare);
  void tag(QList<QMap<QString, QVariant>> thingsToTag,
           QList<QMap<QString, QVariant>> preparedTaggingElemets,
           QList<QStringList> &actors, QList<QStringList> &tags,
           QList<QStringList> &websites,
           QString thingToTagType);

 signals:

 public slots:
};

#endif  // TAGGER_H
