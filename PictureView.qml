import QtQuick 2.0
import QtQuick.Controls 2.1
import "qrc:/thumbview"

Item {
    id:pictureView
    height: 400
    property alias popTest: popup
    property alias popTestImgSource: popupImage.source

    Popup {        
            id: popup
            property int curIndex: -1
            x: 0
            y: 0
            width: parent.width - 5
            height: parent.height - 5
            modal: true
            focus: true
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

            Image {
                id: popupImage
//                source: singleThumbDelegate.imageSource
                asynchronous: true
                fillMode: Image.PreserveAspectFit
                width: parent.width
                height: parent.height
                sourceSize.width: parent.width
                sourceSize.height: parent.height
//                onStatusChanged: {
//                            if (thumb.status === Image.Error)
//                                thumb.source = currentPlaceHolder

//                        }

            }
            MouseArea{
                id: popupImageMouseArea
                anchors.fill: popupImage
                onClicked: {
                    popup.close()
                }
            }

            Button{
                text:"next"
                onClicked: {
                    popupImage.source = thumbGridView.itemAt(6).thumb.source.ds
                }

            }
        }

    GridView{
        id:thumbGridView
        width: parent.width
        height: parent.height
        cellWidth: {
            if (thumbGridView.width <= 1850 ){
                return thumbGridView.width / 4
            }else{
                return thumbGridView.width / 5
            }
        }


        cellHeight: cellWidth * 1.5
        model: pictureModel
        delegate: SinglePictureThumbDelegate{
            id:singleActorThumbDelegate
            width: thumbGridView.cellWidth
            height: thumbGridView.cellHeight
            pictureName: name
            imageSource: "file:///" + thumb
            picturePath: path


        }

        clip: true
        ScrollBar.vertical: ScrollBar {}

    }


}
