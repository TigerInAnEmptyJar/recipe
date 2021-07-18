import QtQuick 2.9
import QtQml.Models 2.3

DelegateModel {
  id: delegateModel

  property var lessThan: function(left, right) { return true; }
  property var filterAcceptsItem: function(item) { return true; }

  function insertPosition(lessThan, item) {
    var lower = 0
    var upper = items.count
    while (lower < upper) {
      var middle = Math.floor(lower + (upper - lower) / 2)
      var result = lessThan(item.model, items.get(middle).model);
      if (result) {
        upper = middle
      } else {
        lower = middle + 1
      }
    }
    return lower
  }

  function sort(lessThan) {
    while (unsortedItems.count > 0) {
      var item = unsortedItems.get(0)
      var index = insertPosition(lessThan, item)

      item.groups = "items"
      items.move(item.itemsIndex, index)
    }
  }

  function update() {
      if (items.count > 0) {
          items.setGroups(0, items.count, "items");
      }

      // Step 1: Filter items
      var visible = [];
      for (var i = 0; i < items.count; ++i) {
          var item = items.get(i);
          if (filterAcceptsItem(item.model)) {
              visible.push(item);
          }
      }

      // Step 2: Sort the list of visible items
      visible.sort(function(a, b) {
          return lessThan(a.model, b.model) ? -1 : 1;
      });

      // Step 3: Add all items to the visible group:
      for (i = 0; i < visible.length; ++i) {
          item = visible[i];
          item.inVisible = true;
          if (item.visibleIndex !== i) {
              visibleItems.move(item.visibleIndex, i, 1);
          }
      }
  }

  items.onChanged: sort(lessThan)
  onLessThanChanged: sort(lessThan)
  onFilterAcceptsItemChanged: sort(lessThan)

  groups: [ DelegateModelGroup {
     id: unsortedItems
     name: "unsorted"
     includeByDefault: true
     onChanged: sort(lessThan)
   },DelegateModelGroup {
      id: visibleItems
      name: "visible"
      includeByDefault: true
   }]
  filterOnGroup: "visible"
}
