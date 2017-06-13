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


