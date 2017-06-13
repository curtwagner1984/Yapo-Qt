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

    function switchPage() {
        itemToTagId = currentModel.directData("id", currentItemIndex)
        thumbSrc = "file:///" + currentModel.directData("thumb",
                                                        currentItemIndex)
        itemToTagLable = currentModel.directData("name", currentItemIndex)

        if (itemToTagType === "Actor") {
            tagModel.getActorTagsForTagger(itemToTagId)
            aliasModel.search(itemToTagId, "Actor")
        } else if (itemToTagType === "Scene") {
            actorModel.getSceneActorsForTagger(itemToTagId)
            tagModel.getSceneTagsForTagger(itemToTagId)
            websiteModel.getSceneWebsitesForTagger(itemToTagId)
        } else if (itemToTagType === "Picture") {
            actorModel.getPictureActorsForTagger(itemToTagId)
            tagModel.getPictureTagsForTagger(itemToTagId)
            websiteModel.getPictureWebsitesForTagger(itemToTagId)
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

    function removeItemHelper (tempModel, selectedItemType, selectedItemName, selectedItemId, selectedItemAliasOfId, itemToAddType) {
        tempModel.removeItem(selectedItemId,itemToAddType,
                          itemToTagType + "_" + itemToAddType, itemToTagId,
                          itemToAddType, itemToTagType,false)
    }

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


    //    if (itemToTagType === "Actor"){

    //        if (selectedItemType.inculdes("Tag"))
    //        {
    //            tempModel = tagModel
    //            addItemHelper(tempModel,selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId,"Tag")
    //        }
    //    }else if (itemToTagType === "Scene"){

    //        if (selectedItemType.inculdes("Tag"))
    //        {
    //            tempModel = tagModel
    //            addItemHelper(tempModel,selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId,"Tag")
    //        }else if (selectedItemType.inculdes("Actor"))
    //        {
    //            tempModel = actorModel
    //            addItemHelper(tempModel,selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId,"Actor")
    //        }else if (selectedItemType.inculdes("Website"))
    //        {
    //            tempModel = websiteModel
    //            addItemHelper(tempModel,selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId,"Website")
    //        }
    //    }else if (itemToTagType === "Picture"){

    //        if (selectedItemType.inculdes("Tag"))
    //        {
    //            tempModel = tagModel
    //            addItemHelper(tempModel,selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId,"Tag")
    //        }else if (selectedItemType.inculdes("Actor"))
    //        {
    //            tempModel = actorModel
    //            addItemHelper(tempModel,selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId,"Actor")
    //        }else if (selectedItemType.inculdes("Website"))
    //        {
    //            tempModel = websiteModel
    //            addItemHelper(tempModel,selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId,"Website")
    //        }
    //    }

    //        if (selectedItemType === "Tag"){
    //            tagModel.addItem(selectedItemId,selectedItemName,"Tag","Actor_Tag",itemToTagId,"Tag","Actor")
    //        }else if (selectedItemType === "New Tag"){
    //            tagModel.addItem("",selectedItemName,"Tag","Actor_Tag",itemToTagId,"Tag","Actor")
    //        }else if (selectedItemType === "TagAlias"){
    //            tagModel.addItem(selectedItemAliasOfId,selectedItemName,"Tag","Actor_Tag",itemToTagId,"Tag","Actor")
    //        }
    //    }else if (itemToTagType === "Scene"){
    //        if (selectedItemType === "Tag"){
    //            tagModel.addItem(selectedItemId,selectedItemName,"Tag","Scene_Tag",itemToTagId,"Tag","Scene")
    //        }else if (selectedItemType === "New Tag"){
    //            tagModel.addItem("",selectedItemName,"Tag","Scene_Tag",itemToTagId,"Tag","Scene")
    //        }else if (selectedItemType === "TagAlias"){
    //            tagModel.addItem(selectedItemAliasOfId,selectedItemName,"Tag","Scene_Tag",itemToTagId,"Tag","Scene")

    //        }else if (selectedItemType === "Actor"){
    //            actorModel.addItem(selectedItemId,selectedItemName,"Actor","Scene_Actor",itemToTagId,"Actor","Scene")
    //        }else if (selectedItemType === "ActorAlias"){
    //            actorModel.addItem(selectedItemAliasOfId,selectedItemName,"Actor","Scene_Actor",itemToTagId,"Actor","Scene")
    //        }else if (selectedItemType === "New Actor"){
    //            actorModel.addItem("",selectedItemName,"Actor","Scene_Actor",itemToTagId,"Actor","Scene")

    //        }else if (selectedItemType === "Website"){
    //            websiteModel.addItem(selectedItemId,selectedItemName,"Website","Scene_Website",itemToTagId,"Website","Scene")
    //        }else if (selectedItemType === "WebsiteAlias"){
    //            websiteModel.addItem(selectedItemAliasOfId,selectedItemName,"Website","Scene_Website",itemToTagId,"Website","Scene")
    //        }else if (selectedItemType === "New Website"){
    //            websiteModel.addItem("",selectedItemName,"Website","Scene_Website",itemToTagId,"Website","Scene")
    //        }
    //    }else if (itemToTagType === "Picture"){
    //        if (selectedItemType === "Tag"){
    //            tagModel.addItem(selectedItemId,selectedItemName,"Tag","Picture_Tag",itemToTagId,"Tag","Picture")
    //        }else if (selectedItemType === "New Tag"){
    //            tagModel.addItem("",selectedItemName,"Tag","Picture_Tag",itemToTagId,"Tag","Picture")
    //        }else if (selectedItemType === "TagAlias"){
    //            tagModel.addItem(selectedItemAliasOfId,selectedItemName,"Tag","Picture_Tag",itemToTagId,"Tag","Picture")

    //        }else if (selectedItemType === "Actor"){
    //            actorModel.addItem(selectedItemId,selectedItemName,"Actor","Picute_Actor",itemToTagId,"Actor","Picture")
    //        }else if (selectedItemType === "ActorAlias"){
    //            actorModel.addItem(selectedItemAliasOfId,selectedItemName,"Actor","Picute_Actor",itemToTagId,"Actor","Picture")
    //        }else if (selectedItemType === "New Actor"){
    //            actorModel.addItem("",selectedItemName,"Actor","Picute_Actor",itemToTagId,"Actor","Picture")

    //        }else if (selectedItemType === "Website"){
    //            websiteModel.addItem(selectedItemId,selectedItemName,"Website","Picute_Website",itemToTagId,"Website","Picture")
    //        }else if (selectedItemType === "WebsiteAlias"){
    //            websiteModel.addItem(selectedItemAliasOfId,selectedItemName,"Website","Picute_Website",itemToTagId,"Website","Picture")
    //        }else if (selectedItemType === "New Website"){
    //            websiteModel.addItem("",selectedItemName,"Website","Picute_Website",itemToTagId,"Website","Picture")
    //        }
    //    }

    //    }
    function removeItem(selectedItemType, selectedItemName, selectedItemId, selectedItemAliasOfId) {
        console.log("TaggerPopup: removeItem function triggered")
        console.log("selectedItemType: *" + selectedItemType + "* selectedItemName: *"
                    + selectedItemName + "* selectedItemId: *" + selectedItemId
                    + "* selectedItemAliasOfId: *" + selectedItemAliasOfId + "*")
        var tempModel;
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
