import QtQuick 2.0
import QtQuick.Controls 2.1
import "qrc:/thumbview"

Item {
    id:actorView
    height: 400

    GridView{
        id:thumbGridView
        width: parent.width
        height: parent.height
        cellWidth: thumbGridView.width / 6
        cellHeight: cellWidth * 1.5
        model: actorModel
        delegate: SingleActorThumbDelegate{
            id:singleActorThumbDelegate
            width: thumbGridView.cellWidth
            height: thumbGridView.cellHeight
            actorName: name
            number_of_pictures: numberOfPictures
            number_of_scenes: numberOfScenes
            imageSource: "file:///" + thumb_320
            gender: gender
            actorId: id

        }

        ScrollBar.vertical: ScrollBar {}

        clip: true

    }


}
