#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>
#include <QString>

class Constants : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QString AUTOCOMPLETE_TAG_PLACEHOLDER READ AUTOCOMPLETE_TAG_PLACEHOLDER)
//    Q_PROPERTY(QString AUTOCOMPLETE_WEBSITE_PLACEHOLDER READ AUTOCOMPLETE_WEBSITE_PLACEHOLDER)
//    Q_PROPERTY(QString AUTOCOMPLETE_ACTOR_PLACEHOLDER READ AUTOCOMPLETE_ACTOR_PLACEHOLDER)
//    Q_PROPERTY(QString BUTTON_ICON_PLAY READ BUTTON_ICON_PLAY)
public:
    explicit Constants(QObject *parent = 0);

//    AUTOCOMPLETE
      Q_INVOKABLE QString aUTOCOMPLETE_TAG_PLACEHOLDER() const { return "qrc:/placeholders/placeholders/placeHolderTag_64.jpg";}

      Q_INVOKABLE QString aUTOCOMPLETE_WEBSITE_PLACEHOLDER() const { return "qrc:/placeholders/placeholders/placeHolderWebsite_64.jpg";}

      Q_INVOKABLE QString aUTOCOMPLETE_ACTOR_PLACEHOLDER() const { return "qrc:/placeholders/placeholders/unknown female_256.jpg";}

      Q_INVOKABLE QString nO_IMAGE_LANDSCAPE_PLACEHOLDER() const { return "qrc:/placeholders/placeholders/unknown scene_360.jpg";}

      Q_INVOKABLE QString nO_IMAGE_PORTRAIT_PLACEHOLDER() const { return aUTOCOMPLETE_ACTOR_PLACEHOLDER();}

      Q_INVOKABLE QString bUTTON_ICON_PLAY() const { return "qrc:/icons/icons/ic_play_arrow_black_24px.svg";}

      Q_INVOKABLE QString bUTTON_ICON_FAVORITE() const { return "qrc:/icons/icons/ic_favorite_black_24px.svg";}






signals:

public slots:
};

#endif // CONSTANTS_H
