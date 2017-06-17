import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Popup{
    id:imageViewer

    property var currentModel
    property int currentIndex

    focus: true

    contentItem: Loader{
        id: contentLoader
        width: imageViewer.width
        height: imageViewer.height
        focus: true

    }

    onOpened: {
        contentLoader.source = "ImageViewerContentItem.qml"
        contentLoader.item.currentIndex = imageViewer.currentIndex
        contentLoader.item.currentModel = imageViewer.currentModel
    }


    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

}
