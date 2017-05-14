#ifndef TAGGER_H
#define TAGGER_H

#include <QObject>
#include <QSqlQuery>
#include "dbmanager.h"

class Tagger : public QObject {
  Q_OBJECT
 public:
  explicit Tagger(DbManager* dbManager, QObject* parent = 0);
  void beginTagging();

 private:
  DbManager* dbManager;
  QList<QMap<QString, QVariant>> prepareTaggingElements (QList<QMap<QString, QVariant>> taggingElements);
  QString prepareForComparison(QString stringToPrepare);
  void tag(QList<QMap<QString, QVariant>> thingsToTag,
           QList<QMap<QString, QVariant>> preparedTaggingElemets,           
           QString thingToTagType, QSqlQuery &query);

  void generateUpdateSql(QSqlQuery &query, QString thingToTagType, QMap<QString, QVariant> currentTaggingElement, QString firstValue);
  int updatedScenesCount = 0;
  int updatedPicturesCount = 0;

 signals:

 public slots:
};

#endif  // TAGGER_H
