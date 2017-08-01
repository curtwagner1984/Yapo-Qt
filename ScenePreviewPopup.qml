import QtQuick 2.8
import QtQuick.Controls 2.1

Popup{
    id:scenePreviewPopup
    focus: true
    clip: true

    property int sceneId
    property string scenePath



//    contentItem: Loader {
//        id: contentLoader
//        anchors.fill: parent
//    }

    onOpened: {
        scenePreview.getPreviewPaths(sceneId)

    }

    onClosed:{
        scenePreview.clear()
    }



    contentItem:GridView{
        id:gridView

        width: scenePreviewPopup.width - 5
        height: scenePreviewPopup.height - 5
        anchors.centerIn: scenePreviewPopup

        cellWidth: (gridView.width - 1) / 4
        cellHeight: cellWidth * 0.5625
        model: scenePreview

        delegate: Item{
            id:wrapper

            Image {
                id: image
                asynchronous: true
                source: 'file:///' + previewPath
                width: gridView.cellWidth
                height: gridView.cellHeight


            }

            Label{
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                text: previewTimestamp
                z:10
            }

            MouseArea{
                id:mouseArea
                anchors.fill: image
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                z:500
                onClicked: {
                    console.log("Clicked on preview thumb")
                    var a = previewTimestamp.split(':'); // split it at the colons

                    // minutes are worth 60 seconds. Hours are worth 60 minutes.
                    var seconds = (+a[0]) * 60 * 60 + (+a[1]) * 60 + (+a[2]);
                    console.log("Scene clicked on -> seconds: " + seconds);

                    mainAppPage.showVideo.setupVideo(scenePath,seconds*1000)
                }
            }
        }

    }

}
