import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
import QtMultimedia 5.8
import "qrc:/settings"
import "qrc:/thumbview"
import "qrc:/actorView"
import "qrc:/sceneView"
import "qrc:/pictureView"
import "qrc:/tagView"
import "qrc:/websiteView"
import "qrc:/autoComplete"
import "qrc:/treeFolderView"

ApplicationWindow {
    width: 1280
    height: 720
    visible: true
    id: mainAppPage

    property alias sceneViewComp: sceneViewComponenet
    property alias searchBarAlias: searchBar
    property alias showImage: showImagePopup
    property alias showVideo: sceneViewPopup

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
        } else if (viewToChangeTo === "Tag Detail View") {
            mainStack.push(tagDetailViewComponenet, {
                               objectName: "Tag Detail View"
                           })
        } else if (viewToChangeTo === "Website Detail View") {
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
            Timer{
                id:searchTimer
                interval: 250
                onTriggered: {
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
                        //                        autoCompleteComponenetLoader.sourceComponent = autoCompleteComponenet
                        qmlComm.autoCompleteSearch(searchBar.text, "")
                        if (!autocomplete.visible) {
                            autocomplete.open()
                        }
                    }

                    console.log("Text changed to: " + searchBar.text)
                }
            }

            TextField {
                id: searchBar
                placeholderText: "Search..."
                Layout.minimumWidth: 200
                Layout.fillWidth: true
                focus: true
                property string searched_text: searchBar.text
                onTextChanged: {
                    searchTimer.restart()
                }

                Connections {
                    target: autocomplete
                    onSelected: {

                        console.log("Autocomplete selected " + selectedItemName
                                    + " Type:" + selectedItemType)

                        if (selectedItemType === "Actor") {
                            qmlComm.prepareDetailView(selectedItemId, "Actor")
                            mainAppPage.changeView("Actor Detail View")
                        } else if (selectedItemType === "ActorAlias") {
                            qmlComm.prepareDetailView(selectedItemAliasOfId,
                                                      "Actor")
                            mainAppPage.changeView("Actor Detail View")
                        } else if (selectedItemType === "Tag") {
                            qmlComm.prepareDetailView(selectedItemId, "Tag")
                            mainAppPage.changeView("Tag Detail View")
                        }
                    }
                }

                Keys.forwardTo: [autocomplete.listview.currentItem, autocomplete.listview]
            }
            Button {
                id: actorScreen
                text: "Actors"
                onClicked: {
                    actorViewComponenetLoader.source = "/actorView/ActorView.qml"
//                    actorViewComponenetLoader.sourceComponent = actorViewComponenet
//                    actorViewComponenetLoader.width = mainStack.width
//                    actorViewComponenetLoader.height = mainStack.height
                    qmlComm.actorSearch("")
                    mainStack.push(actorViewComponenetLoader, {
                                       objectName: "Actor View"
                                   })
                }
            }
            Button {
                id: sceneScreen
                text: "Scenes"
                onClicked: {
                    sceneViewComponenetLoader.sourceComponent = sceneViewComponenet
                    qmlComm.sceneSearch("")
                    mainStack.push(sceneViewComponenetLoader, {
                                       objectName: "Scene View"
                                   })
                }
            }

            Button {
                id: pictureScreen
                text: "Pictures"
                onClicked: {
                    pictureViewComponenetLoader.sourceComponent = pictureViewComponenet
                    qmlComm.pictureSearch("")
                    mainStack.push(pictureViewComponenetLoader, {
                                       objectName: "Picture View"
                                   })
                }
            }

            Button {
                id: tagScreen
                text: "Tags"
                onClicked: {
                    tagViewComponenetLoader.sourceComponent = tagViewComponenet
                    qmlComm.tagSearch("")
                    mainStack.push(tagViewComponenetLoader, {
                                       objectName: "Tag View"
                                   })
                }
            }

            Button {
                id: websiteScreen
                text: "Websites"
                onClicked: {
                    qmlComm.websiteSearch("")
                    websiteViewComponenetLoader.sourceComponent = websiteViewComponenet

                    mainStack.push(websiteViewComponenetLoader, {
                                       objectName: "Website View"
                                   })
                }
            }

            Button {
                id: treeFolderButton
                text: "Folders"
                onClicked: {
                    console.log("Clicked on folders...")
                    treeFolderViewComponenetLoader.sourceComponent = treeFolderViewComponenet

                    mainStack.push(treeFolderViewComponenetLoader, {
                                       objectName: "Tree Folder View"
                                   })
                }
            }

            Button {
                id: settings
                text: "Settings"
                onClicked: {
                    settingsViewComponenetLoader.sourceComponent = settingsView
                    mainStack.push(settingsViewComponenetLoader, {
                                       objectName: "Settings View"
                                   })
                }
            }

            Button {
                id: sideBarButton
                text: "Sidbar"
                onClicked: {
                    if (!sideView.visible) {
                        sideView.visible = true
                        mainView.width = mainItem.width * (3 / 4)
                        mainView.anchors.left = sideView.right
                        mainView.anchors.right = mainItem.right

                    } else {
                        sideView.visible = false
                        mainView.width = mainItem.width
                        mainView.anchors.left = mainItem.left
                    }
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

        Item {
            id: mainView
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

        Item {

            id: sideView
            width: parent.width / 4
            anchors.top: searchBarRow.bottom
            anchors.bottom: parent.bottom
            visible: false
            property alias sideText: moreSidebarText.text

            Text {
                id: sidebarText
                text: {
                    if (mainStack.currentItem.objectName == "Actor View") {
                        return "Actor View"
                    } else {
                        return "Sidebar"
                    }
                }
                font.pixelSize: 28
                color: Material.color(Material.Pink)
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                id: moreSidebarText
//                text: actorModel.directData("name",1)
                font.pixelSize: 28
                color: Material.color(Material.Pink)
                anchors.top: sidebarText.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Item{
                anchors.top: moreSidebarText.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                ListView{
                    id:sideviewListView
                    anchors.fill: parent
                    model: tagModel
                    delegate:
                        Text{
                            text:name
//                        }
                    }

                }
            }

            Connections{
                target: actorViewComponenetLoader.item
                onSelectedOut : {
                    console.log("main.qml:Sidbare:connections: Selected signal triggered, itemType: " + selectedItemType + " ItemName " + selectedItemName + " index: " + selectedItemIndex)
                    moreSidebarText.text = actorModel.directData("name",selectedItemIndex) + " Scenes: "  + actorModel.directData("numberOfScenes",selectedItemIndex)
                    qmlComm.getTagsOfItem(selectedItemId,"Actor")

                }
            }

        }
    }

    Component {
        id: homepagePlaceholder
        Item {

            Rectangle {
                id: homeRect
                width: {
                    Math.max((parent.width / 4), placeholdertext.width + 20)
                }

                height: parent.height / 4
                anchors.centerIn: parent
                color: Material.color(Material.Amber)
            }
            Text {
                id: placeholdertext
                text: "Homepage Placeholder"
                anchors.centerIn: homeRect
                font.pixelSize: homeRect.height / 3
            }
        }
    }

    Loader {
        id: settingsViewComponenetLoader
    }

    Component {
        id: settingsView
        SettingsView {
            id: mySettingsView
        }
    }

    Loader {
        id: actorViewComponenetLoader
        width:mainStack.width
        height:mainStack.height
        asynchronous: true
        visible: status == Loader.Ready
//        width: mainView.width
//        height: mainView.height
        onProgressChanged: {
            console.log("Actor loader proggres changed to " + progress)
        }
    }

//    Component {
//        id: actorViewComponenet
//        ActorView {
//            id: actorView
//            width: mainView.width
//            height: mainView.height
//        }
//    }

    Loader {
        id: sceneViewComponenetLoader
    }

    Component {
        id: sceneViewComponenet
        SceneView {
            width: mainView.width
            height: mainView.height
        }
    }

    Loader {
        id: pictureViewComponenetLoader
        asynchronous: true
        visible: status == Loader.Ready
    }

    Component {
        id: pictureViewComponenet
        PictureView {
            width: mainView.width
            height: mainView.height
        }
    }

    Loader {
        id: tagViewComponenetLoader
    }

    Component {
        id: tagViewComponenet
        TagView {
            width: mainView.width
            height: mainView.height
        }
    }

    Loader {
        id: websiteViewComponenetLoader
    }

    Loader {
        id: treeFolderViewComponenetLoader
    }

    Component {
        id: websiteViewComponenet
        WebsiteView {
            width: mainView.width
            height: mainView.height
        }
    }

    Loader {
        id: actorDetailViewComponenetLoader
    }

    Component {
        id: actorDetailViewComponenet
        ActorDetailView {
            width: mainView.width
            height: mainView.height
        }
    }

    Loader {
        id: tagDetailViewComponenetLoader
    }

    Component {
        id: tagDetailViewComponenet
        TagDetailView {
            width: mainView.width
            height: mainView.height
        }
    }

    Loader {
        id: websiteDetailViewComponenetLoader
    }

    Component {
        id: websiteDetailViewComponenet
        WebsiteDetailView {
            width: mainView.width
            height: mainView.height
        }
    }

    Component {
        id: treeFolderViewComponenet
        TreeFolderView {
            width: mainView.width
            height: mainView.height
        }
    }

    AutoCompletePopup {
        id: autocomplete
        x: searchBar.x
        y: searchBar.y + searchBar.height
        height: mainItem.height / 2
        width: mainItem.width
        searchedText: searchBar.text
    }

    Popup {

        id: showImagePopup
        property int curIndex: -1
        x: mainItem.x
        y: mainItem.y
        width: mainItem.width
        height: mainItem.height
        modal: true
        onOpened: {
            showImagePopupContentItemComponenetLoader.sourceComponent
                    = showImagePopupContentItemComponenet
        }

        //        activeFocus: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        //        Keys.forwardTo: [showImagePopupListView]
        //        Keys.onPressed: {
        //            console.log("Some Key pressed " + event.key)
        //        }
        Component {
            id: showImagePopupContentItemComponenet

            Item {
                id: showImagePopupContentItem
                anchors.fill: parent

                Component {
                    id: showImagePopupListViewHighlight
                    Rectangle {
                        width: showImagePopupContentItem.width - 5
                        height: showImagePopupContentItem.height - 5
                        color: "transparent"
                        radius: 5
                        y: showImagePopupListView.currentItem.y
                    }
                }

                ListView {
                    id: showImagePopupListView
                    currentIndex: showImagePopup.curIndex
                    highlight: showImagePopupListViewHighlight
                    anchors.fill: parent
                    model: pictureModel
                    clip: true
                    delegate: Item {
                        id: showImagePopupDelegate
                        height: showImagePopupListView.height
                        width: showImagePopupListView.width

                        Rectangle {
                            id: showImagePopupDelegateBackground
                            anchors.fill: showImagePopupDelegate
                            color: Material.color(Material.BlueGrey)
                        }

                        Image {
                            id: showImagePopupDelegateImage
                            asynchronous: true
                            source: 'file:///' + thumb
                            sourceSize.height: parent.height
                            height: parent.height
                            fillMode: Image.PreserveAspectFit
                            anchors.centerIn: showImagePopupDelegateBackground
                        }

                        Text {
                            text: name
                            font.pixelSize: showImagePopupDelegate.height / 15
                            anchors.bottom: showImagePopupDelegateBackground.bottom
                            anchors.bottomMargin: 15
                        }
                    }
                }

                MouseArea {
                    id: popupImageMouseArea
                    anchors.fill: parent
                    onClicked: {
                        showImagePopup.close()
                    }
                }

                Button {
                    text: "next"
                    onClicked: {
                        var curIndexx = showImagePopupListView.currentIndex
                        showImagePopup.curIndex = curIndexx + 1
                    }
                }
            }
        }

        contentItem: Loader {
            id: showImagePopupContentItemComponenetLoader
        }
    }

    Popup {
        id: sceneViewPopup
        property alias video: popupVideo
        property int seekValue: -1
        property int seekSum: -1
        property bool timerIsOn: false

        width: mainItem.width
        height: mainItem.height
        x: mainItem.x
        y: mainItem.y
        focus: true

        function setupVideo(source) {
            sceneViewPopup.open()
            console.log("Source:" + source)
            popupVideo.source = source
            //            popupVideo.play();
        }

        contentItem: Item {
            id: popupContentItem
            anchors.fill: parent

            Timer {
                id: popupContentItemTimer
                interval: 2000
                running: false
                repeat: true
                onTriggered: {

                    console.log("Timer was triggered")
                    if (sceneViewPopup.seekSum > (popupVideo.duration - sceneViewPopup.seekValue)) {
                        popupContentItemTimer.stop()
                        console.log("Last 30 seconds")
                        sceneViewPopup.seekSum = popupVideo.duration - 30000
                        popupVideo.seek(sceneViewPopup.seekSum)
                    } else {
                        console.log("Running is " + running + " Seek sum before adding "
                                    + sceneViewPopup.seekSum)
                        popupVideo.seek(sceneViewPopup.seekSum)
                        sceneViewPopup.seekSum = sceneViewPopup.seekSum + sceneViewPopup.seekValue
                        console.log("Running is " + running + " Seek sum after adding "
                                    + sceneViewPopup.seekSum + " Duration is: "
                                    + popupVideo.duration)
                    }
                }
                onRunningChanged: {
                    sceneViewPopup.timerIsOn = popupContentItemTimer.running
                    console.log("Running Changed to " + popupContentItemTimer.running)
                }
            }

            Video {
                id: popupVideo
                width: sceneViewPopup.width
                height: sceneViewPopup.height
                fillMode: VideoOutput.PreserveAspectFit
                onStatusChanged: {
                    console.log("Status changed triggered in popupVideo, Status is " + status)
                    if (status === MediaPlayer.Loaded
                            && sceneViewPopup.visible) {

                        sceneViewPopup.seekValue = popupVideo.duration / 20
                        sceneViewPopup.seekSum = sceneViewPopup.seekValue
                        console.log("Seek value is: " + sceneViewPopup.seekValue
                                    + " duration is " + popupVideo.duration)
                        if (!(popupVideo.duration < 60000)) {
                            popupContentItemTimer.running = true
                        } else {
                            console.log("This is a short Video")
                        }
                        popupVideo.play()
                    }
                }
                onStopped: {
                    console.log("Playback has stopped")
                    if (sceneViewPopup.visible) {
                        console.log("Popup is visable looping video")
                        sceneViewPopup.seekSum = 0
                        popupVideo.seek(0)
                    } else {
                        console.log("Popup is not visable stopping playback")
                    }
                }
                onPositionChanged: {
                    //                    console.log("Position changed");
                    var ratio1 = popupVideo.position / popupVideo.duration
                    var seekBarX = statusBar.width * ratio1
                    statusBarSeekHead.x = seekBarX
                }

                muted: true
            }

            Item {
                id: statusBar
                width: parent.width
                height: 10
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20

                Rectangle {
                    id: statusBarBackground
                    color: Material.color(Material.Grey)
                    height: 1
                    width: parent.width
                }

                Rectangle {
                    id: statusBarSeekHead
                    color: Material.color(Material.Pink)
                    height: 10
                    width: 5
                    anchors.verticalCenter: statusBarBackground.verticalCenter
                    x: parent.x
                }
            }

            MouseArea {
                id: popupContentItemMouseArea
                anchors.fill: popupContentItem
                onDoubleClicked: {
                    sceneViewPopup.close()
                }

                onClicked: {
                    if (sceneViewPopup.timerIsOn) {
                        popupContentItemTimer.stop()
                    } else {
                        popupContentItemTimer.start()
                    }
                }
            }
        }

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        onClosed: {
            console.log("On Closed Evene triggered ...")
            popupContentItemTimer.stop()
            popupVideo.stop()
        }
    }
}
