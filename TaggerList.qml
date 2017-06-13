import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

ListView {
    id: taggerListview
    property string listType
    signal removeClicked(string selectedItemType, string selectedItemName, string selectedItemId, string selectedItemAliasOfId)

    Component{
        id:listViewDelegateComponent
        Rectangle{
            id:listViewDelegate
            height: 50
            width: taggerListview.width - 10
            color: { index % 2 === 0 ? Material.color(Material.Amber) : Material.color(Material.Grey)}

            Text {
                id: nameText
                text: (index + 1) + ". " + name
                width: parent.width
//                font.pixelSize: parent.width / 22
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
//                anchors.leftMargin: 10
                font.pixelSize: 20
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
            }

            RoundButton {
                id: removeButton
                width: 35
                height: 35
                text: "X"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    console.log("Remove Clicked inside delegate on " + id + " " + name)
                    taggerListview.removeClicked(listType,
                                                 name, id, "")
                }
            }
        }
    }

    clip: true
    ScrollBar.vertical: ScrollBar {
    }
    delegate: listViewDelegateComponent
}
