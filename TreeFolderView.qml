import QtQuick 2.5
import QtQuick.Controls 1.4

Item {

    TreeView {
        anchors.fill: parent
//        TableViewColumn {
//            title: "Name"
//            role: "id"
//            width: 300
//        }
        TableViewColumn {
            title: "pathToFolder"
            role: "pathToDir"
            width: 200
        }
        model: treeFolderModel

        itemDelegate: Item {
            Text {
                anchors.verticalCenter: parent.verticalCenter
                color: styleData.textColor
                elide: styleData.elideMode
                text: styleData.value
            }
        }
    }

}
