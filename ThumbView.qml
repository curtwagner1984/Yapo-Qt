import QtQuick 2.8
import QtQuick.Controls 2.1

import com.me.qmlcomponents 1.0

Item {
    id: thumbView

    property var currentModel
    property string searchText

    onSearchTextChanged: {
        console.log("THUMBVIEW: searchTextChanged triggered")
        actorModel.search(searchText)
    }


    property int portraitCellWidth: {
        if (thumbView.width >= 1900){
            return thumbView.width / 6
        }else if (thumbView.width < 1900 &&  thumbView.width >= 1000){
            return thumbView.width / 5
        }else if (thumbView.width < 1000 &&  thumbView.width >= 500){
            return thumbView.width / 3
        }else if (thumbView.width < 500 &&  thumbView.width >= 0){
            return thumbView.width / 2
        }
    }

    anchors.fill: parent




    GridView {
        id: thumbGridView
        property alias ratingPopup: ratingPopup

        anchors.fill: parent

        property string delegateSource
        delegate: Component {
            id: delegateComponent
            Loader {
                id: delegateLoader

                width: thumbGridView.cellWidth
                height: thumbGridView.cellHeight
//                asynchronous: true

                source: thumbGridView.delegateSource

            }
        }

        clip: true
        ScrollBar.vertical: ScrollBar {
        }

        RatingPopup{
            id:ratingPopup
            iconPath: "file:///D:/YAPOC++/resource/icons/ic_favorite_black_24px.svg"

        }



    }




    states: [
        State {
            name: "Actor"
            PropertyChanges {
                target: thumbGridView
                delegateSource: "SingleActorThumbDelegate.qml"
//                cellWidth: thumbGridView.width / 6
                cellWidth: portraitCellWidth
                cellHeight: cellWidth * 1.8
                model:currentModel
            }

        },

        State {
            name: "Scene"
            PropertyChanges {
                target: thumbGridView
                delegateSource: "SingleSceneThumbDelegate.qml"
                cellWidth: thumbGridView.width / 4
                cellHeight: cellWidth * 0.56
                model: currentModel
            }

        },

        State {
            name: "Picture"
            PropertyChanges {
                target: thumbGridView
                delegateSource: "SinglePictureThumbDelegate.qml"
                cellWidth: portraitCellWidth
                cellHeight: cellWidth * 1.8
                model: currentModel
            }

        },

        State {
            name: "Tag"
            PropertyChanges {
                target: thumbGridView
                delegateSource: "SingleTagThumbDelegate.qml"
                cellWidth: thumbGridView.width / 4
                cellHeight: cellWidth * 0.56
                model: currentModel
            }

        },

        State {
            name: "Website"
            PropertyChanges {
                target: thumbGridView
                delegateSource: "SingleWebsiteThumbDelegate.qml"
                cellWidth: thumbGridView.width / 4
                cellHeight: cellWidth * 0.56
                model: currentModel
            }

        }
    ]


}
