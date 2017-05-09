import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Popup {
    id: autoCompletePopup
    property alias listview: autoCompleteListView
    property string searchedText: ""
    property string placeHolderTag: "file:///D:/YAPOC++/resource/unkown/placeHolderTag_64.jpg"
    property string placeHolderSite: "file:///D:/YAPOC++/resource/unkown/placeHolderWebsite_64.jpg"
    signal selected(string selectedItemType, string selectedItemName, string selectedItemId, string selectedItemAliasOfId)
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
            opacity: 0.2
            y: autoCompleteListView.currentItem.y
            z: 1
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

//                    Image {
//                        id: autoCompleteThumb
//                        asynchronous: true
//                        source: 'file:///' + thumb_64
//                        height: 60
//                        fillMode: Image.PreserveAspectFit
//                        anchors.left: autoCompleteDelegateBackground.left
//                        onStatusChanged: {
//                            if (autoCompleteThumb.status === Image.Error)
//                                if (tableName === "Tag") {
//                                    autoCompleteThumb.source = placeHolderTag
//                                } else if (tableName === "Website") {
//                                    autoCompleteThumb.source = placeHolderSite
//                                }
//                        }
//                    }

                    Rectangle{
                        id: autoCompleteThumb
                        height: 60
                        width: 30
                        color: Material.color(Material.Green)
                    }

                    Text{
                        id: firstLetterText
                        text: name.charAt(0)
                        font.pixelSize: 30
                        font.capitalization: Font.AllUppercase
                        anchors.horizontalCenter: autoCompleteThumb.horizontalCenter
                        anchors.verticalCenter: autoCompleteThumb.verticalCenter
                    }

                    Text {
                        id: autoCompleteItemName

                        text: {
                            var result = name.replace(
                                        new RegExp('(' + searchedText + ')',
                                                   'gi'), "<b>$1</b>")
                            if (tableName.indexOf('New') !== -1){
                                result = tableName + " " + result
                            }

                            return result
                        }

                        font.capitalization: Font.Capitalize
                        anchors.left: autoCompleteThumb.right
                        anchors.leftMargin: 10
                        anchors.verticalCenter: autoCompleteDelegateBackground.verticalCenter
                    }

                    Text {
                        id: autoCompleteItemType
                        text: "TYPE: "
                              + tableName /*+ (aliasOf == "na" ? "" : " ALIAS OF :" + aliasOf)*/
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
                            autoCompleteListView.forceActiveFocus()
                            console.log(activeFocus ? "I have active focus!" : "I do not have active focus")
                            autoCompleteListView.currentIndex = index
                        }
                    }
                }
                Keys.onReturnPressed: {
                    var newName = name.replace(/(.*?)( |\.|$)/g, function(txt){return txt.charAt(0).toUpperCase() + txt.substr(1).toLowerCase()})
                    autoCompletePopup.selected(tableName,newName,id,aliasOfId)
                    autoCompletePopup.close()
                }
            }

            highlight: highlightBar
            highlightFollowsCurrentItem: false
            ScrollBar.vertical: ScrollBar {
            }
            focus: false
            clip: true
        }
    }
}
