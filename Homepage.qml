import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Dialogs 1.2

Item {
    id: homeScreen
    property var homePath
    property var ffmpegPath
    property var ffprobePath

    function getText(textVar){
        if (textVar === undefined){
            return ""
        }else{
            return textVar
        }
    }

    Component.onCompleted: {
        //        qmlComm.setSetting("general/homepath","D:/YAPOC++")
        homePath = qmlComm.getSetting("general/homepath")
        ffmpegPath = qmlComm.getSetting("ffmpeg/ffmpegpath")
        ffprobePath = qmlComm.getSetting("ffmpeg/ffprobepath")
        console.log("Home path retrived from settings.ini is: " + homePath)
        if (homePath === undefined || ffmpegPath === undefined
                || ffprobePath === undefined) {
            placeholdertext.text = "One or more paths are missing, Please set paths!"
            selectHomePathPopup.visible = true
        } else {
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

    Popup {
        id: selectHomePathPopup
        x: 100
        y: 100
        width: homeScreen.width * 0.85
        height: homeScreen.height * 0.6
        property bool pathWasSelected: false
        modal: true
        focus: true

        FileDialog {
            id: fileDialog
            property string selectType
            title: "Please choose a homefolder path"
            selectFolder: {selectType === "homefolder"}
            onAccepted: {
                console.log("You chose: " + fileDialog.fileUrls)
                var chosenPath = fileDialog.fileUrl.toString()

                if (selectType == "homefolder") {
                    selectHomePathPopupContentLable.text = chosenPath.replace(
                                'file:///', '')
                    selectHomePathPopup.pathWasSelected = true
                } else if (selectType == "ffmpegpath") {
                    selectffmpegpathlable.text = chosenPath.replace('file:///',
                                                                    '')
                    selectHomePathPopup.pathWasSelected = true
                } else if (selectType == "ffprobepath") {
                    selectffprobepathlable.text = chosenPath.replace('file:///', '')
                    selectHomePathPopup.pathWasSelected = true
                }
                onRejected: console.log("Canceled")
            }
        }

        Item {
            id: selectHomePathPopupContent

            anchors.fill: parent

            Item {
                id: firstLineWrapper
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: 40
                width: parent.width

//                    Label {
//                        text: "Home folder wasn't defined: Please select a path for the home folder"
//                        anchors.top: parent.top
//                        anchors.horizontalCenter: parent.horizontalCenter
//                        font.pixelSize: 20
//                    }

                Label{
                    id:homepathLable
                    text: "HomePath"
                    font.pixelSize: 20
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                }

                Label {
                    id: selectHomePathPopupContentLable
                    text:getText(homePath)
                    anchors.left: homepathLable.right
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * 0.75
                }

                Button {
                    text: "Browse"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        fileDialog.selectType = "homefolder"
                        fileDialog.open()
                    }
                }
            }

            Item {
                id: secondLineWrapper
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: firstLineWrapper.bottom
                height: 40

                Label{
                    id:ffmpegPathLable
                    text: "FFMPEG PATH"
                    font.pixelSize: 20
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                }

                Label {
                    id: selectffmpegpathlable
                    text: getText(ffmpegPath)
                    anchors.left: ffmpegPathLable.right
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * 0.75
                }

                Button {
                    text: "Browse"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        fileDialog.selectType = "ffmpegpath"
                        fileDialog.open()
                    }
                }
            }

            Item {

                id: thirdLineWrapper
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: secondLineWrapper.bottom
                height: 40

                Label{
                    id:ffprobePathLable
                    text: "ffprobe PATH"
                    font.pixelSize: 20
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                }

                Label {
                    id: selectffprobepathlable
                    text: getText(ffprobePath)
                    anchors.left: ffprobePathLable.right
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width * 0.75
                }

                Button {
                    text: "Browse"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        fileDialog.selectType = "ffprobepath"
                        fileDialog.open()
                    }
                }
            }

            Button {
                id: acceptButton

                text: "Accept"
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 10

                onClicked: {
                    if (selectHomePathPopupContentLable.text !== ""
                            && selectffprobepathlable.text !== ""
                            && selectffmpegpathlable.text !== "") {
                        console.log("Saving path to settings.ini")
                        qmlComm.setSetting("general/homepath",
                                           selectHomePathPopupContentLable.text)
                        qmlComm.setSetting(
                                    "general/dbpath",
                                    selectHomePathPopupContentLable.text + "/yapoDb.db")
                        qmlComm.setSetting("ffmpeg/ffprobepath",
                                           selectffprobepathlable.text)
                        qmlComm.setSetting("ffmpeg/ffmpegpath",
                                           selectffmpegpathlable.text)

                        qmlComm.init()
                        placeholdertext.text = "Current HomeFolder is: "
                                + selectHomePathPopupContentLable.text
                        selectHomePathPopup.close()
                    }
                }
            }

            Button {
                text: "Cancel"
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
