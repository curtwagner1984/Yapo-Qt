import QtQuick 2.0
import QtQuick.Controls 2.1
import "qrc:/thumbview"

Item {
    id:actorView
    height: 400    
    signal selected(string selectedItemType, string selectedItemName, string selectedItemId, string selectedItemAliasOfId, int selectedItemIndex)
    signal selectedOut(string selectedItemType, string selectedItemName, string selectedItemId, string selectedItemAliasOfId, int selectedItemIndex)

    onSelected: {
        console.log("ActorView: Selected signal triggered, itemType: " + selectedItemType + " ItemName " + selectedItemName + " index: " + selectedItemIndex)
        selectedOut(selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId,selectedItemIndex)
    }


//    GridView{
//        id:thumbGridView
//        width: parent.width
//        height: parent.height
//        cellWidth: thumbGridView.width / 6
//        cellHeight: cellWidth * 1.5
//        model: actorModel
//        delegate: SingleActorThumbDelegate{
//            id:singleActorThumbDelegate
//            width: thumbGridView.cellWidth
//            height: thumbGridView.cellHeight
//            actorName: name
//            number_of_pictures: numberOfPictures
//            number_of_scenes: numberOfScenes
//            imageSource: "file:///" + thumb_320
//            gender: gender
//            actorId: id
//            actorAge: age

//        }

//        ScrollBar.vertical: ScrollBar {}

//        clip: true

//    }




}
