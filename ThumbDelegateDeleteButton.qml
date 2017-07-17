import QtQuick 2.8
import QtQuick.Controls.Material 2.1

ThumbDelegateButton {
    id: deleteButton
    property string currentItemType
    property string currentItemName

    iconPath: constantClass.iCON_DELETE();
    iconText: ""
    backgroundColor: "transparent"
    backgroundHighlight: "white"
    buttonName: "deleteButton"
    toolTipText: "Remove " + currentItemType + ", " + currentItemName + " From Database."
    overlayColor: Material.color(Material.Grey)
}
