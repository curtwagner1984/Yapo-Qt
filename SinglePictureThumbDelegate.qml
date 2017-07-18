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
        state: "Picture"

        thumbnailSource: 'file:///' + thumb
        lable: name
        currentRating: rating
        pathToFile: path
        multiSelect: thumbView.multiSelection
        currentSelectedState: isSelected

    }

    Connections{
        target: thumbScene

        onPlayClicked:
        {
            console.log("Play clicked")
//            mainAppPage.showVideo.setupVideo(path)
        }

        onRatingClicked:
        {
            console.log("Rating clicked")
//            AuxFunc.showRatingPopup("Actor",id,rating,mouseX,mouseY,index)
        }

        onThumbLeftDoubleClicked:
        {
            console.log("Thumb left double clicked")
//            mainAppPage.changeView("Picture Detail View",thumbView.currentModel,index)
        }

        onThumbLeftClicked:
        {
            console.log("Thumb left double clicked")
            AuxFunc.showImage(thumbView.currentModel,index)
        }
        onThumbRightClicked:
        {
            var cords = thumbDelegate.mapToItem(mainItem,0,0);
            AuxFunc.ctxMenu("Picture", cords.x + mouseX, cords.y + mouseY, index,thumbView.currentModel)
        }
        onDeleteButtonClicked:
        {
            console.log("Picture thumb: delete button clicked. Index: " + index)
            thumbView.currentModel.removeItem(index,true)

        }
        onCurrentItemChecked:
        {
            console.log("SinglePictureThumbDelegate: onCurrentItemChecked triggered")
            isSelected = checkedState;
        }

    }
}


