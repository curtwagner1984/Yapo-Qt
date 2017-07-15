import QtQuick 2.8
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtGraphicalEffects 1.0

import "AuxFunctions.js" as AuxFunc

Item {
    id: thumbDelegate

    GeneralThumb{
        id:thumbActor
        state: "Actor"
        anchors.fill: parent

        thumbnailSource: 'file:///' + thumb
        lable: name
        numOfScenes: numberOfScenes
        numOfPics: numberOfPictures
        currentRating: rating
        currentSelectedState: isSelected
    }

    Connections{
        target: thumbActor
        onRatingClicked:
        {
            console.log("Clicked on rating button")
            AuxFunc.showRatingPopup("Actor",id,rating,mouseX,mouseY,index)
        }

        onThumbLeftDoubleClicked:
        {
            mainAppPage.changeView("Actor Detail View",thumbView.currentModel,index,id)
        }

        onThumbLeftClicked:
        {
            console.log("Single Clicked on " + name)
        }
        onThumbRightClicked:
        {
            var cords = thumbDelegate.mapToItem(mainItem,0,0);
            AuxFunc.ctxMenu("Actor", cords.x + mouseX, cords.y + mouseY, index,thumbView.currentModel)
        }

        onCurrentItemChecked:
        {
            console.log("SingleActorThumbDelegate: onCurrentItemChecked triggered")
            isSelected = checkedState;
        }

    }




//    Rectangle {
//        id: thumbnailBackground
//        color: "black"
//        anchors.fill: parent

//        BusyIndicator {
//            anchors.centerIn: parent
//            running: thumbnail.status === Image.Loading
//        }

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

//        Rectangle {
//            id: buttonBarBackground
//            color: "black"
//            opacity: 0.85
//            height: thumbnailBackground.height / 12
//            width: thumbnailBackground.width
//            anchors.left: parent.left
//            anchors.right: parent.right
//            anchors.bottom: parent.bottom

//            ThumbDelegateButton {
//                id: ratingButton
//                height: buttonBarBackground.height
//                width: height

//                anchors.right: buttonBarBackground.right
//                anchors.verticalCenter: buttonBarBackground.verticalCenter

//                iconPath: "file:///D:/YAPOC++/resource/icons/ic_favorite_black_24px.svg"
//                iconText: "?"
//                backgroundColor: "transparent"
//                backgroundHighlight: "white"
//                buttonName: "ratingButton"
//                toolTipText: "Rating"
//                overlayColor: Material.color(Material.Red)
//            }





//            Connections {
//                id: ratingButtonConnection
//                target: ratingButton

//                onButtonClicked: {
//                    console.log("Clicked on " + buttonName)
//                    AuxFunc.showRatingPopup("Actor",id,rating,ratingButton.x,ratingButton.y,index)

//                }
//            }

//            Connections {
//                id: ratingPupupConnection
//                target: ratingPopup

//                onRated:{
//                    console.log("Rated Item type " + ratedItemType + " Rated index " + index)

//                    if ((ratedItemType === "Actor") && (index === ratedItemIndex)){
//                        ratingButton.iconText = ratedItemRating
//                        console.log("Rating now will be: " + ratedItemRating )
//                    }


//                }
//            }
//        }

//        Text {
//            id: nameText
//            width: thumbnailBackground.width - 11

//            opacity: 1
//            text: name
//            font.pixelSize: thumbnailBackground.height / 20
//            color: "White"
//            font.capitalization: Font.Capitalize
//            style: Text.Outline
//            styleColor: "black"
//            horizontalAlignment: Text.AlignLeft

//            anchors.left: parent.left
//            anchors.bottom: buttonBarBackground.top
//            anchors.margins: 10
//        }

//        Rectangle {
//            id: sceneAndImagesInfoBackground
//            width: sceneAndImagesInfo.contentWidth
//            height: sceneAndImagesInfo.contentHeight
//            color: Material.color(Material.Red)
//            opacity: 0.7

//            anchors.right: sceneAndImagesInfo.right
//            anchors.bottom: sceneAndImagesInfo.bottom
//        }

//        Text {
//            id: sceneAndImagesInfo
//            width: thumbnailBackground.width - 11

//            opacity: 1
//            text: "#S:" + numberOfScenes + " #P:" + numberOfPictures
//            //            anchors.centerIn: nameTextBackground
//            font.pixelSize: thumbnailBackground.height / 32
//            color: "White"
//            font.capitalization: Font.Capitalize
//            style: Text.Outline
//            styleColor: "black"
//            horizontalAlignment: Text.AlignRight

//            anchors.right: parent.right
//            anchors.top: thumbnailBackground.top
//            anchors.margins: 10
//        }

//        MouseArea {
//            id: thumbnailBackgroundMouseArea
//            anchors.top: thumbnailBackground.top
//            anchors.left: thumbnailBackground.left
//            anchors.right: thumbnailBackground.right
//            anchors.bottom: buttonBarBackground.top
//            acceptedButtons: Qt.LeftButton | Qt.RightButton

//            onDoubleClicked: {
//                if (mouse.button & Qt.RightButton) {

//                } else {
////                    qmlComm.prepareDetailView(id, "Actor")
//                    mainAppPage.changeView("Actor Detail View",thumbView.currentModel,index)
//                }
//            }

//            onClicked: {
//                if (mouse.button & Qt.RightButton) {
//                    AuxFunc.ctxMenu("Actor", mouseX, mouseY, index,thumbView.currentModel)
//                } else {
//                    console.log("Single Clicked on " + name)
//                }
//            }
//        }
//    }
}




