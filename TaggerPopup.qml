import QtQuick 2.5
import QtQuick.Controls 2.1

Popup {
    id: taggerPopup
    property string itemToTagId
    property int currentItemIndex
    property string thumbSrc
    property string itemToTagType
    property string itemToTagLable

    property var currentModel

    modal: true
    focus: true

    function switchPage()
    {
        itemToTagId = currentModel.directData("id", currentItemIndex)
        thumbSrc = "file:///" + currentModel.directData("thumb", currentItemIndex)
        itemToTagLable = currentModel.directData("name", currentItemIndex)

        if (itemToTagType === "Actor"){
            tagModel.getActorTagsForTagger(itemToTagId)
            generalAlias.search(itemToTagId, "Actor")
        } else if (itemToTagType === "Scene"){
            actorModel.getSceneActorsForTagger(itemToTagId)
            tagModel.getSceneTagsForTagger(itemToTagId)
            websiteModel.getSceneWebsitesForTagger(itemToTagId)
        }


    }

    function next() {
        currentItemIndex = currentItemIndex + 1
        switchPage();

    }

    function prev() {
        currentItemIndex = currentItemIndex - 1
        switchPage();
    }

    onOpened: {
        if (itemToTagType === "Actor"){
            currentModel = actorModel;
            switchPage();
        }else if (itemToTagType === "Scene"){
            currentModel = sceneModel;
            switchPage();
        }
        contentLoader.source = "TaggerLayout.qml"
        contentLoader.item.state = itemToTagType

    }

    onClosed: {
        tagModel.clear()
        generalAlias.clear()
        thumbSrc = ""
    }

    Shortcut {
        sequence: "Ctrl+Right"
        onActivated: {
            console.log("Sortcut Ctrl+N was pressed!")
            next();
        }
    }

    Shortcut {
        sequence: "Ctrl+Left"
        onActivated: {
            console.log("Sortcut Ctrl+N was pressed!")
            prev();
        }
    }

    contentItem: Loader {
        id: contentLoader
        anchors.fill: parent
    }

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
}
