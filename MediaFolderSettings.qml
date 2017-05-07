import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.1

Item {
    anchors.fill: parent
    id:mediaFolderSettingsView
    property font myFont: Qt.font({
        pixelSize: 14,
        color: "white"
    });

    Component.onCompleted: {
        mediaFolderModel.search("");
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        selectFolder: true
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            selectedFolderLable.text = fileDialog.fileUrl
//            Qt.quit()
        }
        onRejected: {
            console.log("Canceled")
//            Qt.quit()
        }
//        Component.onCompleted: visible = true
    }

    ColumnLayout{

        RowLayout{
            Text{
                id:selectFolderLable
                text:"Select Folder"
            }
            Text{
                id:selectedFolderLable
                text: qmlComm.getCurrentDateTime();
            }
            Button{
                id:browseButton
                text:"Browse"
                onClicked: fileDialog.visible = true
            }
        }

        RowLayout{
            CheckBox{
                id:isVideo
                text:"Is Video?"
            }

            CheckBox{
                id:isPicture
                text:"Is Picture?"
            }

            Button{
                id:addFolder
                text:"Add Folder"
                onClicked: {
                    var isVideoBool = isVideo.checked
                    var isPictureBool = isPicture.checked
                    var pathToAdd = selectedFolderLable.text.replace('file:///','');
                    if (pathToAdd === ""){
                        console.log("Select a folder!")
                    }else{
                        console.log(pathToAdd,isPictureBool,isVideoBool)
                        qmlComm.addMediaFolder(pathToAdd,isPictureBool,isVideoBool);
                        mediaFolderModel.search("");

                    }



                }
            }
        }

        ListView{
            id:mediaFoldersView
            Layout.fillHeight: true
            height: 600
            model: mediaFolderModel
            delegate: Item{
                height: 50
                RowLayout{
                    Text{
                        id:idLable
                        text: "ID: "
                        font: mediaFolderSettingsView.myFont
                    }
                    Text{
                        id:idValue
                        text: id
                        font: mediaFolderSettingsView.myFont
                    }
                    Text{
                        id:pathLable
                        text: "Path: "
                        font: mediaFolderSettingsView.myFont
                    }
                    Text{
                        id:pathValue
                        text: path
                        font: mediaFolderSettingsView.myFont
                    }

                    CheckBox{
                        id:isPictureValue
                        text:"Is Picture"
                        checked: isPic

                    }

                    CheckBox{
                        id:isVideoValue
                        text: "Is Video"
                        checked: isVid

                    }

                    Button{
                        id:scanFolderButton
                        text: "Scan"
                        onClicked: {
                            qmlComm.scanMediaFolder(path,isVid,isPic);
                        }
                    }

                }

            }

        }

    }




}
