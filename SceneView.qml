import QtQuick 2.0
import QtQuick.Controls 2.1
import "qrc:/thumbview"

Item {
    id:sceneView
    height: 400

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
        model: sceneModel
        delegate: SingleSceneThumbDelegate{
            id:singleActorThumbDelegate
            width: thumbGridView.cellWidth
            height: thumbGridView.cellHeight
            sceneName: name
            sceneDuration: duration
            imageSource: "file:///" + thumb
            scenePath: path


        }

        clip: true
        ScrollBar.vertical: ScrollBar {}

    }


}
