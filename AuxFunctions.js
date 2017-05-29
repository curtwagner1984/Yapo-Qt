function ctxMenu(selectedItemType,mouseX,mouseY,index) {
    var cords = singleThumbDelegate.mapToItem(mainItem,0,0);
    console.log(selectedItemType + ": Cordsx: " + cords.x + " Cordsy:" + cords.y);
    contextMenu.x = cords.x + mouseX;
    contextMenu.y = cords.y + mouseY;
    contextMenu.selectedIndex = index;
    contextMenu.selectedItemType = selectedItemType;
    contextMenu.open()
}
