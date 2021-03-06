import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "qrc:/autoComplete"
import "qrc:/general"

Item {
    id: taggerLayout

    ThumbNameAndNextPrevControlls
    {
        id: thumbAndName

        anchors.left: parent.left
        anchors.top: parent.top

        currentModel: taggerPopup.currentModel
        currentIndex: taggerPopup.currentItemIndex


    }

    Connections{
        target: thumbAndName
        onIndexChanged:{
            taggerPopup.changeIndex(newIndex)
        }
    }



    Item {
        id: contentBackgrond
        anchors.left: thumbAndName.right
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom


        AutoCompleteWithTextBox
        {
            id:autoCompleteWithTextBox
            popupHeight: contentBackgrond.height * 0.75
            height: 50
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5                       

        }

        Connections{
            target: autoCompleteWithTextBox
            onSelected:{
                console.log("TaggerLayout: Selected :" + selectedItemName)
                taggerPopup.addItem(selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId)

            }
        }



        Loader {
            id: contentLoader            
            anchors.top: autoCompleteWithTextBox.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 10
        }

        Connections{
            target: contentLoader.item
            ignoreUnknownSignals: true
            onRemoveClicked:{
               taggerPopup.removeItem(selectedItemType, selectedItemName, selectedItemId, selectedItemAliasOfId)
            }
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
                target: thumbAndName
                state: "Actor"
                width: parent.width * 0.4
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
                target: thumbAndName
                state: "Scene"
                width: parent.width * 0.4
                height: width * 0.5625
            }
        },
        State {
            name: "Picture"
            PropertyChanges {
                target: contentLoader
                source: "TaggerPicture.qml"
            }
            PropertyChanges {
                target: thumbAndName
                state: "Picture"
                width: parent.width * 0.3
                height: width * 1.5
            }
        }

    ]
}
