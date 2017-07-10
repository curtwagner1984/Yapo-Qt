import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Item {

    property var tabBarModel: ["Folders","Bulk Insert","Other"]

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

    TabBar {
        id: tabBar
        anchors.top: parent.top
        height: 40
        property int currentTabIndex: 0
        width: parent.width

        Repeater {
            model: tabBarModel

            TabButton {
                text: modelData
//                width: Math.max(100, tabBarModel.width / 5)
                onClicked: {
                    console.log("Clicked tab button : " + modelData + " index: " + index )
                    if (index !== tabBar.currentTabIndex){
                        tabBar.currentTabIndex = index;
                        if (index === 0){
                            mainSettingsViewLoader.source = "MediaFolderSettings.qml"
                        }else if (index === 1){
                            mainSettingsViewLoader.source = "BulkAdd.qml"
                        }else if (index === 2){
                            mainSettingsViewLoader.source = "FFmpegTest.qml"
                        }
                    }



                }
            }
        }
    }

    Item{
        id:mainSettingsView
        anchors.top: tabBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Loader{
            id:mainSettingsViewLoader
            anchors.fill: parent
            source : "MediaFolderSettings.qml"


        }
    }


//    TabView  {
//        anchors.top: tabBar.bottom
//        width: parent.width
////        anchors.top: bar.bottom
////        currentIndex: bar.currentIndex
//        Tab{
//            title: "Folders"
//            MediaFolderSettings {
//                id: mediaFolderSettings
//            }

//        }

//        Tab{
//            title: "Bulk Add"
//            BulkAdd{
//                id: bulkAdd
//            }

//        }

//        Tab{
//            title: "FF Test"
//            FFmpegTest{
//                id: fftest
//            }

//        }

//        style: TabViewStyle {
//                frameOverlap: 1
//                tab: Rectangle {
//                    color: styleData.selected ? "steelblue" :"lightsteelblue"
//                    border.color:  "steelblue"
//                    implicitWidth: Math.max(text.width + 4, 80)
//                    implicitHeight: 20
//                    radius: 2
//                    Text {
//                        id: text
//                        anchors.centerIn: parent
//                        text: styleData.title
//                        color: styleData.selected ? "white" : "black"
//                    }
//                }
//                frame: Rectangle { color: "steelblue" }
//            }

//    }

}
