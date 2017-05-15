import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
import "qrc:/settings"
import "qrc:/thumbview"
import "qrc:/actorView"
import "qrc:/sceneView"
import "qrc:/pictureView"
import "qrc:/tagView"
import "qrc:/websiteView"
import "qrc:/autoComplete"

ApplicationWindow {
    width: 1280
    height: 720
    visible: true    
    id: mainAppPage


    property alias sceneViewComp: sceneViewComponenet
    property alias searchBarAlias : searchBar



    Component.onCompleted: {
        mainStack.push(homepagePlaceholder, {
                           objectName: "Home View"
                       })
    }

    function changeView(viewToChangeTo) {
        if (viewToChangeTo === "Scene View") {
            mainStack.push(sceneViewComponenet, {
                               objectName: "Scene View"
                           })
        } else if (viewToChangeTo === "Actor Detail View") {
            mainStack.push(actorDetailViewComponenet, {
                               objectName: "Actor Detail View"
                           })
        }else if (viewToChangeTo === "Tag Detail View") {
            mainStack.push(tagDetailViewComponenet, {
                               objectName: "Tag Detail View"
                           })
        }else if (viewToChangeTo === "Website Detail View") {
            mainStack.push(websiteDetailViewComponenet, {
                               objectName: "Website Detail View"
                           })
        }
    }

    //    Material.theme: Material.Dark
    //    Material.accent: Material.Purple


    Item {
        id: mainItem
        anchors.fill: parent


        RowLayout {
            id: searchBarRow
            anchors.top: parent.top
            anchors.left: mainItem.left
            anchors.right: mainItem.right
            TextField {
                id: searchBar
                placeholderText: "Search..."
                Layout.minimumWidth: 200
                Layout.fillWidth: true
                focus: true
                property string searched_text: searchBar.text
                onTextChanged: {
                    if (mainStack.currentItem.objectName === "Actor View") {
                        console.log("You are in Actor View")
                        qmlComm.actorSearch(searchBar.text)
                    } else if (mainStack.currentItem.objectName === "Scene View") {
                        console.log("You are in Scene View")
                        qmlComm.sceneSearch(searchBar.text)
                    } else if (mainStack.currentItem.objectName === "Picture View") {
                        console.log("You are in Picture View")
                        qmlComm.pictureSearch(searchBar.text)
                    } else if (mainStack.currentItem.objectName === "Tag View") {
                        console.log("You are in Tag View")
                        qmlComm.tagSearch(searchBar.text)
                    } else if (mainStack.currentItem.objectName === "Website View") {
                        console.log("You are in Website View")
                        qmlComm.websiteSearch(searchBar.text)
                    } else {
                        qmlComm.autoCompleteSearch(searchBar.text,"")
                        if (!autocomplete.visible){
                            autocomplete.open()
                        }


                    }

                    console.log("Text changed to: " + searchBar.text)
                }

                Connections{
                    target: autocomplete
                    onSelected: {

                        console.log("Autocomplete selected " + selectedItemName + " Type:" + selectedItemType)

                        if (selectedItemType === "Actor") {
                            qmlComm.prepareDetailView(selectedItemId,"Actor")
                            mainAppPage.changeView("Actor Detail View")
                        } else if (selectedItemType === "ActorAlias") {
                            qmlComm.prepareDetailView(selectedItemAliasOfId,"Actor")
                            mainAppPage.changeView("Actor Detail View")
                        } else if (selectedItemType === "Tag") {
                            qmlComm.prepareDetailView(selectedItemId,"Tag")
                            mainAppPage.changeView("Tag Detail View")
                        }
                    }
                }

                Keys.forwardTo: [autocomplete.listview.currentItem,autocomplete.listview]



            }
            Button {
                id: actorScreen
                text: "Actors"
                onClicked: {
                    mainStack.push(actorViewComponenet, {
                                       objectName: "Actor View"
                                   })
                }
            }
            Button {
                id: sceneScreen
                text: "Scenes"
                onClicked: {
                    mainStack.push(sceneViewComponenet, {
                                       objectName: "Scene View"
                                   })
                }
            }

            Button {
                id: pictureScreen
                text: "Pictures"
                onClicked: {
                    mainStack.push(pictureViewComponenet, {
                                       objectName: "Picture View"
                                   })
                }
            }

            Button {
                id: tagScreen
                text: "Tags"
                onClicked: {
                    mainStack.push(tagViewComponenet, {
                                       objectName: "Tag View"
                                   })
                }
            }

            Button {
                id: websiteScreen
                text: "Websites"
                onClicked: {
                    mainStack.push(websiteViewComponenet, {
                                       objectName: "Website View"
                                   })
                }
            }
            Button {
                id: settings
                text: "Settings"
                onClicked: {
                    mainStack.push(settingsView, {
                                       objectName: "Settings View"
                                   })
                }
            }

            Button {
                id: backButton
                text: "Back " + mainStack.depth
                enabled: mainStack.depth > 0
                onClicked: {
                    console.log("Poping from stack")
                    mainStack.pop()
                    console.log("Stack current depth is " + mainStack.depth)
                }
            }

            Button {
                id: sideBarButton
                text: "Sidebar"
                onClicked: {
                    if (!sideView.visible){
                        sideView.visible = true
                        mainView.width = mainItem.width * (3/4)
                        mainView.anchors.left = sideView.right
                        mainView.anchors.right = sideView.right
                    }else{
                        sideView.visible = false
                        mainView.width = mainItem.width
                        mainView.anchors.left = mainItem.left


                    }



                }
            }

            Button {
                id: setFullScreen
                text: "fullscreen"
                onClicked: {
                    if (mainAppPage.visibility === Window.FullScreen) {
                        mainAppPage.visibility = Window.Windowed
                    } else {
                        mainAppPage.visibility = Window.FullScreen
                    }
                }
            }

            Rectangle {
                id: db_indicator
                width: 30
                height: 30
                color: "red"
            }
        }


        Item{
            id:mainView
            width: parent.width
            height: parent.height
            anchors.top: searchBarRow.bottom
            anchors.bottom: parent.bottom


            StackView {
                id: mainStack
    //            width: parent.width
    //            anchors.top: searchBarRow.bottom
    //            anchors.bottom: mainAppPage.bottom
                anchors.fill: parent
            }

        }

        Item{

            id:sideView
            width: parent.width / 3
            anchors.top: searchBarRow.bottom
            anchors.bottom: parent.bottom
            visible: false

            Text
            {
                id: sidebarText
                text: "Sidebar"
                font.pixelSize: 28
                color: Material.color(Material.Pink)
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }








    Component {
        id: homepagePlaceholder
        Item {

            Rectangle {
                id: homeRect
                width: parent.width / 4
                height: parent.height / 4
                anchors.centerIn: parent
                color: Material.color(Material.Amber)
            }
            Text {
                text: "Homepage"                
                anchors.centerIn: homeRect
                font.pixelSize: homeRect.height / 3
            }
        }
    }

    Component {
        id: settingsView
        SettingsView {
            id: mySettingsView
        }
    }

    Component {
        id: actorViewComponenet
        ActorView {
            width: mainView.width
            height: mainView.height
        }
    }

    Component {
        id: sceneViewComponenet
        SceneView {
            width: mainView.width
            height: mainView.height
        }
    }

    Component {
        id: pictureViewComponenet
        PictureView {
            width: mainView.width
            height: mainView.height
        }
    }

    Component {
        id: tagViewComponenet
        TagView {
            width: mainView.width
            height: mainView.height
        }
    }

    Component {
        id: websiteViewComponenet
        WebsiteView {
            width: mainView.width
            height: mainView.height
        }
    }

    Component {
        id: actorDetailViewComponenet
        ActorDetailView {
            width: mainView.width
            height: mainView.height
        }
    }

    Component {
        id: tagDetailViewComponenet
        TagDetailView {
            width: mainView.width
            height: mainView.height
        }
    }

    Component {
        id: websiteDetailViewComponenet
        WebsiteDetailView {
            width: mainView.width
            height: mainView.height
        }
    }

    AutoCompletePopup{
        id: autocomplete
        x:searchBar.x
        y:searchBar.y - 10
        height: mainAppPage.height / 2
        width: searchBar.width
        searchedText: searchBar.text
    }


}
