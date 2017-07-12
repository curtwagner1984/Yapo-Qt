import QtQuick 2.8
import QtQuick.Controls.Material 2.1

ThumbDelegateButton {
    id: ratingButton
    property string rating

    iconPath: constantClass.bUTTON_ICON_FAVORITE();
    iconText: {(rating === "") ? "?" : rating }
    backgroundColor: "transparent"
    backgroundHighlight: "white"
    buttonName: "ratingButton"
    toolTipText: "Rating"
    overlayColor: Material.color(Material.Red)
}
