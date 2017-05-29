import QtQuick 2.5
import QtQuick.Controls 2.1

ComboBox {
    id: orderBy
    textRole: "key"
    property string pageType: ""
    width: 300

    ListModel {
        id: actorOrderByModel
        ListElement {
            key: "Name"
            value: "name"
        }

        ListElement {
            key: "Name DESC"
            value: "name"
        }

        ListElement {
            key: "Gender"
            value: "gender"
        }

        ListElement {
            key: "Gender DESC"
            value: "gender"
        }

        ListElement {
            key: "Country"
            value: "country_of_origin"
        }

        ListElement {
            key: "Country DESC"
            value: "country_of_origin"
        }

        ListElement {
            key: "Number Of Scenes"
            value: "NumberOfScenes"
        }

        ListElement {
            key: "Number Of Scenes DESC"
            value: "NumberOfScenes"
        }

        ListElement {
            key: "Number Of Pictures"
            value: "NumberOfPictures"
        }

        ListElement {
            key: "Number Of Scenes DESC"
            value: "NumberOfPictures"
        }

        ListElement {
            key: "Number Of Tags"
            value: "NumberOfTags"
        }

        ListElement {
            key: "Number Of Tags DESC"
            value: "NumberOfTags"
        }

        ListElement {
            key: "DOB"
            value: "date_of_birth"
        }

        ListElement {
            key: "DOB DESC"
            value: "date_of_birth"
        }

        ListElement {
            key: "Date added"
            value: "created_at"
        }

        ListElement {
            key: "Date added DESC"
            value: "created_at"
        }

        ListElement {
            key: "Mainstream DESC"
            value: "is_mainstream"
        }
    }

    ListModel {
        id: pictureOrderByModel

        ListElement {
            key: "Date added DESC"
            value: "created_at"
        }

        ListElement {
            key: "Path"
            value: "path_to_file"
        }

        ListElement {
            key: "Path DESC"
            value: "path_to_file"
        }
    }

    ListModel {
        id: sceneOrderByModel
        ListElement {
            key: "Name"
            value: "name"
        }

        ListElement {
            key: "Name DESC"
            value: "name"
        }

        ListElement {
            key: "Path"
            value: "path_to_file"
        }

        ListElement {
            key: "Path DESC"
            value: "path_to_file"
        }

        ListElement {
            key: "Codec"
            value: "codec_name"
        }

        ListElement {
            key: "Codec DESC"
            value: "codec_name"
        }

        ListElement {
            key: "Duration"
            value: "duration"
        }

        ListElement {
            key: "Duration DESC"
            value: "duration"
        }

        ListElement {
            key: "Size"
            value: "size"
        }

        ListElement {
            key: "Size DESC"
            value: "size"
        }

        ListElement {
            key: "Height"
            value: "height"
        }

        ListElement {
            key: "Height DESC"
            value: "height"
        }

        ListElement {
            key: "Date added"
            value: "created_at"
        }

        ListElement {
            key: "Date added DESC"
            value: "created_at"
        }

        ListElement {
            key: "Mainstream DESC"
            value: "is_mainstream"
        }
    }
    //    model: actorOrderByModel
    visible: false

    states: [
        State {
            name: "actorOrder"
            PropertyChanges {
                target: orderBy
                pageType: "Actor"
                model: actorOrderByModel
            }
        },
        State {
            name: "pictureOrder"
            PropertyChanges {
                target: orderBy
                pageType: "Picture"
                model: pictureOrderByModel
            }
        },
        State {
            name: "sceneOrder"
            PropertyChanges {
                target: orderBy
                pageType: "Scene"
                model: sceneOrderByModel
            }
        }
    ]
    onActivated: {
//        var selectedOrderValue = actorOrderByModel.get(index).value
//        var selectedOrderKey = actorOrderByModel.get(index).key
//        var desc = selectedOrderKey.includes("DESC") ? "DESC" : ""

        function getValues(currentModel, index){
            var selectedOrderValue = currentModel.get(index).value
            var selectedOrderKey = currentModel.get(index).key
            var desc = selectedOrderKey.includes("DESC") ? "DESC" : ""
            return [selectedOrderValue,desc]
        }

        console.log("orderBy activated ... Selected item is: " + actorOrderByModel.get(
                        index).value)
        var currentModel;
        var values;
        if (orderBy.pageType === "Actor") {
            currentModel = actorOrderByModel;
            values = getValues(currentModel,index)
            actorModel.setOrder(values[0], values[1])
        } else if (orderBy.pageType === "Picture") {
            currentModel = pictureOrderByModel;
            values = getValues(currentModel,index)
            pictureModel.setOrder(values[0], values[1])
        } else if (orderBy.pageType === "Scene") {
            currentModel = sceneOrderByModel;
            values = getValues(currentModel,index)
            sceneModel.setOrder(values[0], values[1])
        }
    }
}
