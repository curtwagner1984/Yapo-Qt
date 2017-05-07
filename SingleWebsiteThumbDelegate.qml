import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0



Item{
    id:singleThumbDelegate

    property string imageSource: ""
    property string websiteName: ""
    property string numOfScenes: ""
    property string numOfPictures: ""


    property string currentPlaceHolder: "file:///D:/Webstorm/Yapo-Electron/app/media/unknown/unknown scene_360.jpg"



    Image {
        id: thumb
        source: singleThumbDelegate.imageSource
        asynchronous: true
        fillMode: Image.PreserveAspectCrop
        width: singleThumbDelegate.width
        height: singleThumbDelegate.height
        sourceSize.width: singleThumbDelegate.width
        sourceSize.height: singleThumbDelegate.height
        onStatusChanged: {
                    if (thumb.status == Image.Error)
                        thumb.source = currentPlaceHolder

                }

    }

//    MouseArea{
//        id: thumbMouseArea
//        anchors.fill: thumb
//        hoverEnabled: true
//        onEntered: {
//            console.log("Entered " + singleThumbDelegate.scenePath +". Thumb Path is: " + singleThumbDelegate.imageSource )
//        }
//    }

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

    Text {
        id: lable
                text: singleThumbDelegate.websiteName
                anchors.bottom: buttonBar.top
                anchors.left: thumb.left
                anchors.bottomMargin: 15
                anchors.leftMargin: 5
//                width: thumb.width
                style: Text.Outline
                styleColor: "black"
                font.pixelSize: singleThumbDelegate.width / 26
                font.bold: true
                font.capitalization: Font.Capitalize
//                clip: true
                color: "White"
        }


    Text {
            id: durationLable
            text: "(#S:"+ singleThumbDelegate.numOfScenes + "#P: " + singleThumbDelegate.numOfPictures + ")"
//            anchors.top: thumb.top
            anchors.left: lable.left
            anchors.top: lable.bottom
//            anchors.topMargin: 5
//            anchors.rightMargin: 5
            style: Text.Outline
            styleColor: "black"
            font.pixelSize: singleThumbDelegate.width / 28
            font.capitalization: Font.Capitalize
//            clip: true
            color: "White"

        }


}
