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
        lable: name + " (" + age + ")"
        numOfScenes: numberOfScenes
        numOfPics: numberOfPictures
        currentRating: rating
        currentSelectedState: isSelected
        multiSelect: thumbView.multiSelection
    }

    Connections{
        target: thumbActor
//        mouseX, mouseY are coordinates of the rating button relative to ThumbView.
        onRatingClicked:
        {
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

        onDeleteButtonClicked:
        {
            console.log("Actor thumb: delete button clicked. Index: " + index)
            thumbView.currentModel.removeItem(index,true)

        }


    }
}




