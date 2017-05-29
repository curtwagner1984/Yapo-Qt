import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id: contentPlaceHolder

    Text {
        id: actorLable
        text: "Actors"
        width: parent.width / 3
//        anchors.right: parent.horizontalCenter
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
        id: actorList
        anchors.right: actorLable.right
        anchors.left: actorLable.left
        anchors.top: actorLable.bottom
        anchors.bottom: parent.bottom
        model: actorModel
    }

    Text {
        id: tagLable
        text: "Tags"
        width: parent.width / 3
        anchors.left: actorLable.right
//        anchors.right: parent.right
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
        anchors.right: tagLable.right
        anchors.left: tagLable.left
        anchors.top: tagLable.bottom
        anchors.bottom: parent.bottom
        model: tagModel
    }


    Text {
        id: websiteLable
        text: "Websites"
        width: parent.width / 3
        anchors.left: tagLable.right
//        anchors.right: parent.right
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
        id: websiteList
        anchors.right: websiteLable.right
        anchors.left: websiteLable.left
        anchors.top: websiteLable.bottom
        anchors.bottom: parent.bottom
        model: websiteModel
    }

    Connections {
        target: actorList
        onRemoveClicked: {
            console.log("Connections: Remove clicked on " + selectedItemId + " " + selectedItemName)
        }
    }

    Connections {
        target: tagList
        onRemoveClicked: {
            console.log("Connections: Remove clicked on " + selectedItemId + " " + selectedItemName)
        }
    }
}
