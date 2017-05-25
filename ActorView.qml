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

    function openContextMenu(mouseX,mouseY,selectedIndex)
    {

        var myCords = thumbGridView.currentItem.mapToItem(actorView,0,0)
//        console.log("mousex: " + mouseX + " mouseY: " + mouseY + " Selected index is " + selectedIndex + " myCords " + myCords.x + " myCords.y " + myCords.y)
        contextMenu.x = mouseX;
        contextMenu.y = mouseY;
        contextMenu.selectedIndex = selectedIndex
        contextMenu.open();
    }

//    Connections{
//        target: singleActorThumbDelegate
//        onSelected: {
//            console.log("Got selected signal from singleActorThumbDelegate")
//            selected2(selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId,selectedItemIndex)
//        }
//    }

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

    Menu {
            id: contextMenu
            property int selectedIndex: -6
//            y: fileButton.height

            MenuItem {
                text: "Tag..."
                onTriggered: {
                    console.log("Will open tagging popup for actor " + actorModel.directData("name",contextMenu.selectedIndex))
                }
            }

        }


}
