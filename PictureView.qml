import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import "qrc:/thumbview"

Item {
    id: pictureView
    height: 400   
    property alias popTest: popup

    //    property alias popTestImgSource: popupImage.source
    Popup {
        id: popup
        property int curIndex: -1
        transformOrigin: Popup.TopLeft
        x: - mainAppPage.x
        y: - mainAppPage.y
        width: Screen.desktopAvailableWidth
        height: Screen.desktopAvailableHeight
        modal: true
//        activeFocus: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        Keys.forwardTo: [popupListView]
        Keys.onPressed: {
            console.log("Some Key pressed " + event.key)
        }

        Component {
            id: highlight
            Rectangle {
                width: mainAppPage.width - 5
                height: mainAppPage.height - 5
                color: "transparent"
                radius: 5
                y: popupListView.currentItem.y
            }
        }

        ListView {
            id: popupListView
            anchors.fill: parent
            cacheBuffer: 1
            model: pictureModel
            currentIndex: popup.curIndex
            highlight: highlight
            //                snapMode: ListView.SnapOneItem
            highlightFollowsCurrentItem: false

            delegate: Item {
                id: popupListViewDelegate
                width: popup.width - 5
                //                    height: popup.height * 2
                height: popup.height * 2
                Text {
                    text: name
                }
                Image {
                    id: popupListViewDelegateImage
                    asynchronous: true
                    source: 'file:///' + thumb
                    sourceSize.width: popupListViewDelegate.width
                    width: popupListViewDelegate.width



                    //                        height: width * 1.5
                    fillMode: Image.PreserveAspectFit
                }
            }
            clip: true

            MouseArea {
                id: popupImageMouseArea
                anchors.fill: parent
                onClicked: {
                    popup.close()
                }
            }
        }

        Button {
            text: "next"
            onClicked: {
                var curIndexx = popupListView.currentIndex
                popup.curIndex = curIndexx + 1
            }
        }
    }

    GridView {
        id: thumbGridView
        width: parent.width
        height: parent.height
        cellWidth: {
            if (thumbGridView.width <= 1850) {
                return thumbGridView.width / 4
            } else {
                return thumbGridView.width / 5
            }
        }

        cellHeight: cellWidth * 1.5
        model: pictureModel
        delegate: SinglePictureThumbDelegate {
            id: singleActorThumbDelegate
            width: thumbGridView.cellWidth
            height: thumbGridView.cellHeight
            pictureName: name
            imageSource: "file:///" + thumb
            picturePath: path
        }

        clip: true
        ScrollBar.vertical: ScrollBar {
        }
    }
}
