import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "qrc:/thumbview"

import "qrc:/thumbview/AuxFunctions.js" as ThumbViewAuxFunc

Item {
    id: thumbAndName
    property var currentModel
    property var currentIndex

    signal indexChanged(int newIndex);


    function next(){
        currentIndex = currentIndex + 1;
        indexChanged(currentIndex)
    }

    function prev(){
        currentIndex = currentIndex - 1;
        indexChanged(currentIndex)
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


    Component{
        id:thumbActorComponenet

        Item{
            id:thumbActorWraper

            GeneralThumb{
                id:thumbActor
                anchors.fill: parent
                state: "Actor"

                thumbnailSource: 'file:///' + currentModel.directData("thumb", currentIndex)
                lable: currentModel.directData("name", currentIndex)
                currentRating: currentModel.directData("rating", currentIndex)
                numOfPics: currentModel.directData("numberOfPictures", currentIndex)
                numOfScenes: currentModel.directData("numberOfScenes", currentIndex)
            }

        }
    }


    Component{
        id:thumbSceneComponenet

        Item{
            id:thumbSceneWraper

            GeneralThumb{
                id:thumbScene
                anchors.fill: parent
                state: "Scene"

                thumbnailSource: 'file:///' + currentModel.directData("thumb", currentIndex)
                lable: currentModel.directData("name", currentIndex)
                pathToFile: currentModel.directData("path", currentIndex)
                currentRating: currentModel.directData("rating", currentIndex)
            }

        }
    }

    Component{
        id:thumbPictureComponenet

        Item{
            id:thumbPictureWraper

            GeneralThumb{
                id:thumbPicture
                anchors.fill: parent
                state: "Picture"

                thumbnailSource: 'file:///' + currentModel.directData("thumb", currentIndex)
                lable: currentModel.directData("name", currentIndex)
                pathToFile: currentModel.directData("path", currentIndex)
                currentRating: currentModel.directData("rating", currentIndex)
            }

        }
    }

    Rectangle {
        id: imagePlaceHolder
        width: parent.width * 0.8
        height: width * 1.5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 15
        color: "black"

        Loader{
            id:thumbLoader
            sourceComponent: thumbActorComponenet
            anchors.fill: parent
        }


        RoundButton {
            id: nextButton
            text: "→"
            height: 50
            width: 50
            anchors.top: parent.top
            anchors.right: parent.right
            visible: shouldShowNext();

            onClicked: {
                next()
            }
        }

        RoundButton {
            id: prevButton
            text: "←"
            height: 50
            width: 50
            anchors.top: parent.top
            anchors.left: parent.left
            visible: { currentIndex >= 1 }

            onClicked: {
                prev()
            }
        }
    }

    Text {
        id: selectedName
        width: parent.width - 10
        text: currentModel.directData("name", currentIndex)
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

    states:[
        State {
            name: "Actor"
            PropertyChanges {
                target: thumbLoader
                sourceComponent: thumbActorComponenet

            }
            PropertyChanges {
                target: imagePlaceHolder
                height: 1.5 * imagePlaceHolder.width

            }
        },
        State {
            name: "Scene"
            PropertyChanges {
                target: thumbLoader
                sourceComponent: thumbSceneComponenet

            }
            PropertyChanges {
                target: imagePlaceHolder
                height: 0.56 * imagePlaceHolder.width

            }
        },
        State {
            name: "Picture"
            PropertyChanges {
                target: thumbLoader
                sourceComponent: thumbPictureComponenet

            }
            PropertyChanges {
                target: imagePlaceHolder
                height: 1.5 * imagePlaceHolder.width

            }
        }

    ]


}
