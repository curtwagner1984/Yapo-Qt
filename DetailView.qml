import QtQuick 2.8
import QtQuick.Controls 2.1

Item {
    id:detailView

    property var currentModel
    property int currentIndex
    property string mainContentState


    function next(){
        currentIndex = currentIndex + 1;
        clearDetailView();
        sideBarContentLoader.sourceComponent = sidebarContent;
        mainContentLoader.sourceComponent = mainContent;
    }

    function prev(){
        currentIndex = currentIndex - 1;
        clearDetailView();
        sideBarContentLoader.sourceComponent = sidebarContent;
        mainContentLoader.sourceComponent = mainContent;
    }

    function changeIndex(newIndex){
        currentIndex = newIndex;
        clearDetailView();
        sideBarContentLoader.sourceComponent = sidebarContent;
        mainContentLoader.sourceComponent = mainContent;
    }

    function clearDetailView (){
        sideBarContentLoader.sourceComponent = undefined
        mainContentLoader.sourceComponent = undefined
    }

    function init(curModel, curIndex, contentState){
        currentModel = curModel
        currentIndex = curIndex
        mainContentState = contentState
        sideBarContentLoader.sourceComponent = sidebarContent
        mainContentLoader.sourceComponent = mainContent

    }

    Component{
        id:sidebarContent
        DetailViewSideBarContent{
            state: mainContentState
        }
    }

    Component{
        id:mainContent
        DetailViewMainContent{
            state: mainContentState
        }
    }

    Item{
        id:sidebar
        width: parent.width * 0.25
        height: parent.height;

        anchors.left: parent.left
        anchors.top: parent.top


        Rectangle{
            color: "red"
            anchors.fill: parent

            Loader{
                id:sideBarContentLoader
                anchors.fill: parent
            }


        }

        Button{
            id:hideButton
            text:"hide"

            onClicked: {
                sidebar.visible = false;
                mainView.anchors.left = detailView.left
            }
        }
    }

    Item{
        id:mainView
//        width: parent.width * 0.75
//        height: parent.height;

        anchors.left: sidebar.right
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Rectangle{
            color: "blue"
            anchors.fill: parent;

            Loader{
                id:mainContentLoader
                anchors.fill: parent
            }


        }
    }

    RoundButton{
        id:showSideBar
        visible: !sidebar.visible
        text:"<-"
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
            sidebar.visible = true
            sidebar.anchors.left = parent.left
            sidebar.anchors.top= parent.top
            mainView.anchors.left = sidebar.right
            mainView.anchors.right = parent.right
        }
    }

}
