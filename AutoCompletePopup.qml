import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Popup {
    id: autoCompletePopup
    property alias listview: autoCompleteListView
    property string searchedText: ""
    x: 200
    y: 200
    width: 200
    height: 400
    modal: false
    focus: false
    clip: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside


    Component {
        id: highlightBar
        Rectangle {
            width: autoCompleteListView.width
            height: 60
            color: Material.color(Material.Pink)
            opacity: 0.7
            y: autoCompleteListView.currentItem.y
        }
    }

    Rectangle {
        id: autoCompleteRect
        color: "transparent"
        height: autoCompletePopup.height
        width: autoCompletePopup.width

        ListView {
            id: autoCompleteListView
            anchors.fill: parent
            currentIndex: 0
            model: autoCompleteModel
            delegate: Item {
                id: autoCompleteDelegate
                height: 60
                Rectangle {
                    id: autoCompleteDelegateBackground
                    width: autoCompleteListView.width
                    height: 60
                    focus: true
                    color: "transparent"

                    Image {
                        id: autoCompleteThumb
                        asynchronous: true
                        source: 'file:///' + thumb_64
                        height: 60
                        fillMode: Image.PreserveAspectFit
                        anchors.left: autoCompleteDelegateBackground.left
                    }

                    Text {
                        id: autoCompleteItemName

                        text: {
                            var result = name.replace(new RegExp('(' + searchedText + ')', 'gi'), "<b>$1</b>");
                            return  result
                        }

                        font.capitalization: Font.Capitalize                        
                        anchors.left: autoCompleteThumb.right
                        anchors.leftMargin: 10
                        anchors.verticalCenter: autoCompleteDelegateBackground.verticalCenter
                    }

                    Text {
                        id: autoCompleteItemType
                        text: "TYPE: " + tableName /*+ (aliasOf == "na" ? "" : " ALIAS OF :" + aliasOf)*/
                        font.capitalization: Font.Capitalize
                        anchors.left: autoCompleteItemName.right
                        anchors.leftMargin: 10
                        anchors.verticalCenter: autoCompleteDelegateBackground.verticalCenter
                    }

                    MouseArea {
                        id: autoCompleteMouseArea
                        anchors.fill: autoCompleteDelegateBackground
                        onClicked: {
                            console.log("You clicked " + tableName + " " + name + " " + id)
                            autoCompleteListView.forceActiveFocus();
                            console.log(activeFocus ? "I have active focus!" : "I do not have active focus")
                            autoCompleteListView.currentIndex = index
                        }
                    }






                }
                Keys.onReturnPressed: {
                    console.log("Pressed Enter on" + name + " " + tableName)
                    autoCompletePopup.close()
                }
            }

//            Keys.onPressed: {
//                if (event.key === Qt.Key_Down) {
//                    console.log("AUTOCOMPLETE : Pressed down")
//                } else if (event.key === Qt.Key_Up) {
//                    console.log("AUTOCOMPLETE : Pressed up")
//                } else if (event.key === Qt.Key_Return) {
//                    console.log("AUTOCOMPLETE : Pressed enter")
//                }
//            }

            highlight: highlightBar
            highlightFollowsCurrentItem: false
            ScrollBar.vertical: ScrollBar {
            }
            focus: false
            clip: true
        }
    }
}
