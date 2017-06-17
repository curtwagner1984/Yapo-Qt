import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "/thumbview/AuxFunctions.js" as AuxFunc


Item {
    id:imageViewerContentItem
    focus: true

    property var currentModel
    property int currentIndex

    property int slideShowDelayTime: 3000
    property bool isSlideShow: false

    property string imageSource

    onCurrentModelChanged: {
        setImgSource();
    }

    onCurrentIndexChanged: {
        setImgSource();
    }

    function setImgSource()
    {
        imageSource = 'file:///' + currentModel.directData(
                    "thumb", currentIndex)
    }


    function pageSwitch(newIndex)
    {
        currentIndex = newIndex;
//        setImgSource();
    }

    function shouldshowPrev(){
        return currentIndex > 0;
    }

    function nextPage()
    {
        if (shouldShowNext()){
            currentIndex = currentIndex + 1
        }

    }

    function prevPage()
    {
        if (shouldshowPrev()){
            currentIndex = currentIndex - 1
        }

    }

    function shouldShowNext(){
        var ans = true;
        if (currentIndex === (currentModel.rowCount() - 1)){
            if (!currentModel.canFetchMore()){
                ans = false;
            }
        }
        return ans;
    }


    Keys.onRightPressed: {
        console.log("RightPressed")
        if (isSlideShow){
            slideShowTimer.restart();
        }

        nextPage();


    }

    Keys.onLeftPressed: {
        console.log("LeftPressed")
        if (isSlideShow){
            slideShowTimer.restart();
        }
        prevPage();
    }

    Keys.onSpacePressed: {
        console.log("SpacePressed")
        isSlideShow = !isSlideShow;
    }

    onIsSlideShowChanged: {
        if (isSlideShow){
            slideShowTimer.running = true;
        }else{
            slideShowTimer.running = false;
        }
    }

    Timer{
        id:slideShowTimer
        interval: slideShowDelayTime
        repeat: true
        onTriggered:
        {
            nextPage();
        }

    }

    Item{
        id:imageContainer
        anchors.fill: parent

        BusyIndicator {
            anchors.centerIn: parent
            running: image.status === Image.Loading
        }

        Image {
            id: image
            anchors.fill: parent
            source: imageSource
            fillMode: Image.PreserveAspectFit
            asynchronous: true
        }



    }

    MouseArea{
        id:imageMouseArea
        anchors.fill: imageContainer

        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: {
            console.log("imageMouseArea clicked")
            if (mouse.button & Qt.RightButton) {
                console.log("imageMouseArea Right clicked")
                var cords = image.mapToItem(mainItem,0,0);
                AuxFunc.ctxMenu("Picture", cords.x + mouseX, cords.y + mouseY, currentIndex,currentModel)
            }
        }
    }

    Button{
        id:nextButton
        text:"Next"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        visible: shouldShowNext();
        onClicked: {
            pageSwitch(currentIndex + 1)
        }
    }

    Button{
        id:prevButton
        text:"Prev"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 10
        visible: shouldshowPrev();

        onClicked: {
            pageSwitch(currentIndex - 1)
        }
    }

    Item{
        id:slideShowIndicatorBackground
        width: parent.width * 0.05
        height: width

        anchors.top: prevButton.bottom
        anchors.horizontalCenter: prevButton.horizontalCenter
        anchors.margins: 10

        visible: isSlideShow

        Rectangle{
            id:slideShowIndicator
            anchors.centerIn: parent
            border.color: "green"
            border.width: parent.width * 0.1

            Text {
                id: slideShowIndicatorText
                anchors.centerIn: parent
                text: qsTr("Slideshow")
            }
        }
    }




}
