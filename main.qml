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
import "qrc:/taggerPopup"

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

    Shortcut {
        sequence: "Ctrl+E"
        onActivated: {
            console.log("Sortcut Ctrl+E was pressed!")
        }
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
            Timer {
                id: searchTimer
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
//                        qmlComm.autoCompleteSearch(searchBar.text, "")
                        autocomplete.searchedText = searchBar.text
                        autocomplete.searchType = ""
                        if (!autocomplete.isOpened) {
                            autocomplete.open()
                        }else{
                            autocomplete.search()
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
//                  Keys.forwardTo: [autocomplete.contentLoader.item,
//                      autocomplete.contentLoader.item.autoCompleteContnetItemListView,
//                      autocomplete.contentLoader.item.autoCompleteContnetItemListView.currentItem,
//                  autocomplete.contentLoader.item.autoCompleteContnetItemListView.currentItem.item]

                  Keys.forwardTo: [
                      autocomplete.contentLoader.item.autoCompleteContnetItemListView,
                      autocomplete.contentLoader.item.autoCompleteContnetItemListView.currentItem.item]
//                Keys.forwardTo: [autocomplete.listview.currentItem, autocomplete.listview]
            }

            OrderByComboBox {
                id: orderbyComboBox
                Layout.minimumWidth: 200
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
                onCurrentItemChanged: {
                    console.log("Stackview current item changed.. Current item is: "
                                + mainStack.currentItem.objectName)
                    if (mainStack.currentItem.objectName === "Actor View") {
                        orderbyComboBox.state = "actorOrder"
                        orderbyComboBox.visible = true
                    } else if (mainStack.currentItem.objectName === "Picture View") {
                        orderbyComboBox.state = "pictureOrder"
                        orderbyComboBox.visible = true
                    } else if (mainStack.currentItem.objectName === "Scene View") {
                        orderbyComboBox.state = "sceneOrder"
                        orderbyComboBox.visible = true
                    } else {
                        orderbyComboBox.visible = false
                    }
                }
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

            Item {
                anchors.top: moreSidebarText.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                ListView {
                    id: sideviewListView
                    anchors.fill: parent
                    model: tagModel
                    delegate: Text {
                        text: name
                        //                        }
                    }
                }
            }

            Connections {
                target: actorViewComponenetLoader.item
                onSelectedOut: {
                    console.log("main.qml:Sidbare:connections: Selected signal triggered, itemType: "
                                + selectedItemType + " ItemName " + selectedItemName
                                + " index: " + selectedItemIndex)
                    moreSidebarText.text = actorModel.directData(
                                "name",
                                selectedItemIndex) + " Scenes: " + actorModel.directData(
                                "numberOfScenes", selectedItemIndex)
                    qmlComm.getTagsOfItem(selectedItemId, "Actor")
                }
            }
        }


        Menu {
            id: contextMenu
            property int selectedIndex
            property string selectedItemType

            MenuItem {
                text: "Tag..."
                onTriggered: {
                    //                        console.log("Will open tagging popup for actor " + actorModel.directData("name",contextMenu.selectedIndex))
//                    mainAppPage.openTaggerPopup(
//                                actorModel.directData("thumb",
//                                                      contextMenu.selectedIndex),
//                                "Actor",
//                                actorModel.directData("id",
//                                                      contextMenu.selectedIndex),
//                                contextMenu.selectedIndex)
                    tpopup.currentItemIndex = contextMenu.selectedIndex
                    tpopup.itemToTagType = contextMenu.selectedItemType
                    tpopup.open()
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
        width: mainStack.width
        height: mainStack.height
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

            width: mainItem.width
            height: mainItem.height
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
            width: mainItem.width
            height: mainItem.height
        }
    }

    Loader {
        id: tagViewComponenetLoader
    }

    Component {
        id: tagViewComponenet
        TagView {
            width: mainItem.width
            height: mainItem.height
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
            width: mainItem.width
            height: mainItem.height
        }
    }

    Loader {
        id: actorDetailViewComponenetLoader
    }

    Component {
        id: actorDetailViewComponenet
        ActorDetailView {
            width: mainItem.width
            height: mainItem.height
        }
    }

    Loader {
        id: tagDetailViewComponenetLoader
    }

    Component {
        id: tagDetailViewComponenet
        TagDetailView {
            width: mainItem.width
            height: mainItem.height
        }
    }

    Loader {
        id: websiteDetailViewComponenetLoader
    }

    Component {
        id: websiteDetailViewComponenet
        WebsiteDetailView {
            width: mainItem.width
            height: mainItem.height
        }
    }

    Component {
        id: treeFolderViewComponenet
        TreeFolderView {
            width: mainItem.width
            height: mainItem.height
        }
    }

//    AutoCompletePopup {
//        id: autocomplete
//        x: searchBar.x
//        y: searchBar.y + searchBar.height
//        height: mainItem.height / 2
//        width: searchBar.width
//        searchedText: searchBar.text
//    }


    AutoCompletePopupTwo{
        id: autocomplete
        x: searchBar.x
        y: searchBar.y + searchBar.height
        height: mainItem.height * 0.75
        width: searchBar.width
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
//            console.log("Source:" + source.replace(/\{/g,'\\{').replace(/\}/g,'\\}'))
            console.log("Source:" + source)
//            popupVideo.source = source
//            popupVideo.source = source.replace(/\{/g,'\\{').replace(/\}/g,'\\}');
            popupVideo.source = source ;

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
//                playbackRate: 2

                onStatusChanged: {
                    console.log("Status changed triggered in popupVideo, Status is " + status)
                    console.log("onStatusChanged:BufferProgress " + bufferProgress)
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
                onBufferProgressChanged: {
                    console.log("onBufferProgressChanged:BufferProgress " + bufferProgress)
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
            console.log("On Closed Event triggered ...")
            popupContentItemTimer.stop()
            popupVideo.stop()
        }
    }

    function openTaggerPopup(thumbSrc, itemToTagType, itemToTagId, selectedIndex) {
        //        taggerPopupComponentLoader.thumbSrc = 'file:///' + thumbSrc
        //        if (itemToTagType === "Actor") {
        //            tagModel.getActorTagsForTagger(itemToTagId)
        //            taggerPopup.itemToTagId = itemToTagId
        //        }

        //        taggerPopup.open()
        tpopup.thumbSrc = 'file:///' + thumbSrc
        tpopup.currentItemIndex = selectedIndex
        tpopup.itemToTagId = itemToTagId
        tpopup.itemToTagType = itemToTagType
        tpopup.open()
    }

    TaggerPopup {
        id: tpopup

        width: mainItem.width * 0.75
        height: mainItem.height * 0.75
        x: mainItem.width * 0.12
        y: mainItem.height * 0.12
    }

    Popup {
        id: taggerPopup
        property string itemToTagId

        width: mainItem.width / 2
        height: mainItem.height / 2
        x: mainItem.width / 4
        y: mainItem.height / 4

        modal: true

        contentItem: Loader {

            id: taggerPopupComponentLoader
            property string thumbSrc: "-1"
            sourceComponent: taggerContentComponent
        }

        Component {
            id: taggerContentComponent
            Item {
                id: taggerContentComponentItem
                anchors.fill: parent

                Item {
                    id: taggerPopupThumbItem
                    width: parent.width / 3
                    height: parent.height * 0.75
                    anchors.left: parent.left
                    anchors.top: parent.top

                    Image {
                        id: taggerPopupThumb
                        source: taggerPopupComponentLoader.thumbSrc
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectCrop
                    }
                }

                Item {
                    id: taggerTagSpace
                    anchors.left: taggerPopupThumbItem.right
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top

                    TextField {
                        id: taggerSearchBar
                        anchors.left: parent.left
                        width: parent.width * 0.75
                        onTextChanged: {
                            qmlComm.autoCompleteSearch(taggerSearchBar.text,
                                                       "Tag")
                            taggerAutocomplete.open()
                        }

                        Keys.forwardTo: [taggerAutocomplete.listview, taggerAutocomplete.listview.currentItem]
                    }

                    AutoCompletePopup {
                        id: taggerAutocomplete
                        x: 0
                        y: taggerSearchBar.height
                        height: taggerContentComponentItem.height * 0.85
                        width: taggerSearchBar.width
                        searchedText: taggerSearchBar.text
                        z: 10
                    }

                    ListView {
                        id: taggerListview
                        signal removeClicked(string selectedItemType, string selectedItemName, string selectedItemId, string selectedItemAliasOfId)
                        anchors.top: taggerSearchBar.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        model: tagModel
                        delegate: Item {
                            id: tagListViewDelegate
                            height: 50
                            width: taggerListview.width
                            Text {
                                text: (index + 1) + ". " + name
                                font.pixelSize: taggerListview.width / 22
                                anchors.verticalCenter: tagListViewDelegate.verticalCenter
                            }

                            RoundButton {
                                id: removeButton
                                width: 35
                                height: 35
                                text: "X"
                                anchors.right: tagListViewDelegate.right
                                anchors.verticalCenter: tagListViewDelegate.verticalCenter
                                onClicked: {
                                    console.log("Remove Clicked in delegate on " + id + " " + name)
                                    taggerListview.removeClicked("Tag",
                                                                 name, id, "")
                                }
                            }
                        }
                    }

                    Connections {
                        target: taggerAutocomplete
                        onSelected: {
                            console.log("Selected tagger autocomplete " + selectedItemName)
                            tagModel.addTag(selectedItemId, selectedItemName,
                                            "Actor", taggerPopup.itemToTagId)
                        }
                    }
                    Connections {
                        target: taggerListview
                        onRemoveClicked: {
                            console.log("Remove clicked on " + selectedItemId
                                        + " " + selectedItemName)
                            tagModel.removeTag(selectedItemId, "Actor",
                                               taggerPopup.itemToTagId, false)
                        }
                    }
                }
            }
        }
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    }


}
