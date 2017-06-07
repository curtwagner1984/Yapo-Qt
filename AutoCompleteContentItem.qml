import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id:autoCompleteContnetItem

    property alias autoCompleteContnetItemListView: autoCompleteContnetItemListView

    focus: true

    Component {
        id: highlightBar
        Rectangle {
            width: autoCompleteContnetItemListView.width
            height: parent.width / 8
            color: Material.color(Material.Pink)
            opacity: 0.2
            y: autoCompleteContnetItemListView.currentItem.y
            z: 1
        }
    }


//    Keys.onPressed: {
//        console.log("autoCompleteContnetItem key pressed " + event.key)
//        console.log("autoCompleteContnetItem.currentIdnex =" + autoCompleteContnetItemListView.currentIndex)
//        if (event.key === Qt.Key_Left) {
//            console.log("autoCompleteContnetItem: left pressed");
//            event.accepted = false;
//        }else if (event.key === Qt.Key_Enter) {
//            console.log("autoCompleteContnetItem: enter pressed");
//            event.accepted = false;
//        }else if (event.key === Qt.Key_Up) {
//            console.log("autoCompleteContnetItem: up pressed");
//            autoCompleteContnetItemListView.currentIndex = autoCompleteContnetItemListView.currentIndex - 1
//            event.accepted = false;
//        }else if (event.key === Qt.Key_Down) {
//            console.log("autoCompleteContnetItem: down pressed");
//            autoCompleteContnetItemListView.currentIndex = autoCompleteContnetItemListView.currentIndex + 1
//            event.accepted = false;
//        }else if (event.key === Qt.Key_Right) {
//            console.log("autoCompleteContnetItem: right pressed");
//            event.accepted = false;
//        }
//    }

    Keys.onReturnPressed: {
        console.log("autoCompleteContnetItem: enter pressed on " + autoCompleteContnetItemListView.currentItem.item.name +
                    " current index is" + autoCompleteContnetItemListView.currentIndex);
        var currentIndex = autoCompleteContnetItemListView.currentIndex;
        var selectedItemType = autoCompleteModel.directData("tableName",currentIndex)
        var selectedItemName = autoCompleteModel.directData("name",currentIndex)
        var selectedItemId = autoCompleteModel.directData("id",currentIndex)
        var selectedItemAliasOfId = autoCompleteModel.directData("aliasOfId",currentIndex)
//        signal selected(string selectedItemType, string selectedItemName, string selectedItemId, string selectedItemAliasOfId)
        autoCompletePopupTwo.selected(selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId)
        console.log("Selected " + autoCompleteModel.directData("name",autoCompleteContnetItemListView.currentIndex))
        autoCompletePopupTwo.close()

    }

    Keys.forwardTo: [autoCompleteContnetItemListView]




    ListView{
        id:autoCompleteContnetItemListView
        anchors.fill: parent
        currentIndex: 0
        focus: true

//        Keys.forwardTo: [autoCompleteContnetItemListView.currentItem]
        delegate: Component{
            Loader{
                id:autoCompleteContnetItemListViewLoader
                width: parent.width
                height: parent.width / 8
                source: "AutoCompleteListViewDelegate.qml"
//                focus: true

//                Keys.onPressed: {
//                    if (event.key === Qt.Key_Left) {
//                        console.log("autoCompleteContnetItem.autoCompleteContnetItemListView.autoCompleteContnetItemListViewLoader: left pressed");
//                        event.accepted = false;
//                    }
//                }
            }
        }

//        Keys.onPressed: {
//            if (event.key === Qt.Key_Left) {
//                console.log("autoCompleteContnetItem.autoCompleteContnetItemListView: left pressed");
//                event.accepted = false;
//            }
//        }



        model: autoCompleteModel


        highlight: highlightBar
        highlightFollowsCurrentItem: false
        ScrollBar.vertical: ScrollBar {
        }

    }

}
