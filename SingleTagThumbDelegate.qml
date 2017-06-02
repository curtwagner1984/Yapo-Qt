import QtQuick 2.8
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtGraphicalEffects 1.0

import "AuxFunctions.js" as AuxFunc

Item {
    id: thumbDelegate

    Rectangle {
        id: thumbnailBackground
        color: "black"
        anchors.fill: parent

//        BusyIndicator {
//            anchors.centerIn: parent
//            running: thumbnail.status === Image.Loading
//        }

        Rectangle{
            id:thumbPlaceholder
            anchors.fill: parent
            anchors.margins: 0.5
            color: "white"

        }

//        Image {
//            id: thumbnail

//            anchors.fill: parent
//            anchors.margins: 0.5

//            source: 'file:///' + thumb
//            asynchronous: true
//            fillMode: Image.PreserveAspectCrop
//            sourceSize.height: thumbnail.height
//            sourceSize.width: thumbnail.width

//            onStatusChanged: {
//                if (thumbnail.status == Image.Error) {
//                    thumbnail.source = currentPlaceHolder
//                }
//            }
//        }

        Rectangle {
            id: buttonBarBackground
            color: "black"
            opacity: 0.85
            height: thumbnailBackground.height / 6
            width: thumbnailBackground.width
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            ThumbDelegateButton {
                id: ratingButton
                height: buttonBarBackground.height
                width: height

                anchors.right: buttonBarBackground.right
                anchors.verticalCenter: buttonBarBackground.verticalCenter

                iconPath: "file:///D:/YAPOC++/resource/icons/ic_favorite_black_24px.svg"
                iconText: "?"
                backgroundColor: "transparent"
                backgroundHighlight: "white"
                buttonName: "ratingButton"
                overlayColor: Material.color(Material.Red)
            }

            Connections {
                id: ratingButtonConnection
                target: ratingButton

                onButtonClicked: {
                    console.log("Clicked on " + buttonName)

                }
            }
        }

        Text {
            id: nameText
            width: thumbnailBackground.width - 11

            opacity: 1
            text: name
            font.pixelSize: thumbnailBackground.height / 5
            color: "White"
            font.capitalization: Font.Capitalize
            style: Text.Outline
            styleColor: "black"
            horizontalAlignment: Text.AlignHCenter
            fontSizeMode: Text.Fit

            anchors.centerIn: thumbPlaceholder

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
                    qmlComm.prepareDetailView(id, "Scene")
                    mainAppPage.changeView("Scene Detail View")
                }
            }

            onClicked: {
                if (mouse.button & Qt.RightButton) {
                    AuxFunc.ctxMenu("Scene", mouseX, mouseY, index)
                } else {
                    console.log("Single Clicked on " + name)
                    mainAppPage.showVideo.setupVideo(path)
                }
            }
        }
    }
}


//import QtQuick 2.0
//import QtQuick.Layouts 1.1
//import QtQuick.Controls 2.1
//import QtGraphicalEffects 1.0
//import QtQuick.Controls.Material 2.1
//import QtQuick.Controls.Material 2.1


//Item{
//    id:singleThumbDelegate

//    property string imageSource: ""
//    property string tagName: ""
//    property string tagId: ""
//    property string numOfScenes: ""
//    property string numOfPictures: ""
//    property string numOfActors: ""

//    property string currentPlaceHolder: "file:///D:/Webstorm/Yapo-Electron/app/media/unknown/unknown scene_360.jpg"

//    Rectangle{
//        id:background
//        anchors.fill: parent
//        color: Material.color(Material.Grey)

//    }

//    Rectangle{
//        id:thumb
//        anchors.fill: background
//        anchors.margins: 2
//        color: "white"

//    }

////    Image {
////        id: thumb
////        source: singleThumbDelegate.imageSource
////        asynchronous: true
////        fillMode: Image.PreserveAspectCrop
////        width: singleThumbDelegate.width
////        height: singleThumbDelegate.height
////        sourceSize.width: singleThumbDelegate.width
////        sourceSize.height: singleThumbDelegate.height
////        onStatusChanged: {
////                    if (thumb.status == Image.Error)
////                        thumb.source = currentPlaceHolder

////                }

////    }

//    MouseArea{
//        id: thumbMouseArea
//        anchors.fill: thumb
//        onClicked: {
//            qmlComm.prepareDetailView(tagId,"Tag")
//            mainAppPage.changeView("Tag Detail View")
//        }
//    }

//    Rectangle{
//        id: buttonBar
//        width: singleThumbDelegate.width
//        height: singleThumbDelegate.height / 5
//        anchors.bottom: thumb.bottom
//        opacity: 0.7
//        color: "black"
//    }

//    Button{
//        id:playScene
//        text:""
//        height: buttonBar.height - 4
//        implicitWidth: height
//        anchors.left: buttonBar.left
//        anchors.verticalCenter: buttonBar.verticalCenter
//        hoverEnabled: true
//        ToolTip.visible: hovered
//        ToolTip.text: qsTr("Play file " + singleThumbDelegate.scenePath)
//        onClicked: {
//            qmlComm.playScene(singleThumbDelegate.scenePath)
//        }

//        Image {
//                id: image
//                fillMode: Image.PreserveAspectFit
//                anchors.centerIn: parent
//                sourceSize.height: playScene.background.height - 6
//                height: sourceSize.height
//                source: "file:///D:/YAPOC++/resource/icons/ic_play_arrow_black_48dp/web/ic_play_arrow_black_48dp_1x.png"
//            }
//            ColorOverlay {
//                anchors.fill: image
//                source: image
//                // In dark styles background is dark, this makes the image white
//                color: "#ffffffff"
//            }

//    }

//    Item{
//        id:lableContainer
//        width: thumb.width - 10
//        height: thumb.height * (3/4)

//        Text {
//            id: lable
//            text: singleThumbDelegate.tagName
////                    wrapMode: Text.Wrap


////                anchors.bottom: buttonBar.top
////                anchors.left: thumb.left
////                anchors.bottomMargin: 15
////                anchors.leftMargin: 5
//            anchors.centerIn: lableContainer
////                width: thumb.width
//            style: Text.Outline
//            styleColor: "black"
//            font.pixelSize: thumb.height / 8
//            font.bold: true
//            font.capitalization: Font.Capitalize
//            clip: true
//            color: "White"




//            }

//    }




//    Rectangle
//    {
//        id: durationLableBackground
//        anchors.fill: durationLable
//        color: Material.color(Material.Red)

//    }


//    Text {
//            id: durationLable
//            text: "(#S:"+ singleThumbDelegate.numOfScenes + " #A: "  + singleThumbDelegate.numOfActors + "#P: " + singleThumbDelegate.numOfPictures + ")"
////            anchors.top: thumb.top
////            anchors.left: lable.left
////            anchors.top: lable.bottom
//            anchors.horizontalCenter: lableContainer.horizontalCenter
////            anchors.top: lable.bottom
//            anchors.bottom: buttonBar.top
//            anchors.bottomMargin: 10
////            anchors.rightMargin: 5
//            style: Text.Outline
//            styleColor: "black"
//            font.pixelSize: thumb.height / 10
//            font.capitalization: Font.Capitalize
////            clip: true
//            color: "White"

//        }




//}
