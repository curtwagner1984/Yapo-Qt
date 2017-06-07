import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id:sideBarContent

    Item {
        id: thumbAndNameBackground
        width: parent.width
        height: parent.height
        anchors.left: parent.left
        anchors.top: parent.top

        Rectangle {
            id: imagePlaceHolder
            width: parent.width * 0.8
            height: width * 1.5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 15
            color: "black"

            Image {
                id: thumbnail
                anchors.fill: parent
                anchors.margins: 5
                fillMode: Image.PreserveAspectCrop
                asynchronous: true
                sourceSize.width: width
                sourceSize.height: height
                source: 'file:///' + detailView.currentModel.directData("thumb", detailView.currentIndex)

                RoundButton {
                    id: nextButton
                    text: "→"
                    height: 50
                    width: 50
                    anchors.top: parent.top
                    anchors.right: parent.right

                    onClicked: {
                        taggerPopup.next()
                    }
                }

                RoundButton {
                    id: prevButton
                    text: "←"
                    height: 50
                    width: 50
                    anchors.top: parent.top
                    anchors.left: parent.left

                    onClicked: {
                        taggerPopup.prev()
                    }
                }
            }
        }

        Text {
            id: selectedName
            width: parent.width - 10
            text: detailView.currentModel.directData("name", detailView.currentIndex)
            font.pixelSize: parent.width / 10
            anchors.top: imagePlaceHolder.bottom
            anchors.topMargin: 10
            color: Material.color(Material.Purple)
            anchors.horizontalCenter: parent.horizontalCenter
            style: Text.Outline
            styleColor: "black"
            fontSizeMode: Text.HorizontalFit
            horizontalAlignment: Text.AlignHCenter
        }
    }

}
