import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtMultimedia 5.8

import "qrc:/thumbview"

Item {
    id:sceneView
    height: 400
    property alias popup: sceneViewPopup

    Popup{
        id:sceneViewPopup
        property alias video: popupVideo
        property int seekValue: -1
        property int seekSum: -1

        width: 640
        height: 360
        focus: true



        function setupVideo (source){
            sceneViewPopup.open()
            console.log("Source:" + source)
            popupVideo.source = source;
//            popupVideo.play();


        }

        contentItem: Item{
            id:popupContentItem
            anchors.fill: parent

            Timer {
                   id:popupContentItemTimer
                   interval: 3000; running: false; repeat: true
                   onTriggered:
                   {
                       console.log("Timer was triggered");
                       if (sceneViewPopup.seekSum > (popupVideo.duration - sceneViewPopup.seekValue))
                       {
                           console.log("Last 15 seconds")
                           sceneViewPopup.seekSum = popupVideo.duration - 15000
                           popupVideo.seek(sceneViewPopup.seekSum);
                           running = false
                       }else{
                           console.log("Seek sum before adding " + sceneViewPopup.seekSum);
                           popupVideo.seek(sceneViewPopup.seekSum);
                           sceneViewPopup.seekSum = sceneViewPopup.seekSum + sceneViewPopup.seekValue;
                           console.log("Seek sum after adding " + sceneViewPopup.seekSum + " Duration is: " + popupVideo.duration);
                       }


                   }
               }



            Video{
                id:popupVideo
                width: sceneViewPopup.width
                height: sceneViewPopup.height
                fillMode: VideoOutput.PreserveAspectFit
                onStatusChanged: {
                        if(status === MediaPlayer.Loaded){
                            popupVideo.play()
                            sceneViewPopup.seekValue = popupVideo.duration / 12;
                            sceneViewPopup.seekSum = sceneViewPopup.seekValue
                            console.log("Seek value is: " + sceneViewPopup.seekValue + " duration is " + popupVideo.duration)
                            if (!(popupVideo.duration < 60000))
                            {

                                popupContentItemTimer.running = true
                            }else{
                                console.log("This is a short Video")
                            }


                        }
                    }
                onStopped:
                {
                    console.log("Playback has stopped");
                    if (sceneViewPopup.visible)
                    {
                        console.log("Popup is visable looping video");
                        sceneViewPopup.seekSum = 0;
                        popupVideo.seek(0);

                    }else{
                        console.log("Popup is not visable stopping playback");
                    }



                }
                onPositionChanged:
                {
                    console.log("Position changed");
                    var ratio1 = popupVideo.position / popupVideo.duration
                    var seekBarX = statusBar.width * ratio1
                    statusBarSeekHead.x = seekBarX

                }


                muted: true


            }

            Item
            {
                id:statusBar
                width: parent.width
                height: 10
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20

                Rectangle
                {
                    id:statusBarBackground
                    color: Material.color(Material.Grey)
                    height: 2
                    anchors.fill: parent
                }

                Rectangle
                {
                    id:statusBarSeekHead
                    color: Material.color(Material.Pink)
                    height: 10
                    width: 5
                    anchors.verticalCenter: statusBarBackground.verticalCenter
                    x: parent.x

                }

            }

            MouseArea{
                id: popupContentItemMouseArea
                anchors.fill: popupContentItem
                onClicked: {
                    sceneViewPopup.close()
                }

            }
        }

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        onClosed:
        {
            console.log("On Closed Evene triggered ...")
            popupContentItemTimer.running = false
            popupVideo.stop()


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
                return thumbGridView.width / 4
            }
        }


        cellHeight: cellWidth * 0.5625
        model: sceneModel
        delegate: SingleSceneThumbDelegate{
            id:singleActorThumbDelegate
            width: thumbGridView.cellWidth
            height: thumbGridView.cellHeight
            sceneName: name
            sceneDuration: duration
            imageSource: "file:///" + thumb
            scenePath: path


        }

        clip: true
        ScrollBar.vertical: ScrollBar {}

    }


}
