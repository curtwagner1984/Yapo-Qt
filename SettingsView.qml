import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Item {

    TabBar {
        id: bar
        width: parent.width
        TabButton {
            text: qsTr("Media Folders")
        }
        TabButton {
            text: qsTr("Bulk Add")
        }
        TabButton {
            text: qsTr("Activity")
        }
    }

    StackLayout {
        width: parent.width
        anchors.top: bar.bottom
        currentIndex: bar.currentIndex
        MediaFolderSettings {
            id: mediaFolderSettings
        }
        BulkAdd{
            id: bulkAdd
        }

        FFmpegTest{
            id: fftest
        }
    }

}
