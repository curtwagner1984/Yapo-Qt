import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0



Item{
    id:singleThumbDelegate
//    height: thumbGridView.cellHeight
//    width: thumbGridView.cellWidth
    property string imageSource: ""
    property string lableText: ""
    property string countInfoText: ""
    property string placeholderActor: constantClass.nO_IMAGE_PORTRAIT_PLACEHOLDER()
    property string placeholderScene: constantClass.nO_IMAGE_LANDSCAPE_PLACEHOLDER()
    property string currentPlaceHolder: ""




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
                text: singleThumbDelegate.lableText
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
            text: singleThumbDelegate.countInfoText
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
            visible: false
        }

    states:[
            State {
                name: "ACTOR_VIEW"
                PropertyChanges {
                    target: singleThumbDelegate ; currentPlaceHolder: placeholderActor

                }
                PropertyChanges {
                    target: count_info ; visible: true

                }

                PropertyChanges {
                    target: lable ; font.pixelSize: singleThumbDelegate.width / 14

                }

            },

            State {
                name: "SCENE_VIEW"
                PropertyChanges {
                    target: singleThumbDelegate ; currentPlaceHolder: placeholderScene

                }

                PropertyChanges {
                    target: lable ; font.pixelSize: singleThumbDelegate.height / 14

                }

                PropertyChanges {
                    target: buttonBar ; height: singleThumbDelegate.height / 5

                }


            },

            State {
                name: "PICTURE_VIEW"
                PropertyChanges {
                    target: singleThumbDelegate ; currentPlaceHolder: placeholderScene

                }

                PropertyChanges {
                    target: thumb ; fillMode: Image.PreserveAspectFit

                }

                PropertyChanges {
                    target: lable ; font.pixelSize: singleThumbDelegate.height / 14

                }

            }



        ]

//    Glow {
//           anchors.fill: lable
//           radius: singleThumbDelegate.width / 14
//           samples: 25
//           color: "black"
//           source: lable
//       }
}
