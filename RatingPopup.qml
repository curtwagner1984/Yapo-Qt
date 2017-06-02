import QtQuick 2.8
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtGraphicalEffects 1.0

Popup {
    id: ratingPopup

    property string iconPath

    property string ratedItemType
    property string ratedItemId
    property string ratedItemRating
    property int ratedItemIndex

    signal rated(string ratedItemType, string ratedItemId,string ratedItemRating, int ratedItemIndex)

    onClosed: {
        clearColor();
    }

    height: 50
    width: 360
    modal: false
    focus: false
    clip: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside



    background: Rectangle {
        implicitWidth: 50
        implicitHeight: ratingPopup.width
        color: "grey"
        //        border.color: "#444"
    }

    function applyColor(ix) {
        for (var i = 0; i < repeater.count; i++) {
//            console.log("inside applycolor loop i=" + i)
            if (i <= ix) {
                repeater.itemAt(i).color = Qt.rgba(1, 0, 0, (i + 1) * 0.1)
            } else {
                repeater.itemAt(i).color = Qt.rgba(0, 0, 0, 0)
            }
        }
    }

    function clearColor(){
        for (var i = 0 ; i < repeater.count ; i++){
            repeater.itemAt(i).color = Qt.rgba(0, 0, 0, 0)
        }
    }



    contentItem: RowLayout {
        id: popupRowLayout
        Repeater {
            id: repeater
            model: 10


            Item {

                id: repeaterItem
                property alias color: colorOverlay.color
                width: 30
                height: 30

                Image {
                    id: ratingImage
                    anchors.centerIn: parent
                    width: 30
                    height: 30
                    sourceSize.width: 30
                    sourceSize.height: 30
                    source: ratingPopup.iconPath

                }

                ParallelAnimation {
                        id:zoomAnimation
                        property int toValue
                        PropertyAnimation {id:zoomAnimationWidth; target:ratingImage; properties: "width"; to: zoomAnimation.toValue; easing.type: Easing.InOutQuad; duration: 300 }
                        PropertyAnimation {id:zoomAnimationHeight; target:ratingImage; properties: "height"; to: zoomAnimation.toValue; easing.type: Easing.InOutQuad; duration: 300 }
                    }

                ColorOverlay {
                    id: colorOverlay
                    anchors.fill: ratingImage
                    source: ratingImage
                    antialiasing: true
                }

                Text {
                    id: lable
                    anchors.centerIn: ratingImage
                    text: index + 1
                    color: "white"
                    font.pixelSize: ratingImage.height * 0.3
                }



                MouseArea {
                    anchors.fill: ratingImage
                    hoverEnabled: true
                    onEntered: {
//                        console.log("ratingPopup: Entered index : " + index)
//                        ratingImage.width = ratingImage.width * 1.5
//                        ratingImage.height = ratingImage.height * 1.5
                        applyColor(index)
                        zoomAnimation.stop()
                        zoomAnimation.toValue = ratingImage.width * 1.5
                        zoomAnimation.start();
                    }

                    onExited: {
//                        console.log("ratingPopup: Entered index : " + index)
//                        ratingImage.width = 30
//                        ratingImage.height = 30
                        zoomAnimation.stop()
                        zoomAnimation.toValue = 30
                        zoomAnimation.start();
                    }

                    onClicked: {
                        console.log("Rating " + (index + 1) + " Selected")
                        rated(ratedItemType, ratedItemId,index + 1,index)
                        ratingPopup.close()
                    }
                }
            }
        }
    }
}
