import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id: contentPlaceHolder
    signal removeClicked(string selectedItemType, string selectedItemName, string selectedItemId, string selectedItemAliasOfId)

    Text {
        id: tagLable
        text: "Tags"
        anchors.right: parent.horizontalCenter
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 10
        color: Material.color(Material.BlueGrey)
        style: Text.Outline
        styleColor: "black"
        font.pixelSize: 40
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
        height: 50
    }

    TaggerList {
        id: tagList
        listType: "Tag"
        anchors.right: parent.horizontalCenter
        anchors.left: parent.left
        anchors.top: tagLable.bottom
        anchors.bottom: parent.bottom
        model: tagModel
    }

    Text {
        id: aliasLable
        text: "Alias"
        anchors.left: parent.horizontalCenter
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 10
        color: Material.color(Material.BlueGrey)
        style: Text.Outline
        styleColor: "black"
        font.pixelSize: 40
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
        height: 50
    }

    TaggerList {
        id: aliasList
        listType: "ActorAlias"
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        anchors.top: aliasLable.bottom
        anchors.bottom: parent.bottom
        model: taggerPopup.aliasModel
    }

    Connections {
        target: tagList
        onRemoveClicked: {
            console.log("Remove Clicked selectedItemType " + selectedItemType + " selectedItemName " + selectedItemName + " selectedItemId " +selectedItemId+ " selectedItemAliasOfId" + selectedItemAliasOfId)
            removeClicked(selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId)
        }
    }

    Connections {
        target: aliasList
        onRemoveClicked: {
            console.log("Remove Clicked selectedItemType " + selectedItemType + " selectedItemName " + selectedItemName + " selectedItemId " +selectedItemId+ " selectedItemAliasOfId" + selectedItemAliasOfId)
            removeClicked(selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId)
        }
    }
}
