import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id:autoCompleteListViewDelegate

    property string placeHolderTag: "file:///D:/YAPOC++/resource/unkown/placeHolderTag_64.jpg"
    property string placeHolderSite: "file:///D:/YAPOC++/resource/unkown/placeHolderWebsite_64.jpg"

    focus: true

    Rectangle{
        id: imageBackground
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width / 10
        height: width * 1.5
        color: "black"

        BusyIndicator {
            anchors.centerIn: parent
            running: thumbnail.status === Image.Loading
        }

        Image {
            id: thumbnail
            asynchronous: true
            anchors.fill: parent
            anchors.margins: 3
            source: 'file:///' + thumb
            sourceSize.height: height
            sourceSize.width: width
            fillMode: Image.PreserveAspectFit
            onStatusChanged: {
                if (thumbnail.status === Image.Error)
                    if (tableName === "Tag") {
                        thumbnail.source = placeHolderTag
                    } else if (tableName === "Website") {
                        thumbnail.source = placeHolderSite
                    }else{
                        thumbnail.source = placeHolderSite
                    }
            }
        }
    }


    Text{
        id:mainLable
        anchors.left: imageBackground.right
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        text: name
    }



    Text{
        id:typeLable
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        text: tableName
    }

    MouseArea{
        id: autoCompleteListViewDelegateMouseArea
        anchors.fill: parent

        onClicked: {
            console.log("Clicked inside of autocomplete delegate")
            autoCompleteContnetItemListView.currentIndex = index
        }
    }


//    Keys.onReturnPressed: {
//        console.log("Return pressed in delegate")
//        autoCompletePopupTwo.selected(tableName, name, id, aliasOfId);
//        autoCompletePopupTwo.close()
//    }




}
