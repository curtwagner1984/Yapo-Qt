import QtQuick 2.8
import QtQuick.Controls.Material 2.1

ThumbDelegateButton {
    id: ratingButton
    property string rating

    iconPath: "file:///D:/YAPOC++/resource/icons/ic_favorite_black_24px.svg"
    iconText: {(rating === "") ? "?" : rating }
    backgroundColor: "transparent"
    backgroundHighlight: "white"
    buttonName: "ratingButton"
    toolTipText: "Rating"
    overlayColor: Material.color(Material.Red)
}
