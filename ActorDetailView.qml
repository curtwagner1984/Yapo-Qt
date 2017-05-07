import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3

import "qrc:/sceneView"
import "qrc:/pictureView"
import "qrc:/tagView"

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
        source: "file:///" + actorDetailObject.getActorAttrib("thumbnail")
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
        text: actorDetailObject.getActorAttrib("name")
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
                        text: actorDetailObject.getActorAttrib("gender")
                    }
                }
                RowLayout {

                    Text {
                        text: "Birthplace: "
                    }
                    Text {
                        text: actorDetailObject.getActorAttrib(
                                  "country_of_origin")
                    }
                }
                RowLayout {

                    Text {
                        text: "Birthday: "
                    }
                    Text {
                        text: actorDetailObject.getActorAttrib("date_of_birth")
                    }
                }
                RowLayout {

                    CheckBox {
                        id:isActorExempt
                        text: "One Word Exempt? "
                        checked: actorDetailObject.getActorAttrib("is_exempt_from_one_word_search") === "0" ? false : true
                        onCheckStateChanged: {
                            actorDetailObject.setActorAttrib("is_exempt_from_one_word_search",isActorExempt.checked ? "1" : "0")
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

                text: actorDetailObject.getActorAttrib("description")
                wrapMode: Text.WordWrap
            }
        }

        Item {
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
                            text: "One Word Exempt? "
                            checked: isExemptFromOneWordSearch
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
                        //                        actorDetailObject.getActorAttrib("imdb_id") === "" ? "" : '<html><style type="text/css"></style><a href="www.imdb.com/name/"' + actorDetailObject.getActorAttrib("imdb_id") + '">TMdB</a></html>'
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
            text: qsTr("Scenes" + " (" + actorDetailObject.getActorAttrib(
                           "NumberOfScenes") + ")")
            //            onClicked: {
            //                console.log("Clicked Scenes tab button")
            //                mainViewStackLayout.push(mainAppPage.sceneViewComp)
            //            }
        }
        TabButton {
            text: qsTr("Pictures" + " (" + actorDetailObject.getActorAttrib(
                           "NumberOfPictures") + ")")
        }
        TabButton {
            text: qsTr("Tags")
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

        TagView {
            width: mainview.width
            height: mainview.height - mainViewTabBar.height
        }
    }
}
