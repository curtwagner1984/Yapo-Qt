import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0



Item{
    id:singleThumbDelegate
//    height: thumbGridView.cellHeight
//    width: thumbGridView.cellWidth
    property string imageSource: ""
    property string actorName: ""
    property string actorId: ""
    property string number_of_scenes: ""
    property string number_of_pictures: ""
    property string gender: ""
    property string currentPlaceHolder: "file:///D:/Webstorm/Yapo-Electron/app/media/unknown/unknown female_256.jpg"



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

    MouseArea{
        id: thumbMouseArea
        anchors.fill: thumb
        onClicked: {

            qmlComm.getActorScenes(actorId)
            qmlComm.getActorPictures(actorId)
            qmlComm.aliasSearch(actorId, "Actor")
            actorDetailObject.setActor(actorId);
            mainAppPage.changeView("Actor Detail View")
        }
    }

    Rectangle{
        id: buttonBar
        width: singleThumbDelegate.width
        height: singleThumbDelegate.height / 10
        anchors.bottom: thumb.bottom
        opacity: 0.7
        color: "black"
    }

    Text {
        id: lable
                text: singleThumbDelegate.actorName
                anchors.bottom: buttonBar.top
                anchors.left: thumb.left
                anchors.bottomMargin: 15
                anchors.leftMargin: 5
//                width: thumb.width
                style: Text.Outline
                styleColor: "black"
                font.pixelSize: singleThumbDelegate.width / 14
                font.bold: true
                font.capitalization: Font.Capitalize
//                clip: true
                color: "White"
        }


    Text {
            id: count_info
            text: "#S:" + singleThumbDelegate.number_of_scenes + "#P:" + singleThumbDelegate.number_of_pictures
//            anchors.top: thumb.top
            anchors.horizontalCenter: lable.horizontalCenter
            anchors.top: lable.bottom
//            anchors.topMargin: 5
//            anchors.rightMargin: 5
            style: Text.Outline
            styleColor: "black"
            font.pixelSize: singleThumbDelegate.width / 18
            font.capitalization: Font.Capitalize
//            clip: true
            color: "White"

        }


}
