function ctxMenu(selectedItemType,mouseX,mouseY,index) {
    var cords = thumbDelegate.mapToItem(mainItem,0,0);
    console.log(selectedItemType + ": Cordsx: " + cords.x + " Cordsy:" + cords.y);
    contextMenu.x = cords.x + mouseX;
    contextMenu.y = cords.y + mouseY;
    contextMenu.selectedIndex = index;
    contextMenu.selectedItemType = selectedItemType;
    contextMenu.open()
}


function showRatingPopup(ratedItemType,ratedItemId,ratedItemRating,x,y,index) {
    var cords = ratingButton.mapToItem(thumbGridView,0,0);
    console.log(index + ": Cordsx: " + cords.x + " Cordsy:" + cords.y);



    thumbGridView.ratingPopup.x = cords.x
    thumbGridView.ratingPopup.y = cords.y
    thumbGridView.ratingPopup.ratedItemType = ratedItemType;
    thumbGridView.ratingPopup.ratedItemId = ratedItemId;
    thumbGridView.ratingPopup.ratedItemRating = ratedItemRating;
    thumbGridView.ratingPopup.ratedItemIndex = index;
//    thumbGridView.ratingPopup.selectedItemType = selectedItemType;

    thumbGridView.ratingPopup.open()
}
