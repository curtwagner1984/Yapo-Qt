import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Dialogs 1.2

Item {
    id:homeScreen

    Component.onCompleted: {
//        qmlComm.setSetting("general/homepath","D:/YAPOC++")
        var homePath = qmlComm.getSetting("general/homepath")
        console.log("Home path retrived from settings.ini is: " + homePath);
        if (homePath === undefined)
        {
            placeholdertext.text = "HomePath is not set, Please set homepath!"
            selectHomePathPopup.visible = true
        }else{
            console.log("Will load/create database in homepath")
            placeholdertext.text = "Current HomeFolder is: " + homePath
            qmlComm.init()
        }
    }

    Rectangle {
        id: homeRect
        width: {
            Math.max((parent.width / 4), placeholdertext.width + 20)
        }

        height: parent.height / 4
        anchors.centerIn: parent
        color: Material.color(Material.Amber)
    }
    Text {
        id: placeholdertext
        text: "Homepage Placeholder"
        anchors.centerIn: homeRect
        font.pixelSize: homeRect.height / 3
    }

    Popup
    {
        id:selectHomePathPopup
        x: 100
        y: 100                
        width: homeScreen.width * 0.85
        height: homeScreen.height * 0.6
        property bool pathWasSelected: false
        modal: true
        focus: true

        FileDialog {
                id: fileDialog
                title: "Please choose a homefolder path"
                selectFolder: true
                onAccepted: {
                    console.log("You chose: " + fileDialog.fileUrls)
                    var chosenPath = fileDialog.fileUrl.toString()
                    selectHomePathPopupContentLable.text = chosenPath.replace('file:///','')
                    selectHomePathPopup.pathWasSelected = true

                }
                onRejected: {
                    console.log("Canceled")
                }
            }

        Item{
            id:selectHomePathPopupContent

            anchors.fill: parent

            Label{
                text: "Home folder wasn't defined: Please select a path for the home folder"
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 20

            }

            Label{
                id:selectHomePathPopupContentLable
                text:""
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width * 0.75
            }

            Button{
                text:"Browse"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                onClicked: fileDialog.open()

            }

            Button{
                id:acceptButton

                text:"Accept"
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 10

                onClicked: {
                    if (selectHomePathPopup.pathWasSelected)
                    {
                        console.log("Saving path to settings.ini")
                        qmlComm.setSetting("general/homepath",selectHomePathPopupContentLable.text)
                        qmlComm.setSetting("general/dbpath",selectHomePathPopupContentLable.text + "/yapoDb.db")
                        qmlComm.init()
                        placeholdertext.text = "Current HomeFolder is: " + selectHomePathPopupContentLable.text
                        selectHomePathPopup.close()
                    }
                }

            }

            Button{
                text:"Cancel"
                anchors.right: acceptButton.left
                anchors.rightMargin: 10
                anchors.verticalCenter: acceptButton.verticalCenter

                onClicked: {
                    Qt.quit()
                }

            }
        }



        closePolicy: Popup.NoAutoClose

        Component.onCompleted: {
            console.log("selectHomePathPopup loaded")
        }
    }
}
