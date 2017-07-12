//import QtQuick 2.0
//import QtQuick.Controls 2.1
//import QtQuick.Controls.Material 2.1


//import "qrc:/thumbview"

//Item {
//    id:sceneView
//    height: 400
////    property alias popup: sceneViewPopup

////    Popup{
////        id:sceneViewPopup
////        property alias video: popupVideo
////        property int seekValue: -1
////        property int seekSum: -1

////        width: 640
////        height: 360
////        focus: true



////        function setupVideo (source){
////            sceneViewPopup.open()
////            console.log("Source:" + source)
////            popupVideo.source = source;
//////            popupVideo.play();


////        }

////        contentItem: Item{
////            id:popupContentItem
////            anchors.fill: parent

////            Timer {
////                   id:popupContentItemTimer
////                   interval: 2000; running: false; repeat: true
////                   onTriggered:
////                   {

////                       console.log("Timer was triggered");
////                       if (sceneViewPopup.seekSum > (popupVideo.duration - sceneViewPopup.seekValue))
////                       {
////                           popupContentItemTimer.stop()
////                           console.log("Last 30 seconds")
////                           sceneViewPopup.seekSum = popupVideo.duration - 30000
////                           popupVideo.seek(sceneViewPopup.seekSum);

////                       }else{
////                           console.log("Running is " + running + " Seek sum before adding " + sceneViewPopup.seekSum);
////                           popupVideo.seek(sceneViewPopup.seekSum);
////                           sceneViewPopup.seekSum = sceneViewPopup.seekSum + sceneViewPopup.seekValue;
////                           console.log("Running is " + running + " Seek sum after adding " + sceneViewPopup.seekSum + " Duration is: " + popupVideo.duration);
////                       }


////                   }
////                   onRunningChanged: {
////                       console.log("Running Changed to " + popupContentItemTimer.running)
////                   }
////               }



////            Video{
////                id:popupVideo
////                width: sceneViewPopup.width
////                height: sceneViewPopup.height
////                fillMode: VideoOutput.PreserveAspectFit
////                onStatusChanged: {
////                    console.log("Status changed triggered in popupVideo, Status is " + status)
////                        if(status === MediaPlayer.Loaded && sceneViewPopup.visible){


////                            sceneViewPopup.seekValue = popupVideo.duration / 20;
////                            sceneViewPopup.seekSum = sceneViewPopup.seekValue
////                            console.log("Seek value is: " + sceneViewPopup.seekValue + " duration is " + popupVideo.duration)
////                            if (!(popupVideo.duration < 60000))
////                            {
////                                popupContentItemTimer.running = true
////                            }else{
////                                console.log("This is a short Video")
////                            }
////                            popupVideo.play()


////                        }
////                    }
////                onStopped:
////                {
////                    console.log("Playback has stopped");
////                    if (sceneViewPopup.visible)
////                    {
////                        console.log("Popup is visable looping video");
////                        sceneViewPopup.seekSum = 0;
////                        popupVideo.seek(0);

////                    }else{
////                        console.log("Popup is not visable stopping playback");
////                    }



////                }
////                onPositionChanged:
////                {
//////                    console.log("Position changed");
////                    var ratio1 = popupVideo.position / popupVideo.duration
////                    var seekBarX = statusBar.width * ratio1
////                    statusBarSeekHead.x = seekBarX

////                }


////                muted: true


////            }

////            Item
////            {
////                id:statusBar
////                width: parent.width
////                height: 10
////                anchors.bottom: parent.bottom
////                anchors.bottomMargin: 20

////                Rectangle
////                {
////                    id:statusBarBackground
////                    color: Material.color(Material.Grey)
////                    height: 1
////                    width: parent.width
////                }

////                Rectangle
////                {
////                    id:statusBarSeekHead
////                    color: Material.color(Material.Pink)
////                    height: 10
////                    width: 5
////                    anchors.verticalCenter: statusBarBackground.verticalCenter
////                    x: parent.x

////                }

////            }

////            MouseArea{
////                id: popupContentItemMouseArea
////                anchors.fill: popupContentItem
////                onClicked: {
////                    sceneViewPopup.close()
////                }

////            }
////        }

////        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
////        onClosed:
////        {
////            console.log("On Closed Evene triggered ...")
////            popupContentItemTimer.stop()
////            popupVideo.stop()


////        }


////    }

//    GridView{
//        id:thumbGridView
//        width: parent.width
//        height: parent.height
//        cellWidth: {
//            if (thumbGridView.width <= 1850 ){
//                return thumbGridView.width / 4
//            }else{
//                return thumbGridView.width / 4
//            }
//        }


//        cellHeight: cellWidth * 0.5625
//        model: sceneModel
//        delegate: SingleSceneThumbDelegate{
//            id:singleActorThumbDelegate
//            width: thumbGridView.cellWidth
//            height: thumbGridView.cellHeight
//            sceneName: name
//            sceneDuration: duration
//            imageSource: "file:///" + thumb
//            scenePath: path


//        }

//        clip: true
//        ScrollBar.vertical: ScrollBar {}

//    }


//}
