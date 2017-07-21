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

    property bool isMulti: false

    modal: true
    focus: true

    function switchPage() {
        if (isMulti)
        {
               itemToTagId = currentModel.getSelectedIdsCSV();
        }else
        {
               itemToTagId = currentModel.directData("id", currentItemIndex)
        }


        thumbSrc = "file:///" + currentModel.directData("thumb",
                                                        currentItemIndex)
        itemToTagLable = currentModel.directData("name", currentItemIndex)


        if (itemToTagType === "Actor") {
            tagModel.getActorTagsForTagger(itemToTagId,isMulti)

            /*
             *There is no point displaying aliases in multi tag mode.
             */
            if(!isMulti){
                aliasModel.search(itemToTagId, "Actor")
            }
        } else if (itemToTagType === "Scene") {
            actorModel.getSceneActorsForTagger(itemToTagId,isMulti)
            tagModel.getSceneTagsForTagger(itemToTagId,isMulti)
            websiteModel.getSceneWebsitesForTagger(itemToTagId,isMulti)
        } else if (itemToTagType === "Picture") {
            actorModel.getPictureActorsForTagger(itemToTagId,isMulti)
            tagModel.getPictureTagsForTagger(itemToTagId,isMulti)
            websiteModel.getPictureWebsitesForTagger(itemToTagId,isMulti)
        }else if (itemToTagType === "Website") {
            tagModel.getWebsiteTagsForTagger(itemToTagId,isMulti)
            aliasModel.search(itemToTagId, "Actor")
        }
    }

    function changeIndex(newIndex) {
        console.log("New index is " + newIndex)
        currentItemIndex = newIndex
        switchPage()
    }

    //    QString itemToAddId, QString itemToAddName,
    //                               QString itemToAddTableName,
    //                               QString itemToAddRelationTableName,
    //                               QString itemToAddRelationItemId,
    //                               QString itemToAddType,
    //                               QString itemRelationType
    function addItemHelper(tempModel, selectedItemType, selectedItemName, selectedItemId, selectedItemAliasOfId, itemToAddType) {
        if (selectedItemType.includes("New")) {
            tempModel.addItem("", selectedItemName, itemToAddType,
                              itemToTagType + "_" + itemToAddType, itemToTagId,
                              itemToAddType, itemToTagType)
        } else if (selectedItemType.includes("Alias")) {
            tempModel.addItem(selectedItemAliasOfId, selectedItemName,
                              itemToAddType,
                              itemToTagType + "_" + itemToAddType, itemToTagId,
                              itemToAddType, itemToTagType)
        } else {
            tempModel.addItem(selectedItemId, selectedItemName, itemToAddType,
                              itemToTagType + "_" + itemToAddType, itemToTagId,
                              itemToAddType, itemToTagType)
        }
    }

    function removeItemHelper(tempModel, selectedItemType, selectedItemName, selectedItemId, selectedItemAliasOfId, itemToAddType) {
        tempModel.removeItem(selectedItemId, itemToAddType,
                             itemToTagType + "_" + itemToAddType, itemToTagId,
                             itemToAddType, itemToTagType, false)
    }

    // Adds tag to database...
    function addItem(selectedItemType, selectedItemName, selectedItemId, selectedItemAliasOfId) {
        console.log("TaggerPopup: addItem function triggered")
        console.log("selectedItemType: *" + selectedItemType + "* selectedItemName: *"
                    + selectedItemName + "* selectedItemId: *" + selectedItemId
                    + "* selectedItemAliasOfId: *" + selectedItemAliasOfId + "*")

        var tempModel

        if (selectedItemType.includes("Tag")) {
            tempModel = tagModel
            addItemHelper(tempModel, selectedItemType, selectedItemName,
                          selectedItemId, selectedItemAliasOfId, "Tag")
        } else if (selectedItemType.includes("Actor")) {
            tempModel = actorModel
            addItemHelper(tempModel, selectedItemType, selectedItemName,
                          selectedItemId, selectedItemAliasOfId, "Actor")
        } else if (selectedItemType.includes("Website")) {
            tempModel = websiteModel
            addItemHelper(tempModel, selectedItemType, selectedItemName,
                          selectedItemId, selectedItemAliasOfId, "Website")
        }
    }

    function removeItem(selectedItemType, selectedItemName, selectedItemId, selectedItemAliasOfId) {
        console.log("TaggerPopup: removeItem function triggered")
        console.log("selectedItemType: *" + selectedItemType + "* selectedItemName: *"
                    + selectedItemName + "* selectedItemId: *" + selectedItemId
                    + "* selectedItemAliasOfId: *" + selectedItemAliasOfId + "*")
        var tempModel
        if (selectedItemType.includes("Tag")) {
            tempModel = tagModel
            removeItemHelper(tempModel, selectedItemType, selectedItemName,
                             selectedItemId, selectedItemAliasOfId, "Tag")
        } else if (selectedItemType.includes("Actor")) {
            tempModel = actorModel
            removeItemHelper(tempModel, selectedItemType, selectedItemName,
                             selectedItemId, selectedItemAliasOfId, "Actor")
        } else if (selectedItemType.includes("Website")) {
            tempModel = websiteModel
            removeItemHelper(tempModel, selectedItemType, selectedItemName,
                             selectedItemId, selectedItemAliasOfId, "Website")
        }
    }

    onOpened: {
        tagModel = mainAppPage.createModel("Tag")
        websiteModel = mainAppPage.createModel("Website")
        actorModel = mainAppPage.createModel("Actor")
        aliasModel = mainAppPage.createModel("Alias")
        switchPage()
        contentLoader.source = "TaggerLayout.qml"
        contentLoader.item.state = itemToTagType
    }

    onClosed: {
        //        currentModel.destroy()
        tagModel.destroy()
        websiteModel.destroy()
        actorModel.destroy()
        aliasModel.destroy()
        contentLoader.sourceComponent = undefined
    }

    Shortcut {
        sequence: "Ctrl+Right"
        onActivated: {
            console.log("Sortcut Ctrl+Right was pressed!")
            switchPage(currentItemIndex + 1)
        }
    }

    Shortcut {
        sequence: "Ctrl+Left"
        onActivated: {
            console.log("Sortcut Ctrl+Right was pressed!")
            switchPage(currentItemIndex - 1)
        }
    }

    contentItem: Loader {
        id: contentLoader
        anchors.fill: parent
    }

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
}
