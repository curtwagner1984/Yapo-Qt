import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id: contentPlaceHolder


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
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        anchors.top: aliasLable.bottom
        anchors.bottom: parent.bottom
        model: taggerPopup.aliasModel
    }

    Connections {
        target: tagList
        onRemoveClicked: {
            console.log("Connections: Remove clicked on " + selectedItemId + " " + selectedItemName)
        }
    }

    Connections {
        target: aliasList
        onRemoveClicked: {
            console.log("Connections: Remove clicked on " + selectedItemId + " " + selectedItemName)
        }
    }
}
