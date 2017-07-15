import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "qrc:/general"

Item {
    id: sideBarContent

    Item {
        id: thumbAndNameBackground
        width: parent.width
        height: parent.height
        anchors.left: parent.left
        anchors.top: parent.top

        ThumbNameAndNextPrevControlls {
            id: thumbAndName
            //        width: parent.width / 3
            //        height: parent.height
            anchors.fill: parent

            currentModel: detailView.currentModel
            currentIndex: detailView.currentIndex
        }

        Connections {
            target: thumbAndName
            onIndexChanged: {
                detailView.changeIndex(newIndex)
            }
        }

        Connections{
            target: detailView.currentModel
            onBaseSearchFinished :{
                console.log("Detail view sidebar: deatilView.currentModel onBaseSearchFinished triggered")
                /*
                This might seem redunant but when using a global search, detailView.currentModel doesn't have the time
                to finish it's search before this element is displayed, therefore it doesn't have the information we want.
                So with this slot we update the displayed information when detailView.currentModel finishes it's search.
                */
                thumbAndName.currentModel = detailView.currentModel
                thumbAndName.currentIndex = detailView.currentIndex
            }
        }
    }

    states: [
        State {
        name: "Actor"
        PropertyChanges {
            target: thumbAndName
            state: "Actor"
        }
    },State {
            name: "Scene"
            PropertyChanges {
                target: thumbAndName
                state: "Scene"
            }
        }
    ]
}


