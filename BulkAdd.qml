import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.1



Item {
    id:item1


    TextArea {
        id: csvInput
        x: 50
        y: 50
        width: 300
        height: 400
        text: qsTr("")
    }

    ColumnLayout {
        id: columnLayout
        x: 400
        y: 50
        width: 100
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 200
        anchors.top: csvInput.top

        Button {
            id: addMainstream
            text: qsTr("Add Mainstream")
            onClicked: {
                qmlComm.addActors(csvInput.text,1);

            }
        }


        Button {
            id: addAdult
            text: qsTr("Add Adult")
            onClicked: {
                qmlComm.addActors(csvInput.text,0);
            }
        }

        Button {
            id: addTags
            text: qsTr("Add Tags")
            onClicked: {
                qmlComm.addTags(csvInput.text);
            }
        }

        Button {
            id: addWebsite
            text: qsTr("AddWebsite")
            onClicked: {
                qmlComm.addWebsites(csvInput.text);
            }
        }
    }



}
