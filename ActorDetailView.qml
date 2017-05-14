import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3


//import QtQuick.Controls 2.0
//import QtQuick.Layouts 1.3
//import QtQuick.Controls.Material 2.1
//import QtQuick.Dialogs 1.2
//import QtQuick.Window 2.0
import "qrc:/sceneView"
import "qrc:/pictureView"
import "qrc:/tagView"
import "qrc:/autoComplete"

Item {
    id: actorDetailView
    height: 400

    Rectangle {
        id: sideBar
        x: 0
        y: 0
        width: parent.width / 4
        height: parent.height
        color: "transparent"
    }

    Image {
        id: actorThumb
        source: "file:///" + detailObject.getDetailObjectAttrib("thumbnail")
        width: sideBar.width - 100
        height: width * 1.5
        x: 50
        y: 20
        asynchronous: true
    }

    Text {
        id: actorNameLable
        anchors.top: actorThumb.bottom
        anchors.horizontalCenter: actorThumb.horizontalCenter
        text: detailObject.getDetailObjectAttrib("name")
        font.pixelSize: sideBar.width / 10
        color: Material.color(Material.Pink)
    }

    TabBar {
        id: actorTabBar
        width: sideBar.width
        anchors.top: actorNameLable.bottom
        anchors.horizontalCenter: sideBar.horizontalCenter
        TabButton {
            text: qsTr("Stats")
        }
        TabButton {
            text: qsTr("Bio")
        }
        TabButton {
            text: qsTr("Aliases")
        }
        TabButton {
            text: qsTr("Links")
        }
    }

    StackLayout {
        id: sidebarStackedLayout
        width: sideBar.width
        height: 400
        anchors.top: actorTabBar.bottom
        currentIndex: actorTabBar.currentIndex
        Item {
            ColumnLayout {
                Layout.fillWidth: true
                RowLayout {

                    Text {
                        text: "Gender: "
                    }
                    Text {
                        text: detailObject.getDetailObjectAttrib("gender")
                    }
                }
                RowLayout {

                    Text {
                        text: "Birthplace: "
                    }
                    Text {
                        text: detailObject.getDetailObjectAttrib(
                                  "country_of_origin")
                    }
                }
                RowLayout {

                    Text {
                        text: "Birthday: "
                    }
                    Text {
                        text: detailObject.getDetailObjectAttrib("date_of_birth")
                    }
                }
                RowLayout {

                    CheckBox {
                        id: isActorExempt
                        text: "One Word Exempt? "
                        checked: detailObject.getDetailObjectAttrib(
                                     "is_exempt_from_one_word_search") === "0" ? false : true
                        onCheckStateChanged: {
                            detailObject.setDetailObjectAttrib(
                                        "is_exempt_from_one_word_search",
                                        isActorExempt.checked ? "1" : "0")
                        }
                    }
                }
            }
        }

        Item {

            Text {
                width: sideBar.width - 20
                id: bioText
                font.pixelSize: sideBar.width / 20

                text: detailObject.getDetailObjectAttrib("description")
                wrapMode: Text.WordWrap
            }
        }

        Item {
            RowLayout {
                Layout.fillWidth: true
                id: addAliasRow
                Button {
                    text: "Add"
                    onClicked: {
                        if (addAliasTextEdit.visible === false) {
                            addAliasTextEdit.visible = true
                            addAliasTextEdit.focus = true
                            addAliasTextEdit.text = ""
                        } else {
                            if (addAliasTextEdit.text !== "") {
                                console.log("Would try to add alias " + addAliasTextEdit.text
                                            + " To actor id: " + detailObject.getDetailObjectAttrib(
                                                "id"))
                                qmlComm.addAlias(
                                            addAliasTextEdit.text, "Actor",
                                            detailObject.getDetailObjectAttrib(
                                                "id"))
                                qmlComm.aliasSearch(
                                            detailObject.getDetailObjectAttrib(
                                                "id"), "Actor")
                            }
                            addAliasTextEdit.visible = false
                        }
                    }
                }
                TextEdit {
                    id: addAliasTextEdit
                    text: "Alias to add"
                    visible: false
                }
            }

            Rectangle {
                color: "transparent"
                width: parent.width
                height: parent.height - addAliasRow.height
                anchors.top: addAliasRow.bottom

                ListView {
                    model: generalAlias
                    height: sidebarStackedLayout.height
                    delegate: Item {
                        height: 20
                        RowLayout {

                            Text {
                                text: name
                            }

                            CheckBox {
                                id: isExemptCheckBox
                                text: "One Word Exempt? "
                                checked: isExemptFromOneWordSearch                                
                                property bool currentCheckState: isExemptCheckBox.checked
                                onClicked: {
                                    console.log("Checkbox clicked")
                                    isExemptFromOneWordSearch = isExemptCheckBox.checked
                                    if (isExemptCheckBox.checked !== currentCheckState){
                                        console.log("Current check state is diffrent than previouse")
                                        isExemptFromOneWordSearch = isExemptCheckBox.checked
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        Item {
            RowLayout {
                Text {
                    id: imdbLinkLable
                    text: "IMdB: "
                }
                Text {
                    id: imdbLink

                    text: {
                        //                        detailObject.getDetailObjectAttrib("imdb_id") === "" ? "" : '<html><style type="text/css"></style><a href="www.imdb.com/name/"' + detailObject.getDetailObjectAttrib("imdb_id") + '">TMdB</a></html>'
                        '<html><style type="text/css"></style><a href="http://google.com">google</a></html>'
                    }
                    onLinkActivated: Qt.openUrlExternally(
                                         "http://www.stackoverflow.com/")
                }
            }
        }
    }

    Rectangle {
        id: mainview
        anchors.left: sideBar.right
        anchors.top: sideBar.top
        width: (parent.width * 3) / 4
        height: parent.height
        color: "transparent"
    }

    TabBar {
        id: mainViewTabBar
        width: mainview.width
        anchors.top: mainview.top
        anchors.horizontalCenter: mainview.horizontalCenter
        TabButton {
            text: qsTr("Scenes" + " (" + detailObject.getDetailObjectAttrib(
                           "NumberOfScenes") + ")")
            //            onClicked: {
            //                console.log("Clicked Scenes tab button")
            //                mainViewStackLayout.push(mainAppPage.sceneViewComp)
            //            }
        }
        TabButton {
            text: qsTr("Pictures" + " (" + detailObject.getDetailObjectAttrib(
                           "NumberOfPictures") + ")")
        }
        TabButton {
            text: qsTr("Tags" + " (" + detailObject.getDetailObjectAttrib(
                           "NumberOfTags") + ")")
        }
    }

    StackLayout {
        id: mainViewStackLayout
        width: mainViewTabBar.width
        anchors.top: mainViewTabBar.bottom
        anchors.left: sideBar.right
        currentIndex: mainViewTabBar.currentIndex

        SceneView {
            width: mainview.width
            height: mainview.height - mainViewTabBar.height
        }

        PictureView {
            width: mainview.width
            height: mainview.height - mainViewTabBar.height
        }

        Item {
            id: tagViewTab
            focus: true
            Rectangle {
                id: background
                color: Material.color(Material.BlueGrey)
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 20
                height: 60
            }

            Button {
                id: tagAddButton
                text: "Add"
                anchors.left: background.left
                onClicked: {
                    console.log("tagAddButton clicked")
                    addTagTextEdit.visible = true
                    addTagTextEdit.focus = true
                }
            }

            TextField {
                id: addTagTextEdit
                placeholderText: "Search..."
                width: background.width
                anchors.left: tagAddButton.right
                anchors.right: background.right
                anchors.verticalCenter: background.verticalCenter
                anchors.margins: 20

                onTextChanged: {
                    qmlComm.autoCompleteSearch(addTagTextEdit.text,"Tag")
                    if (!autocomplete.visible) {
                        autocomplete.open()
                    }
                }

                Connections{
                    target: autocomplete
                    onSelected: {


//                        var newName = selectedItemName.replace(/\w\S*/g, function(txt){return txt.charAt(0).toUpperCase() + txt.substr(1).toLowerCase()})
                        console.log("Autocomplete selected " + selectedItemName + " Type:" + selectedItemType + "Inside ActorDetailView ")
                        qmlComm.addTag(selectedItemId,selectedItemName,"Actor",detailObject.getDetailObjectAttrib("id"))
                        qmlComm.getTagsOfItem(detailObject.getDetailObjectAttrib("id"),"Actor")

                    }
                }

                Keys.forwardTo: [autocomplete.listview.currentItem,autocomplete.listview]
            }

            TagView {
                anchors.top: background.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                //              width: mainview.width
                //              height: mainview.height - mainViewTabBar.height
            }

            AutoCompletePopup {
                id: autocomplete
                x: background.x
                y: 60
                height: mainAppPage.height / 2
                width: background.width
                searchedText: addTagTextEdit.text
            }
        }
    }
}
