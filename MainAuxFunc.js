function autoCompleteSelected(selectedItemType, selectedItemName,selectedItemId, selectedItemAliasOfId) {

    console.log("Autocomplete selected " + selectedItemName
                + " Type:" + selectedItemType)
    var currentModel;

    if (selectedItemType === "Actor") {
          currentModel = createModel("Actor")
          currentModel.searchById(selectedItemId)
          mainAppPage.changeView("Actor Detail View",currentModel,0)
    } else if (selectedItemType === "ActorAlias") {
        currentModel = createModel("Actor")
        currentModel.searchById(selectedItemAliasOfId)
        mainAppPage.changeView("Actor Detail View",currentModel,0)
    } else if (selectedItemType === "Tag") {
        currentModel = createModel("Tag")
        currentModel.searchById(selectedItemId)
        mainAppPage.changeView("Tag Detail View",currentModel,0)
    } else if (selectedItemType === "TagAlias") {
        currentModel = createModel("Tag")
        currentModel.searchById(selectedItemAliasOfId)
        mainAppPage.changeView("Tag Detail View",currentModel,0)
    }else if (selectedItemType === "Website") {
        currentModel = createModel("Website")
        currentModel.searchById(selectedItemId)
        mainAppPage.changeView("Website Detail View",currentModel,0)
    }else if (selectedItemType === "WebsiteAlias") {
        currentModel = createModel("Website")
        currentModel.searchById(selectedItemAliasOfId)
        mainAppPage.changeView("Website Detail View",currentModel,0)
    }

}
