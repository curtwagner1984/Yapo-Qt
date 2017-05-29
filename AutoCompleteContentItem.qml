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


    Keys.onPressed: {
        if (event.key === Qt.Key_Left) {
            console.log("autoCompleteContnetItem: left pressed");
            event.accepted = false;
        }
    }


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
                focus: true

                Keys.onPressed: {
                    if (event.key === Qt.Key_Left) {
                        console.log("autoCompleteContnetItem.autoCompleteContnetItemListView.autoCompleteContnetItemListViewLoader: left pressed");
                        event.accepted = false;
                    }
                }
            }
        }

        Keys.onPressed: {
            if (event.key === Qt.Key_Left) {
                console.log("autoCompleteContnetItem.autoCompleteContnetItemListView: left pressed");
                event.accepted = false;
            }
        }



        model: autoCompleteModel
        highlight: highlightBar
        highlightFollowsCurrentItem: false
        ScrollBar.vertical: ScrollBar {
        }

    }

}
