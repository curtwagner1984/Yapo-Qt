import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Popup {
    id:autoCompletePopupTwo

    property bool isMultiTag

    property string searchedText
    property string searchType
    property alias contentLoader: contentLoader
    property bool isOpened: false

    signal selected(string selectedItemType, string selectedItemName, string selectedItemId, string selectedItemAliasOfId)


    modal: false
    focus: false
    clip: true


    function search ()
    {
        console.log("searchType: " + searchType + " +searchedText: " + searchedText)
        if (searchType === "Tag")
        {
            autoCompleteModel.search(searchedText,"Tag")
        }else if (searchType === "MultiSearch"){
            autoCompleteModel.search(searchedText,"MultiSearch")
        }else{
            autoCompleteModel.search(searchedText,"")
        }
    }


    contentItem: Loader {
        id: contentLoader
        anchors.fill: parent
        source: "AutoCompleteContentItem.qml"
        focus: true


    }

    Binding{
        target: contentLoader.item
        property: "isMultiTag"
        value: autoCompletePopupTwo.isMultiTag
        when: contentLoader.status === Loader.Ready
    }

    onOpened:
    {
      search();

    }

    onAboutToShow: {
       isOpened = true;
    }

    onAboutToHide: {
        isOpened = false;
    }

    onClosed:
    {

    }


    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
}
