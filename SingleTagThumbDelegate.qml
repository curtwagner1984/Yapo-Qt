import QtQuick 2.8
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtGraphicalEffects 1.0

import "AuxFunctions.js" as AuxFunc

Item {
    id: thumbDelegate

    GeneralThumb{
        id:thumbScene
        anchors.fill: parent
        state: "Tag"

        thumbnailSource: 'file:///' + thumb
        lable: name
        currentRating: rating
        numOfActors: numberOfActors
        numOfScenes: numberOfScenes
        numOfPics: numberOfPictures
        numOfSites: numberOfWebsites



    }

    Connections{
        target: thumbScene

        onPlayClicked:
        {
            mainAppPage.showVideo.setupVideo(path)
        }

        onRatingClicked:
        {
            console.log("Clicked on rating button")
//            AuxFunc.showRatingPopup("Actor",id,rating,mouseX,mouseY,index)
        }

        onThumbLeftDoubleClicked:
        {
            mainAppPage.changeView("Tag Detail View",thumbView.currentModel,index)
        }

        onThumbLeftClicked:
        {
            console.log("Single Clicked on " + name)
        }
        onThumbRightClicked:
        {
            console.log("Right Clicked on " + name)
//            var cords = thumbDelegate.mapToItem(mainItem,0,0);
//            AuxFunc.ctxMenu("Scene", cords.x + mouseX, cords.y + mouseY, index,thumbView.currentModel)
        }

    }
}


