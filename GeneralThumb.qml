import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Rectangle {
    id: thumbnailBackground
    property string thumbnailSource
    property string lable
    property string numOfScenes
    property string numOfPics
    property string numOfTags
    property string numOfSites
    property string numOfActors
    property string pathToFile
    property string currentRating
    property bool currentSelectedState

    property string currentPlaceHolder : "file:///D:/Webstorm/Yapo-Electron/app/media/unknown/unknown scene_360.jpg"

    property bool isMultiSelectEnabled



    signal ratingClicked(real mouseX, real mouseY)
    signal thumbLeftDoubleClicked()
    signal thumbLeftClicked()
    signal thumbRightClicked(real mouseX, real mouseY)
    signal currentItemChecked(bool checkedState)
    signal playClicked()


    color: "black"

    Component.onCompleted: {
        if (thumbnailSource === "file:///")
        {
            thumbnailSource =  "file:///D:/Webstorm/Yapo-Electron/app/media/unknown/unknown scene_360.jpg"
        }
    }

    CheckBox{
        id:multiSelectCheckbox
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 5
        checked: currentSelectedState
        z:5

        onCheckedChanged: {
            console.log("GeneralThumb: onCheckStateChanged triggered")
            currentItemChecked(multiSelectCheckbox.checked)
        }

    }


    BusyIndicator {
        anchors.centerIn: parent
        running: thumbnail.status === Image.Loading
    }

    Image {
        id: thumbnail

        anchors.fill: parent
        anchors.margins: 0.5

        source: thumbnailBackground.thumbnailSource
        asynchronous: true
        fillMode: Image.PreserveAspectCrop
        sourceSize.height: thumbnail.height
        sourceSize.width: thumbnail.width

//        onStatusChanged: {
//            if (thumbnail.status == Image.Error) {
//                thumbnail.source = currentPlaceHolder
//            }
//        }
    }

    Rectangle {
        id: buttonBarBackground
        color: "black"
        opacity: 0.85
        height: thumbnailBackground.height * 0.08
        width: thumbnailBackground.width
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        ThumbDelegateButtonPlay {
            id: playButton
            height: buttonBarBackground.height
            width: height

            anchors.left: buttonBarBackground.left
            anchors.verticalCenter: buttonBarBackground.verticalCenter

            toolTipText: "Play " + pathToFile

        }

        Connections {
            id: playButtonConnection
            target: playButton

            onButtonClicked: {
                playClicked()
            }
        }



        ThumbDelegateButtonRating {
            id: ratingButton
            height: buttonBarBackground.height
            width: height
            rating: currentRating

            anchors.right: buttonBarBackground.right
            anchors.verticalCenter: buttonBarBackground.verticalCenter

        }

        Connections {
            id: ratingButtonConnection
            target: ratingButton

            onButtonClicked: {
                ratingClicked(ratingButton.x, ratingButton.y)

            }
        }

    }

    Text {
        id: nameText
        width: thumbnailBackground.width - 11

        opacity: 1
        text: thumbnailBackground.lable
        font.pixelSize: thumbnailBackground.height / 20
        color: "White"
        font.capitalization: Font.Capitalize
        style: Text.Outline
        styleColor: "black"
        horizontalAlignment: Text.AlignLeft
        elide: Text.ElideRight

        anchors.left: parent.left
        anchors.bottom: buttonBarBackground.top
        anchors.margins: 10
    }

    Rectangle {
        id: sceneAndImagesInfoBackground
        width: sceneAndImagesInfo.contentWidth + 5
        height: sceneAndImagesInfo.height + 2
        color: Material.color(Material.Red)
        opacity: 0.7

        anchors.right: thumbnailBackground.right
        anchors.top: thumbnailBackground.top
        anchors.margins: 10
        visible: { thumbnailBackground.state === "Actor" || thumbnailBackground.state === "Tag" || thumbnailBackground.state ===  "Website"  }
    }

    Text {
        id: sceneAndImagesInfo
        width: thumbnailBackground.width * 0.75
//        height: thumbnailBackground.height * 0.2

        text: "#S:" + thumbnailBackground.numOfScenes + " #P:" + thumbnailBackground.numOfPics
        //            anchors.centerIn: nameTextBackground
//        font.pixelSize: thumbnailBackground.height / 5
//        fontSizeMode: Text.Fit
        minimumPixelSize: 10
        font.pixelSize: thumbnailBackground.height * 0.04
        elide: Text.ElideRight
        color: "White"
        font.capitalization: Font.Capitalize
        style: Text.Outline
        styleColor: "black"
        horizontalAlignment: Text.AlignRight

        anchors.right:  sceneAndImagesInfoBackground.right
        anchors.top: sceneAndImagesInfoBackground.top
        anchors.margins: 1
    }

    MouseArea {
        id: thumbnailBackgroundMouseArea
        anchors.top: thumbnailBackground.top
        anchors.left: thumbnailBackground.left
        anchors.right: thumbnailBackground.right
        anchors.bottom: buttonBarBackground.top
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onDoubleClicked: {
            if (mouse.button & Qt.RightButton) {

            } else {
//                    qmlComm.prepareDetailView(id, "Actor")
                thumbLeftDoubleClicked();
//                mainAppPage.changeView("Actor Detail View",thumbView.currentModel,index)
            }
        }

        onClicked: {
            if (mouse.button & Qt.RightButton) {
                thumbRightClicked(mouseX, mouseY)
//                AuxFunc.ctxMenu("Actor", mouseX, mouseY, index,thumbView.currentModel)
            } else {
//                console.log("Single Clicked on " + name)
                thumbLeftClicked()
            }
        }
    }

   states: [
       State {
           name: "Actor"
           PropertyChanges {
               target: sceneAndImagesInfo
               text: "#S:" + thumbnailBackground.numOfScenes + " #P:" + thumbnailBackground.numOfPics
               visible: true

           }
       },
       State {
           name: "Scene"
           PropertyChanges {
               target: sceneAndImagesInfo
               visible: false

           }
           PropertyChanges {
               target: buttonBarBackground
               height: thumbnailBackground.height * 0.2

           }
       },
       State {
           name: "Picture"
           PropertyChanges {
               target: sceneAndImagesInfo
               visible: false
           }
           PropertyChanges {
               target: thumbnail
               fillMode: Image.PreserveAspectFit
           }
       },
       State {
           name: "Tag"
           PropertyChanges {
               target: sceneAndImagesInfo
               text: "#S:" + thumbnailBackground.numOfScenes + " #P:" + thumbnailBackground.numOfPics + " #W:" + thumbnailBackground.numOfSites + " #A:" + thumbnailBackground.numOfActors               
               font.pixelSize: thumbnailBackground.height * 0.08
               visible: true

           }
           PropertyChanges {
               target: nameText
               font.pixelSize: thumbnailBackground.height * 0.1


           }
           PropertyChanges {
               target: buttonBarBackground
               height: thumbnailBackground.height * 0.2

           }

       },
       State {
           name: "Website"
           PropertyChanges {
               target: sceneAndImagesInfo
               text: "#S:" + thumbnailBackground.numOfScenes + " #P:" + thumbnailBackground.numOfPics +  " #P:" + thumbnailBackground.numOfTags
               font.pixelSize: thumbnailBackground.height * 0.08
               visible: true

           }
           PropertyChanges {
               target: nameText
               font.pixelSize: thumbnailBackground.height * 0.1


           }
           PropertyChanges {
               target: buttonBarBackground
               height: thumbnailBackground.height * 0.2

           }
       }
   ]
}
