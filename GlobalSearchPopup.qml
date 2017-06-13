import QtQuick 2.8
import QtQuick.Controls 2.1
import "qrc:/autoComplete"
import "MainAuxFunc.js" as MainAux



Popup {
    id: globalSearchPopup
    focus: true



    contentItem: Item{
        focus: true
        AutoCompleteWithTextBox{
            id: autoComplete
            focus: true
            width: globalSearchPopup.width
            height: globalSearchPopup.height
            popupHeight: mainItem.height * 0.7
        }

        Connections{
            target: autoComplete
            onSelected:{
                MainAux.autoCompleteSelected(selectedItemType,selectedItemName, selectedItemId, selectedItemAliasOfId)
                globalSearchPopup.close()
            }
        }
    }
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

}
