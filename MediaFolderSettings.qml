import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.1

Item {
    anchors.fill: parent
    id: mediaFolderSettingsView
    Component.onCompleted: {
        mediaFolderModel.search("")
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        selectFolder: true
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            selectFolderItemLable.text = fileDialog.fileUrl
            selectFolderItem.folderSelected = true
            //            Qt.quit()
        }
        onRejected: {
            console.log("Canceled")
            //            Qt.quit()
        }
        //        Component.onCompleted: visible = true
    }

    Item{
        id: selectFolderItem

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        height: 50

        property bool folderSelected: false

        Rectangle{
            anchors.fill: parent
            color: Material.color(Material.Grey)

            Label{
                id:selectFolderItemLable
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 10
                font.pixelSize: selectFolderItem.height * 0.3
                text: "Select Folder To Add"
                elide: Text.ElideRight
                width: parent.width * 0.75

            }

            Button{
                id:browseButton

                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter

                visible: !selectFolderItem.folderSelected

                text:"Browse"
                onClicked: fileDialog.visible = true

            }

            Button{
                id:clearSelectedFolderButton
                visible: selectFolderItem.folderSelected

                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter



                text:"Clear"
                onClicked: {
                    selectFolderItemLable.text = "Please Selecte A Folder"
                    selectFolderItem.folderSelected = false
                }

            }

            Button{
                id:addFolderButton
                visible: selectFolderItem.folderSelected

                anchors.right: clearSelectedFolderButton.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter



                text:"Add Folder"
                onClicked: {
                    var isVideoBool = isFolderVideo.checked
                    var isPictureBool = isFolderPicture.checked
                    var pathToAdd = selectFolderItemLable.text.replace(
                                'file:///', '')
                    if (pathToAdd === "") {
                        console.log("Select a folder!")
                    }else if (!(isVideoBool || isPictureBool)){
                        console.log("You have to check either 'Folder is Video' or 'Folder is Picture' or both")
                    }else {
                        console.log(pathToAdd, isPictureBool, isVideoBool)
                        qmlComm.addMediaFolder(pathToAdd, isPictureBool,
                                               isVideoBool)
                        mediaFolderModel.search("")

                        selectFolderItemLable.text = "Please Selecte A Folder"
                        selectFolderItem.folderSelected = false

                    }
                }

            }

            CheckBox {
                id: isFolderPicture
                text: "Is Picture"
                visible: selectFolderItem.folderSelected

                anchors.right: addFolderButton.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter

            }

            CheckBox {
                id: isFolderVideo
                text: "Is Video"
                visible: selectFolderItem.folderSelected

                anchors.right: isFolderPicture.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter

            }




        }



    }

    Component{
        id:mediaFoldersListViewDelegate
        Item{
            id:mediaFoldersListViewDelegateContainer
            width: mediaFoldersListView.width - 5
            height: mediaFoldersListView.height / 12

            Label {
                id: pathLable
                text: (index + 1) + " Path: "
                anchors.left: parent.left
                anchors.margins: 5
                anchors.verticalCenter: parent.verticalCenter
//                font: mediaFolderSettingsView.myFont
            }
            Label {
                id: pathValue
                text: path
                anchors.left: pathLable.right
                anchors.margins: 5
                anchors.verticalCenter: parent.verticalCenter
//                font: mediaFolderSettingsView.myFont
            }

            Button {
                id: scanFolderButton
                text: "Scan"

                anchors.right: parent.right
                anchors.rightMargin: 30
                anchors.verticalCenter: parent.verticalCenter

                onClicked: {
                    qmlComm.scanMediaFolder(path, isVid, isPic)
                }
            }

            CheckBox {
                id: isPictureValue
                text: "Is Picture"
                checked: isPic

                anchors.right: scanFolderButton.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
            }

            CheckBox {
                id: isVideoValue
                text: "Is Video"
                checked: isVid

                anchors.right: isPictureValue.left
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
            }

        }
    }


    Item{
        id:mediaFoldersList
        anchors.top: selectFolderItem.bottom
        anchors.bottom: parent.bottom
//        height: 300
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.topMargin: 10

        ListView{
            id: mediaFoldersListView
            anchors.fill: parent
            model: mediaFolderModel
            clip: true
            delegate: mediaFoldersListViewDelegate



        }
    }

//    ColumnLayout {
//        id: myColumnLayout

//        RowLayout {
//            Text {
//                id: selectFolderLable
//                text: "Select Folder"
//            }
//            Text {
//                id: selectedFolderLable
//                text: qmlComm.getCurrentDateTime()
//            }
//            Button {
//                id: browseButton
//                text: "Browse"
//                onClicked: fileDialog.visible = true
//            }
//        }

//        RowLayout {
//            CheckBox {
//                id: isVideo
//                text: "Is Video?"
//            }

//            CheckBox {
//                id: isPicture
//                text: "Is Picture?"
//            }

//            Button {
//                id: addFolder
//                text: "Add Folder"
//                onClicked: {
//                    var isVideoBool = isVideo.checked
//                    var isPictureBool = isPicture.checked
//                    var pathToAdd = selectedFolderLable.text.replace(
//                                'file:///', '')
//                    if (pathToAdd === "") {
//                        console.log("Select a folder!")
//                    } else {
//                        console.log(pathToAdd, isPictureBool, isVideoBool)
//                        qmlComm.addMediaFolder(pathToAdd, isPictureBool,
//                                               isVideoBool)
//                        mediaFolderModel.search("")
//                    }
//                }
//            }
//        }
//    }
//    Rectangle {
//        id: listviewBackground
//        color: "blue"
//        width: mainAppPage.width
//        height: mainAppPage.height - 300
//        anchors.left: mediaFolderSettingsView.left
//        anchors.right: mediaFolderSettingsView.right
//        anchors.top: myColumnLayout.bottom


//        ListView {
//            id: mediaFoldersView
//            anchors.fill: parent

//            model: mediaFolderModel
//            delegate: Item {
//                height: 50
//                RowLayout {
//                    Text {
//                        id: idLable
//                        text: "ID: "
//                        font: mediaFolderSettingsView.myFont
//                    }
//                    Text {
//                        id: idValue
//                        text: id
//                        font: mediaFolderSettingsView.myFont
//                    }
//                    Text {
//                        id: pathLable
//                        text: "Path: "
//                        font: mediaFolderSettingsView.myFont
//                    }
//                    Text {
//                        id: pathValue
//                        text: path
//                        font: mediaFolderSettingsView.myFont
//                    }

//                    CheckBox {
//                        id: isPictureValue
//                        text: "Is Picture"
//                        checked: isPic
//                    }

//                    CheckBox {
//                        id: isVideoValue
//                        text: "Is Video"
//                        checked: isVid
//                    }

//                    Button {
//                        id: scanFolderButton
//                        text: "Scan"
//                        onClicked: {
//                            qmlComm.scanMediaFolder(path, isVid, isPic)
//                        }
//                    }
//                }
//            }
//        }
//    }
}
