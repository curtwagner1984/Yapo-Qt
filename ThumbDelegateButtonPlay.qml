import QtQuick 2.8
import QtQuick.Controls.Material 2.1


ThumbDelegateButton {
    id: playButton

    iconPath: constantClass.bUTTON_ICON_PLAY()
    iconText: ""
    backgroundColor: "transparent"
    backgroundHighlight: "white"
    buttonName: "playButton"
    overlayColor: Material.color(Material.Green)
}
