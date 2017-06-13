import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Rectangle {
    id: thumbnailBackground
    property string thumbnailSource
    property string lable
    property string numOfScenes
    property string numOfPics
    property string actorRating


    signal ratingClicked(real mouseX, real mouseY)
    signal thumbLeftDoubleClicked()
    signal thumbLeftClicked()
    signal thumbRightClicked(real mouseX, real mouseY)


    color: "black"


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

        onStatusChanged: {
            if (thumbnail.status == Image.Error) {
                thumbnail.source = currentPlaceHolder
            }
        }
    }

    Rectangle {
        id: buttonBarBackground
        color: "black"
        opacity: 0.85
        height: thumbnailBackground.height / 12
        width: thumbnailBackground.width
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        ThumbDelegateButtonRating {
            id: ratingButton
            height: buttonBarBackground.height
            width: height

            anchors.right: buttonBarBackground.right
            anchors.verticalCenter: buttonBarBackground.verticalCenter

            rating: thumbnailBackground.actorRating

        }

        Connections {
            id: ratingButtonConnection
            target: ratingButton

            onButtonClicked: {
                console.log("Clicked on " + buttonName)
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

        anchors.left: parent.left
        anchors.bottom: buttonBarBackground.top
        anchors.margins: 10
    }

    Rectangle {
        id: sceneAndImagesInfoBackground
        width: sceneAndImagesInfo.contentWidth
        height: sceneAndImagesInfo.contentHeight
        color: Material.color(Material.Red)
        opacity: 0.7

        anchors.right: sceneAndImagesInfo.right
        anchors.bottom: sceneAndImagesInfo.bottom
    }

    Text {
        id: sceneAndImagesInfo
        width: thumbnailBackground.width - 11

        opacity: 1
        text: "#S:" + thumbnailBackground.numOfScenes + " #P:" + thumbnailBackground.numOfPics
        //            anchors.centerIn: nameTextBackground
        font.pixelSize: thumbnailBackground.height / 32
        color: "White"
        font.capitalization: Font.Capitalize
        style: Text.Outline
        styleColor: "black"
        horizontalAlignment: Text.AlignRight

        anchors.right: parent.right
        anchors.top: thumbnailBackground.top
        anchors.margins: 10
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
}
