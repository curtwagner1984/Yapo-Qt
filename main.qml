import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
import QtMultimedia 5.8
import Qt.labs.settings 1.0


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

import "qrc:/viewers"

import "qrc:/detailView"

import "MainAuxFunc.js" as MainAux

import com.me.qmlcomponents 1.0

ApplicationWindow {
    width: 1280
    height: 720
    visible: true
    id: mainAppPage

    property alias searchBarAlias: searchBar
//    property alias showImage: showImagePopup
    property alias showVideo: sceneViewPopup
    property alias mainStack: mainStack

    Component.onCompleted: {
        mainStack.push(homepagePlaceholder, {
                           objectName: "Home View"
                       })
    }

    Shortcut {
        sequence: "Ctrl+G"
        onActivated: {
            console.log("Sortcut Ctrl+G was pressed!")
            globalSearch.open();
            globalSearch.focus = true;
        }
    }



    function changeView(viewToChangeTo,currentModel,currentIndex) {
        if (viewToChangeTo === "Actor Detail View") {
            var x = detailViewComponent.createObject();
            x.init(currentModel,currentIndex,"Actor")
            mainStack.push(x, {
                               objectName: "Actor Detail View"
                           })
        }else if (viewToChangeTo === "Tag Detail View") {
            var x = detailViewComponent.createObject();
            x.init(currentModel,currentIndex,"Tag")
            mainStack.push(x, {
                               objectName: "Tag Detail View"
                           })
        }else if (viewToChangeTo === "Website Detail View") {
            var x = detailViewComponent.createObject();
            x.init(currentModel,currentIndex,"Website")
            mainStack.push(x, {
                               objectName: "Website Detail View"
                           })
        }else if (viewToChangeTo === "Scene Detail View") {
            var x = detailViewComponent.createObject();
            x.init(currentModel,currentIndex,"Scene")
            mainStack.push(x, {
                               objectName: "Scene Detail View"
                           })
        }
//        } else if (viewToChangeTo === "Actor Detail View") {
//            mainStack.push(detailViewComponent, {
//                               objectName: "Actor Detail View"
//                           })
//        } else if (viewToChangeTo === "Tag Detail View") {
//            mainStack.push(tagDetailViewComponenet, {
//                               objectName: "Tag Detail View"
//                           })
//        } else if (viewToChangeTo === "Website Detail View") {
//            mainStack.push(websiteDetailViewComponenet, {
//                               objectName: "Website Detail View"
//                           })
//        }
    }


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

                    var currentItemName = mainStack.currentItem.objectName
                    if (currentItemName.includes("Main")) {
                        console.log("You are in " + currentItemName + " View")
                        mainStack.currentItem.currentModel.search(
                                    searchBar.text)
                    }else {
                        autocomplete.searchedText = searchBar.text
                        autocomplete.searchType = ""
                        if (!autocomplete.isOpened) {
                            autocomplete.open()
                        } else {
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

                          MainAux.autoCompleteSelected(selectedItemType,selectedItemName, selectedItemId, selectedItemAliasOfId)

//                        console.log("Autocomplete selected " + selectedItemName
//                                    + " Type:" + selectedItemType)

//                        if (selectedItemType === "Actor") {
//                              var actModel = createModel("Actor")
//                              actModel.searchById(selectedItemId)
//                              mainAppPage.changeView("Actor Detail View",actModel,0)
//                        } else if (selectedItemType === "ActorAlias") {
//                            var actModel = createModel("Actor")
//                            actModel.searchById(selectedItemAliasOfId)
//                            mainAppPage.changeView("Actor Detail View",actModel,0)
//                        } else if (selectedItemType === "Tag") {
//                            var tagModel = createModel("Tag")
//                            tagModel.searchById(selectedItemId)
//                            mainAppPage.changeView("Tag Detail View",tagModel,0)
//                        } else if (selectedItemType === "TagAlias") {
//                            var tagModel = createModel("Tag")
//                            tagModel.searchById(selectedItemAliasOfId)
//                            mainAppPage.changeView("Tag Detail View",tagModel,0)
//                        }else if (selectedItemType === "Website") {
//                            var websiteModel = createModel("Website")
//                            websiteModel.searchById(selectedItemId)
//                            mainAppPage.changeView("Website Detail View",websiteModel,0)
//                        }else if (selectedItemType === "WebsiteAlias") {
//                            var websiteModel = createModel("Website")
//                            websiteModel.searchById(selectedItemAliasOfId)
//                            mainAppPage.changeView("Website Detail View",websiteModel,0)
//                        }
                    }
                }

                  Keys.forwardTo: [autocomplete.contentLoader.item]

            }

            OrderByComboBox {
                id: orderbyComboBox
                Layout.minimumWidth: 200
            }

            Connections{
                target:orderbyComboBox
                onSelected:{
                    console.log("Selected order" + orderBy + " "  + orderDirection)
                    mainStack.currentItem.currentModel.setOrder(orderBy,orderDirection)
                }
            }

            Button {
                id: actorScreen
                text: "Actors"
                onClicked: {

                    var view = createThumbView("Actor")
                    view.currentModel.search("")

                    mainStack.push(view, {
                                       objectName: "Main Actor View"
                                   })

                }
            }
            Button {
                id: sceneScreen
                text: "Scenes"
                onClicked: {
                    var view = createThumbView("Scene")
                    view.currentModel.search("")

                    mainStack.push(view, {
                                       objectName: "Main Scene View"
                                   })
                }
            }

            Button {
                id: pictureScreen
                text: "Pictures"
                onClicked: {
                    var view = createThumbView("Picture")
                    view.currentModel.search("")

                    mainStack.push(view, {
                                       objectName: "Main Picture View"
                                   })
                }
            }

            Button {
                id: tagScreen
                text: "Tags"
                onClicked: {
                    var view = createThumbView("Tag")
                    view.currentModel.search("")
                    mainStack.push(view, {
                                       objectName: "Main Tag View"
                                   })
                }
            }

            Button {
                id: websiteScreen
                text: "Websites"
                onClicked: {
                    var view = createThumbView("Website")
                    view.currentModel.search("")

                    mainStack.push(view, {
                                       objectName: "Main Website View"
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
                onClicked:
                    globalSearch.open()
            }

            Button {
                id: backButton
                text: "Back " + mainStack.depth
                enabled: mainStack.depth > 0
                onClicked: {
                    console.log("Poping from stack")

                    var currentItemName = mainStack.currentItem.objectName

                    if (currentItemName.includes("Main")) {
                        console.log("Deleting current main view")
                        var x = mainStack.currentItem
                        x.currentModel.destroy()
                        x.destroy()
                    }
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
                anchors.fill: parent
                onCurrentItemChanged: {
                    console.log("Stackview current item changed.. Current item is: "
                                + mainStack.currentItem.objectName)
                    if (mainStack.currentItem.objectName === "Main Actor View") {
                        orderbyComboBox.state = "actorOrder"
                        orderbyComboBox.visible = true
                    } else if (mainStack.currentItem.objectName === "Main Picture View") {
                        orderbyComboBox.state = "pictureOrder"
                        orderbyComboBox.visible = true
                    } else if (mainStack.currentItem.objectName === "Main Scene View") {
                        orderbyComboBox.state = "sceneOrder"
                        orderbyComboBox.visible = true
                    } else {
                        orderbyComboBox.visible = false
                    }
                }
            }
        }

        Menu {
            id: contextMenu
            property int selectedIndex
            property string selectedItemType
            property var currentModel

            MenuItem {
                text: "Tag..."
                onTriggered: {
                    tpopup.currentItemIndex = contextMenu.selectedIndex
                    tpopup.itemToTagType = contextMenu.selectedItemType
                    tpopup.currentModel = contextMenu.currentModel
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

    Component{
        id:detailViewComponent
        DetailView{
//            height: mainItem.height
//            width: mainItem.width
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

    function createThumbView(modelType) {

        var thumbView = thumbViewComp.createObject();
        var modelToInsert = createModel(modelType);


        thumbView.currentModel = modelToInsert
        thumbView.state = modelType

        return thumbView
    }

    function createModel(modelType){
        var modelToInsert;

        if (modelType === "Actor") {
            modelToInsert = actorModelComponent.createObject()
        } else if (modelType === "Tag") {
            modelToInsert = tagModelComponent.createObject()
        } else if (modelType === "Website") {
            modelToInsert = websiteModelComponent.createObject()
        } else if (modelType === "Scene") {
            modelToInsert = sceneModelComponent.createObject()
        } else if (modelType === "Picture") {
            modelToInsert = pictureModelComponent.createObject()
        }else if (modelType === "Alias") {
            modelToInsert = aliasModelComponent.createObject()
        } else {
            return null
        }

        return modelToInsert;
    }


    Component {
        id: aliasModelComponent
        GeneralAlias {
            Component.onCompleted: {
                console.log("GeneralAliasModel:ON COMPLETE TRIGGERED")
                init(dbManager)
            }
        }
    }

    Component {
        id: actorModelComponent
        ActorModel {
            Component.onCompleted: {
                console.log("ACTORMODEL:ON COMPLETE TRIGGERED")
                init(dbManager)
            }
        }
    }

    Component {
        id: tagModelComponent
        TagModel {
            Component.onCompleted: {
                console.log("TAGMODEL:ON COMPLETE TRIGGERED")
                init(dbManager)
            }
        }
    }

    Component {
        id: websiteModelComponent
        WebsiteModel {
            Component.onCompleted: {
                console.log("WebsiteModel:ON COMPLETE TRIGGERED")
                init(dbManager)
            }
        }
    }

    Component {
        id: sceneModelComponent
        SceneModel {
            Component.onCompleted: {
                console.log("SceneModel:ON COMPLETE TRIGGERED")
                init(dbManager)
            }
        }
    }

    Component {
        id: pictureModelComponent
        PictureModel {
            Component.onCompleted: {
                console.log("PictureModel:ON COMPLETE TRIGGERED")
                init(dbManager)
            }
        }
    }

    Component {
        id: thumbViewComp
        ThumbView {
            //            width: mainStack.width
            //            height: mainStack.height
        }
    }


    AutoCompletePopupTwo {
        id: autocomplete
        x: searchBar.x
        y: searchBar.y + searchBar.height
        height: mainItem.height * 0.75
        width: searchBar.width
        searchedText: searchBar.text
    }

    ImageViewer{
        id:imageViewer
        width: mainAppPage.width
        height: mainAppPage.height
    }

//    Popup {

//        id: showImagePopup
//        property int curIndex: -1
//        x: mainItem.x
//        y: mainItem.y
//        width: mainItem.width
//        height: mainItem.height
//        modal: true
//        onOpened: {
//            showImagePopupContentItemComponenetLoader.sourceComponent
//                    = showImagePopupContentItemComponenet
//        }

//        focus: true
//        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

//        Component {
//            id: showImagePopupContentItemComponenet

//            Item {
//                id: showImagePopupContentItem
//                anchors.fill: parent

//                Component {
//                    id: showImagePopupListViewHighlight
//                    Rectangle {
//                        width: showImagePopupContentItem.width - 5
//                        height: showImagePopupContentItem.height - 5
//                        color: "transparent"
//                        radius: 5
//                        y: showImagePopupListView.currentItem.y
//                    }
//                }

//                ListView {
//                    id: showImagePopupListView
//                    currentIndex: showImagePopup.curIndex
//                    highlight: showImagePopupListViewHighlight
//                    anchors.fill: parent
//                    model: pictureModel
//                    clip: true
//                    delegate: Item {
//                        id: showImagePopupDelegate
//                        height: showImagePopupListView.height
//                        width: showImagePopupListView.width

//                        Rectangle {
//                            id: showImagePopupDelegateBackground
//                            anchors.fill: showImagePopupDelegate
//                            color: Material.color(Material.BlueGrey)
//                        }

//                        Image {
//                            id: showImagePopupDelegateImage
//                            asynchronous: true
//                            source: 'file:///' + thumb
//                            sourceSize.height: parent.height
//                            height: parent.height
//                            fillMode: Image.PreserveAspectFit
//                            anchors.centerIn: showImagePopupDelegateBackground
//                        }

//                        Text {
//                            text: name
//                            font.pixelSize: showImagePopupDelegate.height / 15
//                            anchors.bottom: showImagePopupDelegateBackground.bottom
//                            anchors.bottomMargin: 15
//                        }
//                    }
//                }

//                MouseArea {
//                    id: popupImageMouseArea
//                    anchors.fill: parent
//                    onClicked: {
//                        showImagePopup.close()
//                    }
//                }

//                Button {
//                    text: "next"
//                    onClicked: {
//                        var curIndexx = showImagePopupListView.currentIndex
//                        showImagePopup.curIndex = curIndexx + 1
//                    }
//                }
//            }
//        }

//        contentItem: Loader {
//            id: showImagePopupContentItemComponenetLoader
//        }
//    }

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

                        sceneViewPopup.seekValue = popupVideo.duration / 60
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
//        tpopup.thumbSrc = 'file:///' + thumbSrc
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

    GlobalSearchPopup{
        id:globalSearch

        width: mainItem.width * 0.75
        height: mainItem.height * 0.05
        x: mainItem.width * 0.12
        y: mainItem.height * 0.12

    }


}
