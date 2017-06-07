import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "qrc:/autoComplete"

Item {
    id: taggerLayout

    Item {
        id: thumbAndNameBackground
        width: parent.width / 3
        height: parent.height
        anchors.left: parent.left
        anchors.top: parent.top

        Rectangle {
            id: imagePlaceHolder
            width: parent.width * 0.8
            height: width * 1.5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 15
            color: "black"

            Image {
                id: thumbnail
                anchors.fill: parent
                anchors.margins: 5
                fillMode: Image.PreserveAspectCrop
                asynchronous: true
                sourceSize.width: width
                sourceSize.height: height
                source: taggerPopup.thumbSrc

                RoundButton {
                    id: nextButton
                    text: "→"
                    height: 50
                    width: 50
                    anchors.top: parent.top
                    anchors.right: parent.right

                    onClicked: {
                        taggerPopup.next()
                    }
                }

                RoundButton {
                    id: prevButton
                    text: "←"
                    height: 50
                    width: 50
                    anchors.top: parent.top
                    anchors.left: parent.left

                    onClicked: {
                        taggerPopup.prev()
                    }
                }
            }
        }

        Text {
            id: selectedName
            width: parent.width - 10
            text: taggerPopup.itemToTagLable
            font.pixelSize: parent.width / 10
            anchors.top: imagePlaceHolder.bottom
            anchors.topMargin: 10
            color: Material.color(Material.Purple)
            anchors.horizontalCenter: parent.horizontalCenter
            style: Text.Outline
            styleColor: "black"
            fontSizeMode: Text.HorizontalFit
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Item {
        id: contentBackgrond
        anchors.left: thumbAndNameBackground.right
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom


        AutoCompleteWithTextBox
        {
            id:autoCompleteWithTextBox
            popupHeight: contentBackgrond.height * 0.75
//            height: parent.width / 14
            height: 50
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5                       
//            z:2





        }

        Connections{
            target: autoCompleteWithTextBox
            onSelected:{
                console.log("TaggerLayout: Selected :" + selectedItemName)
//                selected(selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId)
                if (taggerPopup.itemToTagType === "Actor"){
                    if (selectedItemType === "Tag"){
                        tagModel.addItem(selectedItemId,selectedItemName,"Tag","Actor_Tag",taggerPopup.itemToTagId,"Tag","Actor")
                    }else if (selectedItemType === "New Tag"){
                        tagModel.addItem("",selectedItemName,"Tag","Actor_Tag",taggerPopup.itemToTagId,"Tag","Actor")
                    }
                }else if (taggerPopup.itemToTagType === "Scene"){
                    if (selectedItemType === "Tag"){
                        tagModel.addItem(selectedItemId,selectedItemName,"Tag","Scene_Tag",taggerPopup.itemToTagId,"Tag","Scene")
                    }else if (selectedItemType === "New Tag"){
                        tagModel.addItem("",selectedItemName,"Tag","Scene_Tag",taggerPopup.itemToTagId,"Tag","Scene")
                    }else if (selectedItemType === "Actor"){
                        actorModel.addItem(selectedItemId,selectedItemName,"Actor","Scene_Actor",taggerPopup.itemToTagId,"Actor","Scene")
                    }else if (selectedItemType === "ActorAlias"){
                        actorModel.addItem(selectedItemAliasOfId,selectedItemName,"Actor","Scene_Actor",taggerPopup.itemToTagId,"Actor","Scene")
                    }else if (selectedItemType === "New Actor"){
                        actorModel.addItem("",selectedItemName,"Actor","Scene_Actor",taggerPopup.itemToTagId,"Actor","Scene")
                    }else if (selectedItemType === "Website"){
                        websiteModel.addItem(selectedItemId,selectedItemName,"Website","Scene_Website",taggerPopup.itemToTagId,"Website","Scene")
                    }else if (selectedItemType === "New Website"){
                        websiteModel.addItem("",selectedItemName,"Website","Scene_Website",taggerPopup.itemToTagId,"Website","Scene")
                    }
                }
            }
        }

//        Rectangle {
//            id: searchTextEditPlaceholder
//            //            width: parent.width
//            height: parent.width / 14
//            color: "blue"
//            anchors.top: parent.top
//            anchors.left: parent.left
//            anchors.right: parent.right
//            //            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.margins: 10

//            Text {
//                id: searchTextPlaceholder
//                text: "SearchTextEdit"
//                anchors.centerIn: parent
//            }
//        }

        Loader {
            id: contentLoader
            //            source: "TaggerActor.qml"
            anchors.top: autoCompleteWithTextBox.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 10
        }
    }

    states: [
        State {
            name: "Actor"
            PropertyChanges {
                target: contentLoader
                source: "TaggerActor.qml"
            }
            PropertyChanges {
                target: imagePlaceHolder
                width: parent.width * 0.8
                height: width * 1.5
            }
        },
        State {
            name: "Scene"
            PropertyChanges {
                target: contentLoader
                source: "TaggerScene.qml"
            }
            PropertyChanges {
                target: imagePlaceHolder
                width: parent.width * 0.8
                height: width * 0.5625
            }
        }
    ]
}
