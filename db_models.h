/*
 * Currently not in use, needs to be removed.
*/

#ifndef DB_MODELS_H
#define DB_MODELS_H
#include <QString>

struct DbModel {};

struct Actor : DbModel {
  int id;
  QString name;
  QString thumbnail;
};

struct Tag : DbModel {
  int id;
  QString name;
};

struct Website : DbModel {
  int id;
  QString name;
};

struct MediaFolder : DbModel {
  int id;
  QString pathToFile;
  bool isPicture;
  bool isVideo;
  int numberOfScenes;
  int numberOfPictures;
};

#endif  // DB_MODELS_H
