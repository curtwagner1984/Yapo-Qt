import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Item {

//    TabBar {
//        id: bar
//        width: parent.width
//        TabButton {
//            text: qsTr("Media Folders")
//        }
//        TabButton {
//            text: qsTr("Bulk Add")
//        }
//        TabButton {
//            text: qsTr("Activity")
//        }
//    }


    TabView  {
        width: parent.width
//        anchors.top: bar.bottom
//        currentIndex: bar.currentIndex
        Tab{
            title: "Folders"
            MediaFolderSettings {
                id: mediaFolderSettings
            }

        }

        Tab{
            title: "Bulk Add"
            BulkAdd{
                id: bulkAdd
            }

        }

        Tab{
            title: "FF Test"
            FFmpegTest{
                id: fftest
            }

        }

        style: TabViewStyle {
                frameOverlap: 1
                tab: Rectangle {
                    color: styleData.selected ? "steelblue" :"lightsteelblue"
                    border.color:  "steelblue"
                    implicitWidth: Math.max(text.width + 4, 80)
                    implicitHeight: 20
                    radius: 2
                    Text {
                        id: text
                        anchors.centerIn: parent
                        text: styleData.title
                        color: styleData.selected ? "white" : "black"
                    }
                }
                frame: Rectangle { color: "steelblue" }
            }

    }

}
