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

        BusyIndicator {
            anchors.centerIn: parent
            running: thumbnail.status === Image.Loading
        }

        Image {
            id: thumbnail

            anchors.fill: parent
            anchors.margins: 0.5

            source: 'file:///' + thumb
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
//                    thumbGridView.ratingPopup.x = ratingButton.x
//                    thumbGridView.ratingPopup.y = ratingButton.y
//                    thumbGridView.ratingPopup.open()
                    AuxFunc.showRatingPopup("Actor",id,rating,ratingButton.x,ratingButton.y,index)

                }
            }

            Connections {
                id: ratingPupupConnection
                target: ratingPopup

                onRated:{
                    console.log("Rated Item type " + ratedItemType + " Rated index " + index)

                    if ((ratedItemType === "Actor") && (index === ratedItemIndex)){
                        ratingButton.iconText = ratedItemRating
                        console.log("Rating now will be: " + ratedItemRating )
                    }


                }
            }
        }

        Text {
            id: nameText
            width: thumbnailBackground.width - 11

            opacity: 1
            text: name
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
            text: "#S:" + numberOfScenes + " #P:" + numberOfPictures
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
                    qmlComm.prepareDetailView(id, "Actor")
                    mainAppPage.changeView("Actor Detail View")
                }
            }

            onClicked: {
                if (mouse.button & Qt.RightButton) {
                    AuxFunc.ctxMenu("Actor", mouseX, mouseY, index)
                } else {
                    console.log("Single Clicked on " + name)
                }
            }
        }
    }
}


//Item{
//    id:singleThumbDelegate
////    height: thumbGridView.cellHeight
////    width: thumbGridView.cellWidth
//    property string imageSource: ""
//    property string actorName: ""
//    property string actorAge: ""
//    property string actorId: ""
//    property string number_of_scenes: ""
//    property string number_of_pictures: ""
//    property string gender: ""
//    property string currentPlaceHolder: "file:///D:/Webstorm/Yapo-Electron/app/media/unknown/unknown female_256.jpg"

//    Image {
//        id: thumbnail
//        source: 'file:///' + thumb_320
//        asynchronous: true
//        fillMode: Image.PreserveAspectCrop
//        width: singleThumbDelegate.width
//        height: singleThumbDelegate.height
//        sourceSize.width: singleThumbDelegate.width
//        sourceSize.height: singleThumbDelegate.height
//        onStatusChanged: {
//                    if (thumbnail.status == Image.Error)
//                        thumbnail.source = currentPlaceHolder

//                }

//    }

//    MouseArea{
//        id: thumbMouseArea
//        anchors.fill: thumbnail
//        acceptedButtons: Qt.LeftButton | Qt.RightButton

//        onDoubleClicked: {
//            if(mouse.button & Qt.RightButton)
//            {

//            }else{
//                qmlComm.prepareDetailView(actorId,"Actor")
//                mainAppPage.changeView("Actor Detail View")
//            }

//        }

//        onClicked: {
//            if(mouse.button & Qt.RightButton)
//            {
//                  AuxFunc.ctxMenu("Actor",mouseX,mouseY,index);

//            }else{
//                console.log("Single Clicked on " + actorName)

//            }

//        }

//    }

//    Rectangle{
//        id: buttonBar
//        width: singleThumbDelegate.width
//        height: singleThumbDelegate.height / 10
//        anchors.bottom: thumb.bottom
//        opacity: 0.7
//        color: "black"
//    }

//    Text {
//        id: lable
//                text: singleThumbDelegate.actorName + " (" + singleThumbDelegate.actorAge + ")"
//                anchors.bottom: buttonBar.top
//                anchors.left: thumb.left
//                anchors.bottomMargin: 15
//                anchors.leftMargin: 5
////                width: thumb.width
//                style: Text.Outline
//                styleColor: "black"
//                font.pixelSize: singleThumbDelegate.width / 14
//                font.bold: true
//                font.capitalization: Font.Capitalize
////                clip: true
//                color: "White"
//        }

//    Text {
//            id: count_info
//            text: "#S:" + singleThumbDelegate.number_of_scenes + "#P:" + singleThumbDelegate.number_of_pictures
////            anchors.top: thumb.top
//            anchors.horizontalCenter: lable.horizontalCenter
//            anchors.top: lable.bottom
////            anchors.topMargin: 5
////            anchors.rightMargin: 5
//            style: Text.Outline
//            styleColor: "black"
//            font.pixelSize: singleThumbDelegate.width / 18
//            font.capitalization: Font.Capitalize
////            clip: true
//            color: "White"

//        }

//}

