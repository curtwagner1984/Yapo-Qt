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
    property var tagModel
    property var websiteModel
    property var actorModel
    property var aliasModel


    modal: true
    focus: true

    function switchPage()
    {
        itemToTagId = currentModel.directData("id", currentItemIndex)
        thumbSrc = "file:///" + currentModel.directData("thumb", currentItemIndex)
        itemToTagLable = currentModel.directData("name", currentItemIndex)

        if (itemToTagType === "Actor"){
            tagModel.getActorTagsForTagger(itemToTagId)
            aliasModel.search(itemToTagId, "Actor")
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
//        if (itemToTagType === "Actor"){
////            currentModel = mainAppPage.createModel("Actor");
//            currentModel = mainAppPage.mainStack.currentItem.currentModel
//            switchPage();
//        }else if (itemToTagType === "Scene"){
//            currentModel = mainAppPage.createModel("Scene");
//            switchPage();
//        }

        tagModel = mainAppPage.createModel("Tag");
        websiteModel = mainAppPage.createModel("Website")
        actorModel = mainAppPage.createModel("Actor")
        aliasModel = mainAppPage.createModel("Alias")
        switchPage();
        contentLoader.source = "TaggerLayout.qml"
        contentLoader.item.state = itemToTagType

    }

    onClosed: {
//        currentModel.destroy()
        tagModel.destroy()
        websiteModel.destroy()
        actorModel.destroy()
        aliasModel.destroy()

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
