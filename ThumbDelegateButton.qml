import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtGraphicalEffects 1.0

Item {
    id: buttonContainer
    property string iconPath
    property string backgroundColor
    property string backgroundHighlight
    property string buttonName
    property string iconText
    property string toolTipText
    property var overlayColor

    signal buttonClicked (string buttonName);


    Rectangle{
        id:iconBackground
        anchors.fill: parent
        color: backgroundColor

        PropertyAnimation {id: animateColorIn; target: iconBackground; properties: "color"; to: backgroundHighlight; duration: 200}
        PropertyAnimation {id: animateColorOut; target: iconBackground; properties: "color"; to: backgroundColor; duration: 200}


        MouseArea{
            id:iconBackgroundMouseArea
            anchors.fill: parent
            hoverEnabled: true
            property bool hovered: false


            onEntered: {
                animateColorOut.stop();
                animateColorIn.start();
                hovered = true;
            }

            onExited: {
                animateColorIn.stop()
                animateColorOut.start();
                hovered = false;
            }

            onClicked: {
                buttonContainer.buttonClicked(buttonContainer.buttonName)
            }

            ToolTip.delay: 500
            ToolTip.timeout: 5000
            ToolTip.visible: hovered
            ToolTip.text: toolTipText
        }



    }

    Image{
        id:icon
        anchors.fill: parent
        anchors.margins: 2
        source: buttonContainer.iconPath



    }

    ColorOverlay {
        anchors.fill: icon
        source: icon

        // In dark styles background is dark, this makes the image white
        color: buttonContainer.overlayColor
    }

    Text{
        id:buttonText
        text: iconText
        anchors.centerIn: icon
        anchors.margins: 0.5
        color: "White"
        font.pixelSize: icon.height /2
        font.capitalization: Font.Capitalize
        style: Text.Outline
        styleColor: "black"

    }
}
