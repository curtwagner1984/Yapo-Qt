import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1


Rectangle {
    id: thumbnailBackground
    color: "black"
    property string thumbnailSource
    property string lable
    property string pathToFile
    property string sceneRating


    signal ratingClicked(real mouseX, real mouseY)
    signal playClicked()
    signal thumbLeftDoubleClicked()
    signal thumbLeftClicked()
    signal thumbRightClicked(real mouseX, real mouseY)

    anchors.fill: parent

    BusyIndicator {
        anchors.centerIn: parent
        running: thumbnail.status === Image.Loading
    }

    Image {
        id: thumbnail

        anchors.fill: parent
        anchors.margins: 0.5

        source: thumbnailSource
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
        height: thumbnailBackground.height / 6
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

            toolTipText: pathToFile

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
            rating: sceneRating

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
        text: lable
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
//                console.log("double clicked")
                thumbLeftDoubleClicked()
            }
        }

        onClicked: {
            if (mouse.button & Qt.RightButton) {
//                console.log("right clicked")
                thumbRightClicked(mouseX,mouseY)
            } else {
//                console.log("Single Clicked on " + lable)
//                    mainAppPage.showVideo.setupVideo(path)
                thumbLeftClicked();
            }
        }
    }
}
