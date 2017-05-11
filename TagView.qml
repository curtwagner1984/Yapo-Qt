import QtQuick 2.0
import QtQuick.Controls 2.1
import "qrc:/thumbview"

Item {
    id:sceneView

    GridView{
        id:thumbGridView
        width: parent.width
        height: parent.height
        cellWidth: {
            if (thumbGridView.width <= 1850 ){
                return thumbGridView.width / 4
            }else{
                return thumbGridView.width / 4
            }
        }


        cellHeight: cellWidth * 0.5625
        model: tagModel
        delegate: SingleTagThumbDelegate{
            id:singleActorThumbDelegate
            width: thumbGridView.cellWidth
            height: thumbGridView.cellHeight

            imageSource: "file:///" + thumb
            tagName: name
            numOfScenes: numberOfScenes
            numOfPictures: numberOfPictures
            numOfActors: numberOfActors
            tagId: id



        }

        clip: true
        ScrollBar.vertical: ScrollBar {}

    }


}
