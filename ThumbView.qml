import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    id: thumbView
    property string thumbViewType: ""
    property alias thumbViewModel: thumbGridView.model
    width: parent.width
    height: parent.height

    GridView{
        id:thumbGridView
        width: parent.width
        height: parent.height
        cellWidth: thumbGridView.width / 6
        cellHeight: cellWidth * 1.5
        delegate: SingleActorThumbDelegate{
            id:singleActorThumbDelegate
            width: thumbGridView.cellWidth
            height: thumbGridView.cellHeight
            actorName: name
            number_of_pictures: numberOfPictures
            number_of_scenes: numberOfScenes
            imageSource: "file:///" + thumb_320
            gender: gender

        }

        clip: true

    }


//    states:[
//        State {
//            name: "PORTRAIT"
//            PropertyChanges {
//                target: thumbGridView ; cellWidth: thumbGridView.width / 6 ; cellHeight: cellWidth * 1.5

//            }

//            PropertyChanges {
//                target: singleThumbDelegate ; lableText: singleThumbDelegate.model.name; countInfoText: numberOfScens + numberOfPictures

//            }


//        },

//            State {
//                name: "LANDSCAPE"
//                PropertyChanges {
//                    target: thumbGridView ; cellWidth: thumbGridView.width / 4 ; cellHeight: cellWidth * 0.5625

//                }


//            }
//        ]

}
