import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Material 2.1


Item{
    id:singleThumbDelegate

    property string imageSource: ""
    property string tagName: ""
    property string tagId: ""
    property string numOfScenes: ""
    property string numOfPictures: ""
    property string numOfActors: ""

    property string currentPlaceHolder: "file:///D:/Webstorm/Yapo-Electron/app/media/unknown/unknown scene_360.jpg"

    Rectangle{
        id:background
        anchors.fill: parent
        color: Material.color(Material.Grey)

    }

    Rectangle{
        id:thumb
        anchors.fill: background
        anchors.margins: 2
        color: "white"

    }

//    Image {
//        id: thumb
//        source: singleThumbDelegate.imageSource
//        asynchronous: true
//        fillMode: Image.PreserveAspectCrop
//        width: singleThumbDelegate.width
//        height: singleThumbDelegate.height
//        sourceSize.width: singleThumbDelegate.width
//        sourceSize.height: singleThumbDelegate.height
//        onStatusChanged: {
//                    if (thumb.status == Image.Error)
//                        thumb.source = currentPlaceHolder

//                }

//    }

    MouseArea{
        id: thumbMouseArea
        anchors.fill: thumb
        onClicked: {
            qmlComm.prepareDetailView(tagId,"Tag")
            mainAppPage.changeView("Tag Detail View")
        }
    }

    Rectangle{
        id: buttonBar
        width: singleThumbDelegate.width
        height: singleThumbDelegate.height / 5
        anchors.bottom: thumb.bottom
        opacity: 0.7
        color: "black"
    }

    Button{
        id:playScene
        text:""
        height: buttonBar.height - 4
        implicitWidth: height
        anchors.left: buttonBar.left
        anchors.verticalCenter: buttonBar.verticalCenter
        hoverEnabled: true
        ToolTip.visible: hovered
        ToolTip.text: qsTr("Play file " + singleThumbDelegate.scenePath)
        onClicked: {
            qmlComm.playScene(singleThumbDelegate.scenePath)
        }

        Image {
                id: image
                fillMode: Image.PreserveAspectFit
                anchors.centerIn: parent
                sourceSize.height: playScene.background.height - 6
                height: sourceSize.height
                source: "file:///D:/YAPOC++/resource/icons/ic_play_arrow_black_48dp/web/ic_play_arrow_black_48dp_1x.png"
            }
            ColorOverlay {
                anchors.fill: image
                source: image
                // In dark styles background is dark, this makes the image white
                color: "#ffffffff"
            }

    }

    Item{
        id:lableContainer
        width: thumb.width - 10
        height: thumb.height * (3/4)

        Text {
            id: lable
            text: singleThumbDelegate.tagName
//                    wrapMode: Text.Wrap


//                anchors.bottom: buttonBar.top
//                anchors.left: thumb.left
//                anchors.bottomMargin: 15
//                anchors.leftMargin: 5
            anchors.centerIn: lableContainer
//                width: thumb.width
            style: Text.Outline
            styleColor: "black"
            font.pixelSize: thumb.height / 8
            font.bold: true
            font.capitalization: Font.Capitalize
            clip: true
            color: "White"




            }

    }




    Rectangle
    {
        id: durationLableBackground
        anchors.fill: durationLable
        color: Material.color(Material.Red)

    }


    Text {
            id: durationLable
            text: "(#S:"+ singleThumbDelegate.numOfScenes + " #A: "  + singleThumbDelegate.numOfActors + "#P: " + singleThumbDelegate.numOfPictures + ")"
//            anchors.top: thumb.top
//            anchors.left: lable.left
//            anchors.top: lable.bottom
            anchors.horizontalCenter: lableContainer.horizontalCenter
//            anchors.top: lable.bottom
            anchors.bottom: buttonBar.top
            anchors.bottomMargin: 10
//            anchors.rightMargin: 5
            style: Text.Outline
            styleColor: "black"
            font.pixelSize: thumb.height / 10
            font.capitalization: Font.Capitalize
//            clip: true
            color: "White"

        }




}
