import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id:autoCompleteListViewDelegate



    property string placeHolderTag: "file:///D:/YAPOC++/resource/unkown/placeHolderTag_64.jpg"
    property string placeHolderSite: "file:///D:/YAPOC++/resource/unkown/placeHolderWebsite_64.jpg"
    property string placeHolderActor: "file:///D:/Webstorm/Yapo-Electron/app/media/unknown/unknown female_256.jpg"

    focus: true

    Rectangle{
        id: imageBackground
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width / 10
        height: parent.height
        color: "black"

        BusyIndicator {
            anchors.centerIn: parent
            running: thumbnail.status === Image.Loading
        }

        Image {
            id: thumbnail
            asynchronous: true
            anchors.fill: parent
            anchors.margins: 1
            source: 'file:///' + thumb
            sourceSize.height: height
            sourceSize.width: width
            fillMode: Image.PreserveAspectFit
            onStatusChanged: {
                if (thumbnail.status === Image.Error)
                    if (tableName === "Tag") {
                        thumbnail.source = placeHolderTag
                    }else if (tableName === "Website") {
                        thumbnail.source = placeHolderSite
                    }else if (tableName === "Actor") {
                        thumbnail.source = placeHolderActor
                    }else{
                        thumbnail.source = placeHolderSite
                    }
            }
        }
    }

    Item{
        id:nameContainer
        width: parent.width * 0.6
        height: parent.height
        anchors.left: imageBackground.right
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter



        function textHighlight(){
            var ans = name;
            if(autoCompletePopupTwo.searchedText.includes(" ") || autoCompletePopupTwo.searchedText.includes("."))
            {
              var tempArray = autoCompletePopupTwo.searchedText.split(/[ .]/)
              var mapObj = {};
              for (var i = 0 ; i < tempArray.length ; i++)
              {
                  if (tempArray[i] !== ""){
                    mapObj[tempArray[i].toLowerCase()] = "<font color=\"red\"><b>"+ tempArray[i] +"</b></font>";
//                    console.log("mapObj["+tempArray[i]+"]: " + mapObj[tempArray[i]])
                  }

              }
              var re = new RegExp(Object.keys(mapObj).join("|"),"ig");

//              console.log("AutoCompleteListViewDelegate: TextHighlight: ans before:'" + ans + "'")
              ans = ans.replace(re, function(matched){
                return mapObj[matched.toLowerCase()];
              });

//              console.log("AutoCompleteListViewDelegate: TextHighlight: ans after:'" + ans + "'")
              return ans

            }else{
                var reg = new RegExp(autoCompletePopupTwo.searchedText, "ig");
                ans = name.replace(reg, "<font color=\"red\"><b>"+ autoCompletePopupTwo.searchedText +"</b></font>")
                return ans
            }


        }

        Text{
            id:mainLable

            anchors.centerIn: parent
            width: parent.width - 5
            elide: Text.ElideRight
            font.pixelSize: parent.height * 0.3
            font.capitalization: Font.Capitalize
            text: {
                parent.textHighlight();
            }

            color: "White"
        }

    }

    CheckBox{
        id:isSelectedCheckBox
        visible: autoCompleteContnetItem.isMultiTag
        anchors.right: typeLableContainer.left
        anchors.verticalCenter: parent.verticalCenter
        checked: isSelected

        onClicked: {
            isSelected = checked
        }

        z:10
    }



    Item{
        id:typeLableContainer

        width: parent.width * 0.2
        height: parent.height
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter

        Rectangle{
            id:typeLableBackground
            anchors.fill: typeLable
            anchors.margins: -5
            color: "white"
            border.color: "black"
            border.width: 2
        }

        Text{
            id:typeLable
            anchors.centerIn: parent
            width: parent.width - 5
            elide: Text.ElideRight
            font.pixelSize: parent.height * 0.2
            font.capitalization: Font.Capitalize
            horizontalAlignment: Text.AlignHCenter
            text: tableName
            color: "red"
        }
    }



    MouseArea{
        id: autoCompleteListViewDelegateMouseArea
        anchors.fill: parent

        onClicked: {
            console.log("Clicked inside of autocomplete delegate")
            autoCompleteContnetItemListView.currentIndex = index
        }

        onDoubleClicked: {
            autoCompleteContnetItemListView.currentIndex = index
            autoCompleteContnetItem.selected()
        }
    }

}
