import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    property string searchType
    property int popupHeight

    signal selected(string selectedItemType, string selectedItemName, string selectedItemId, string selectedItemAliasOfId)

    Timer {
        id: searchTimer
        interval: 250
        onTriggered: {
            autocomplete.searchedText = searchBar.text
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
        property string searched_text: searchBar.text
        onTextChanged: {
            searchTimer.restart()
        }

        Keys.forwardTo: [autocomplete.contentLoader.item]
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
        x: searchBar.x
        y: searchBar.y + searchBar.height
        height: popupHeight
        width: searchBar.width
        searchedText: searchBar.text

    }
}
