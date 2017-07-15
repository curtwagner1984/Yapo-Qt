import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Item {
    id: mainContent

    property var tabBarModel
    property var views: []
    property var counts: []

    function destroyViews() {
        for (var i = 0; i < views.length; i++) {
            views[i].destroy()
        }
    }

    Component.onCompleted: {

        var currentItemId = detailView.currentId

        if (state === "Actor") {

            var currentView = mainAppPage.createThumbView("Scene")
            currentView.currentModel.getActorScenes(currentItemId)

            views.push(currentView)

            var pictureView = mainAppPage.createThumbView("Picture")
            pictureView.currentModel.getActorPictures(currentItemId)
            views.push(pictureView)

            var tagView = mainAppPage.createThumbView("Tag")
            tagView.currentModel.getActorTags(currentItemId)
            views.push(tagView)
        } else if (state === "Tag") {

            var sceneView = mainAppPage.createThumbView("Scene")
            sceneView.currentModel.getTagScenes(currentItemId)

            views.push(sceneView)

            var pictureView = mainAppPage.createThumbView("Picture")
            pictureView.currentModel.getTagPictures(currentItemId)

            views.push(pictureView)

            var actorView = mainAppPage.createThumbView("Actor")
            actorView.currentModel.getTagActors(currentItemId)

            views.push(actorView)

            var websiteView = mainAppPage.createThumbView("Website")
            websiteView.currentModel.getTagWebsites(currentItemId)

            views.push(websiteView)
        } else if (state === "Website") {

            var sceneView = mainAppPage.createThumbView("Scene")
            sceneView.currentModel.getWebsiteScenes(currentItemId)

            views.push(sceneView)

            var pictureView = mainAppPage.createThumbView("Picture")
            pictureView.currentModel.getWebsitePictures(currentItemId)

            views.push(pictureView)

            var tagView = mainAppPage.createThumbView("Tag")
            tagView.currentModel.getWebsiteTags(currentItemId)

            views.push(tagView)
        } else if (state === "Scene") {

            var actorView = mainAppPage.createThumbView("Actor")
            actorView.currentModel.getSceneActorsForTagger(currentItemId)

            views.push(actorView)

            var tagView = mainAppPage.createThumbView("Tag")
            tagView.currentModel.getSceneTagsForTagger(currentItemId)

            views.push(tagView)

            var websiteView = mainAppPage.createThumbView("Website")
            websiteView.currentModel.getSceneWebsitesForTagger(currentItemId)

            views.push(websiteView)
        }

        detailStackView.push(views[0])
    }

    function getCount(countIndex) {
        return counts[countIndex]
    }

    TabBar {
        id: tabBar
        anchors.top: parent.top
        property int currentTabIndex: 0
        width: parent.width

        Repeater {
            model: tabBarModel

            TabButton {
                text: modelData
                width: Math.max(100, tabBarModel.width / 5)
                onClicked: {
                    console.log("Clicked tab button : " + modelData + " index: " + index)
                    if (index !== tabBar.currentTabIndex) {
                        changePage(index)
                        tabBar.currentTabIndex = index
                    }
                }
            }
        }
    }

    function changePage(ixToChangeTo) {
        detailStackView.push(views[ixToChangeTo])
    }

    StackView {
        id: detailStackView
        anchors.top: tabBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        clip: true

        onCurrentItemChanged: {
            console.log("detailStackView current item changed")
        }
    }

    states: [
        State {
            name: "Actor"
            PropertyChanges {
                target: mainContent
                tabBarModel: ["Scenes (" + detailView.currentModel.directData(
                        "numberOfScenes", detailView.currentIndex)
                    + ")", "Pictures (" + detailView.currentModel.directData(
                        "numberOfPictures", detailView.currentIndex)
                    + ")", "Tags (" + detailView.currentModel.directData(
                        "numberOfTags", detailView.currentIndex) + ")"]
            }
        },
        State {
            name: "Tag"
            PropertyChanges {
                target: mainContent
                tabBarModel: ["Scenes ("
                    + detailView.currentModel.directData("numberOfScenes",
                                                         detailView.currentIndex) + ")", "Pictures ("
                    + detailView.currentModel.directData("numberOfPictures",
                                                         detailView.currentIndex) + ")", "Actors ("
                    + detailView.currentModel.directData("numberOfActors",
                                                         detailView.currentIndex) + ")", "Websites ("
                    + detailView.currentModel.directData("numberOfWebsites",
                                                         detailView.currentIndex) + ")"]
            }
        },
        State {
            name: "Website"
            PropertyChanges {
                target: mainContent
                tabBarModel: ["Scenes (" + detailView.currentModel.directData(
                        "numberOfScenes", detailView.currentIndex)
                    + ")", "Pictures (" + detailView.currentModel.directData(
                        "numberOfPictures", detailView.currentIndex)
                    + ")", "Tags (" + detailView.currentModel.directData(
                        "numberOfTags", detailView.currentIndex) + ")"]
            }
        },
        State {
            name: "Scene"
            PropertyChanges {
                target: mainContent
                tabBarModel: ["Actors (" + detailView.currentModel.directData(
                        "numberOfScenes", detailView.currentIndex)
                    + ")", "Tags (" + detailView.currentModel.directData(
                        "numberOfPictures", detailView.currentIndex)
                    + ")", "Website (" + detailView.currentModel.directData(
                        "numberOfTags", detailView.currentIndex) + ")"]
            }
        }
    ]
}
