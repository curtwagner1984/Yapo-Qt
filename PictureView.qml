import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import "qrc:/thumbview"

Item {
    id: pictureView
    height: 400   
//    property alias popTest: popup

    //    property alias popTestImgSource: popupImage.source


    GridView {
        id: thumbGridView
        width: parent.width
        height: parent.height
        cellWidth: {
            if (thumbGridView.width <= 1850) {
                return thumbGridView.width / 4
            } else {
                return thumbGridView.width / 5
            }
        }

        cellHeight: cellWidth * 1.5
        model: pictureModel
        delegate: SinglePictureThumbDelegate {
            id: singleActorThumbDelegate
            width: thumbGridView.cellWidth
            height: thumbGridView.cellHeight
            pictureName: name
            imageSource: "file:///" + thumb
            picturePath: path
        }

        clip: true
        ScrollBar.vertical: ScrollBar {
        }
    }
}
