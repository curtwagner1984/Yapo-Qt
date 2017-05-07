import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1

Item {
    id: item1
    property font myFont: Qt.font({
                                      pixelSize: 14,
                                      color: "white"
                                  })


    RowLayout {
        id: buttonsLayout

        Button {
            text: "test ffprobe"
            onClicked: {
                qmlComm.ffmpegTest()
            }
        }

        Button {
            text: "test tmdb scrape"
            onClicked: {
                qmlComm.scrapeActors()
            }
        }

        Button {
            text: "tag scenes"
            onClicked: {
                qmlComm.tagAll()
            }
        }
    }

    Rectangle {
        anchors.top: buttonsLayout.bottom
//        anchors.bottom: item1.bottom
        width: 300
        color: "transparent"
        height: 600

        Component {
            id: actorDelegate
            Item {
                width: 250
                height: 80
                Row{
                    Rectangle{
                        id:placeholder
                        width:64
                        height: 128
                    }

                    Image{
                        asynchronous: true
                        autoTransform: true
                        fillMode: Image.PreserveAspectCrop
                        source: 'file:///' + thumb_64
                        sourceSize.width: 64
                        sourceSize.height: 128

                    }

                    Text {
                        text: name
                        font.pixelSize: 22
                    }
                }


                MouseArea{
                    id: actorDelegateMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        console.log("You clicked " + name)
                        actorView.currentIndex = index
                    }
                    onEntered: {
                        console.log("Entered " + name)
                    }
                }
            }
        }

        ListView {
            anchors.fill: parent
            id: actorView
            model: actorModel
            delegate: actorDelegate
            highlight: Rectangle {
                color: "lightsteelblue"
                radius: 5
            }
            focus: true
            clip: true

            ScrollBar.vertical: ScrollBar{}
        }
    }
}
//    ColumnLayout{
//        id: columnLayout



//    }

