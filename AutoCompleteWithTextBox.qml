import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id:autoCompleteWithTextBox
    property string searchType
    property int popupHeight

    property bool isMultiTag:false

    property string seperatorChar: ","
    property string searchText

    signal selected(string selectedItemType, string selectedItemName, string selectedItemId, string selectedItemAliasOfId)

    Timer {
        id: searchTimer
        interval: 250
        onTriggered: {
            autocomplete.searchedText = searchText
            autocomplete.searchType = searchType
            if (!autocomplete.isOpened) {
                autocomplete.open()
            } else {
                autocomplete.search()
            }

            console.log("Text changed to: " + searchBar.text)
        }
    }

    TextField {
        id: searchBar
        anchors.fill: parent
        height: parent.height
        placeholderText: "Search..."
        focus: true        
        onTextChanged: {
            if (searchBar.text.includes(seperatorChar))
            {
//                console.log("AutoCompleteWithTextBox: searchBar: onTextChanged: found seperator char: " + "'" + seperatorChar + "'")
                var tempArray = searchBar.text.split(",")
//                console.log("AutoCompleteWithTextBox: searchBar: onTextChanged: tempArray[0]:'" + tempArray[0] + "'")
//                console.log("AutoCompleteWithTextBox: searchBar: onTextChanged: tempArray.slice(-1)[0]:'" + tempArray.slice(-1)[0] + "'")
                if ((tempArray.slice(-1)[0] !== "") && (tempArray.slice(-1)[0] !== undefined) ){
//                    autocomplete.contentLoader.item.isMultiTag = true;
                    autoCompleteWithTextBox.isMultiTag = true;
                    searchType = "MultiSearch"
                    searchText = tempArray.slice(-1)[0]
                    searchTimer.restart()
                }
            }else{
//                autocomplete.contentLoader.item.isMultiTag = false;
                autoCompleteWithTextBox.isMultiTag = false
                searchType = "";
                searchText = searchBar.text
                searchTimer.restart()
            }


        }

        Keys.forwardTo: [autocomplete.contentLoader.item]

        Keys.onReturnPressed: {
            console.log("AutoCompleteWithTextBox: searchBar: Keys.onReturnPressed triggerd")

            autocomplete.searchedText = searchBar.text
            autocomplete.searchType = searchType
            if (!autocomplete.isOpened) {
                autocomplete.open()
            } else {
                autocomplete.search()
            }

        }
    }



    Connections{
        target: autocomplete
        onSelected:{
            console.log("AutoComplteWithTextBox: Selected :" + selectedItemName)
            selected(selectedItemType,selectedItemName,selectedItemId,selectedItemAliasOfId)
        }
    }

    AutoCompletePopupTwo {
        id: autocomplete
        isMultiTag: autoCompleteWithTextBox.isMultiTag
        x: searchBar.x
        y: searchBar.y + searchBar.height
        height: popupHeight
        width: searchBar.width
        searchedText: searchBar.text

    }
}
