import QtQuick 2.8

Item {

    Component{
        id: inlinePreviewDelegate
        Item{
            id:inlinePreviewDelegateWrapper

            Image{
                id:inlinePreviewDelegateImage
                anchors.fill: parent

                source: previewPath
                asynchronous: true
                fillMode: Image.PreserveAspectCrop
                sourceSize.height: inlinePreviewDelegateImage.height
                sourceSize.width: inlinePreviewDelegateImage.width
            }

            Text{
                id:timestampText
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.margins: 5

                text: previewTimeStamp
            }
        }
    }

    ListView{
        id:inlinePreviewListView
        anchors.fill: parent
        orientation: ListView.Horizontal
        model: scenePreview
        snapMode: ListView.SnapToItem
        delegate: inlinePreviewDelegate

    }
}
