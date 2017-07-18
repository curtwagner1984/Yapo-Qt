import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id:autoCompleteContnetItem

    property alias autoCompleteContnetItemListView: autoCompleteContnetItemListView
    property bool isMultiTag

    focus: true

    onIsMultiTagChanged: {
        console.log("autoCompleteContnetItem IsMultiTagChanged!")
    }

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



    function selected(selectedIndex)
    {

        var currentIndex = selectedIndex;
        var selectedItemType = autoCompleteModel.directData("tableName",currentIndex)
        var selectedItemName = autoCompleteModel.directData("name",currentIndex)
        var selectedItemId = autoCompleteModel.directData("id",currentIndex)
        var selectedItemAliasOfId = autoCompleteModel.directData("aliasOfId",currentIndex)
        autoCompletePopupTwo.selected(selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId)
        console.log("Selected " + autoCompleteModel.directData("name",autoCompleteContnetItemListView.currentIndex))

    }


    Keys.onReturnPressed: {
        if (!isMultiTag){
            selected(autoCompleteContnetItemListView.currentIndex)
            autoCompletePopupTwo.close()
        }else{
            console.log("Enter Pressed while isMutliTag == True")
            var selectedItemsIndcies = autoCompleteModel.getSelectedIndices();
            for (var i = 0 ; i < selectedItemsIndcies.length ; i++){
                console.log("Selected index is " + selectedItemsIndcies[i])
                selected(selectedItemsIndcies[i])
            }
            autoCompletePopupTwo.close()
        }

    }

    Keys.forwardTo: [autoCompleteContnetItemListView]




    ListView{
        id:autoCompleteContnetItemListView
        anchors.fill: parent
        currentIndex: 0
        focus: true
        delegate: Component{
            Loader{
                id:autoCompleteContnetItemListViewLoader
                width: parent.width
                height: parent.width / 8
                source: "AutoCompleteListViewDelegate.qml"
            }
        }

        model: autoCompleteModel


        highlight: highlightBar
        highlightFollowsCurrentItem: false
        ScrollBar.vertical: ScrollBar {
        }

    }

}
