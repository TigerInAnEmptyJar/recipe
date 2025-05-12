import QtQuick 2.3
import QtQuick.Controls 2.5
import QtQuick.Dialogs 6.8
import QtQuick.Layouts 1.3
import QtQml.Models 2.3

import "common/"

Dialog {
  id: filterDialog
  title: "Filter Recipes"

  property var defaultFilter: function(item) {return true; }
  property var defaultLess: function(left, right) {return left.meal_type === right.meal_type ? left.title < right.title : left.meal_type < right.meal_type}

  property var filterAcceptsItem: defaultFilter
  property var lessThan: defaultLess

  property int context: Common.context_recipe

  ListModel {
    id: allTagsModel
  }
  ListModel {
    id: anyTagsModel
  }
  ListModel {
    id: anyIngredientModel
  }
  DelegateModel {
    id: delegate
  }

  contentItem: Rectangle {
    color: Common.backgroundColor[context]
    implicitWidth: topLayout.implicitWidth
    implicitHeight: topLayout.implicitHeight
    width: filterDialog.width
    height: filterDialog.height

    RowLayout {
      id: topLayout
      anchors.centerIn: parent
      ColumnLayout {
        id: contentLayout
        Layout.fillWidth: true
        // Filters
        Text {
          text: qsTr("Filters:")
          Layout.fillWidth: true
        }

        RStringListView {
          id: allTags
          function accepts(item) {
            if (model.rowCount() === 0) return true
            for (var i = 0; i < model.rowCount(); i++)
              if (!item.tags.includes(model.get(i).name))
                return false
            return true
          }

          readonly: false
          title: qsTr("AllOf tags:")
          context: filterDialog.context
          model: allTagsModel
          Layout.fillWidth: true
          onAdd: {
            model.append({"name":element})
          }
        }

        RStringListView {
          id: anyTags
          function accepts(item) {
            if (model.rowCount() === 0) return true
            for (var i = 0; i < model.rowCount(); i++)
              if (item.tags.includes(model.get(i).name))
                return true
            return false
          }
          readonly: false
          title: qsTr("AnyOf tags:")
          context: filterDialog.context
          model: anyTagsModel
          Layout.fillWidth: true
          onAdd: {
            model.append({"name":element})
          }
        }
        RStringListView {
          id: anyIngredient

          function accepts(item) {
            if (model.rowCount() === 0) return true
            delegate.model = item.ingredient
            for (var i = 0; i < model.rowCount(); i++)
              for (var j = 0; j < item.ingredient.rowCount(); ++j)
                if (delegate.items.get(j).model.id === model.get(i).identifier)
                  return true
            return false
          }
          readonly: false
          title: qsTr("AnyOf ingredient:")
          context: filterDialog.context
          model: anyIngredientModel
          Layout.fillWidth: true
          getDisplay: function(item){return anyIngredientModel.get(item).name}
          onRemove: {
            anyIngredientModel.remove({"name":element})
          }
          DropArea {
            anchors.fill: parent
            onDropped: {
              console.log("name:" + drag.source.name + ", id:" + drag.source.id)
              anyIngredientModel.append({"name": drag.source.name, "identifier":drag.source.id})
            }
          }
        }
        // Sorting
        Text {
          text: qsTr("Sorting:")
          Layout.fillWidth: true
        }
        GridLayout {
          Layout.fillHeight: true
          Layout.fillWidth: true
          columns: 4
          ButtonGroup { id: sortGroup }
          RadioButton {
            id: nameLess
            text: qsTr("By name ascending")
            ButtonGroup.group: sortGroup
            checked: true
            function filter(a, b) {
              return a.title < b.title
            }
          }
          RadioButton {
            id: nameMore
            text: qsTr("By name descending")
            ButtonGroup.group: sortGroup
            function filter(a, b) {
              return a.title > b.title
            }
          }
          RadioButton {
            id: caloriesLess
            text: qsTr("Less calories")
            ButtonGroup.group: sortGroup
            function filter(a, b) {
              return a.calories < b.calories
            }
          }
          RadioButton {
            id: caloriesMore
            text: qsTr("More calories")
            ButtonGroup.group: sortGroup
            function filter(a, b) {
              return a.calories > b.calories
            }
          }
          RadioButton {
            id: carbsLess
            text: qsTr("Less carbohydrates")
            ButtonGroup.group: sortGroup
            function filter(a, b) {
              return a.carbohydrates < b.carbohydrates
            }
          }
          RadioButton {
            id: carbsMore
            text: qsTr("More carbohydrates")
            ButtonGroup.group: sortGroup
            function filter(a, b) {
              return a.carbohydrates > b.carbohydrates
            }
          }
          RadioButton {
            id: proteinLess
            text: qsTr("Less protein")
            ButtonGroup.group: sortGroup
            function filter(a, b) {
              return a.protein < b.protein
            }
          }
          RadioButton {
            id: proteinMore
            text: qsTr("More protein")
            ButtonGroup.group: sortGroup
            function filter(a, b) {
              return a.protein > b.protein
            }
          }
          RadioButton {
            id: fatLess
            text: qsTr("Less fat")
            ButtonGroup.group: sortGroup
            function filter(a, b) {
              return a.fat < b.fat
            }
          }
          RadioButton {
            id: fatMore
            text: qsTr("More fat")
            ButtonGroup.group: sortGroup
            function filter(a, b) {
              return a.fat > b.fat
            }
          }
          RadioButton {
            id: fast
            text: qsTr("Fast recipes")
            ButtonGroup.group: sortGroup
            function filter(a, b) {
              return a.preparation_time < b.preparation_time
            }
          }
          RadioButton {
            id: slow
            text: qsTr("Slow recipes")
            ButtonGroup.group: sortGroup
            function filter(a, b) {
              return a.preparation_time > b.preparation_time
            }
          }
          RadioButton {
            id: ingredientsLess
            text: qsTr("Fewer ingredients")
            ButtonGroup.group: sortGroup
            function filter(a, b) {
              return a.ingredient.rowCount() < b.ingredient.rowCount()
            }
          }
          RadioButton {
            id: ingredientsMore
            text: qsTr("More ingredients")
            ButtonGroup.group: sortGroup
            function filter(a, b) {
              return a.ingredient.rowCount() > b.ingredient.rowCount()
            }
          }
        }
        DialogButtonBox {
          id: buttonBox
          position:   DialogButtonBox.Footer
          standardButtons: StandardButton.Apply | StandardButton.Reset | StandardButton.Cancel
          onApplied: filterDialog.apply()
          onReset: filterDialog.reset()
          onRejected: filterDialog.reject()
          width: contentLayout.width
        }
      }
      IngredientList {
        id: ingredientsList
        implicitWidth: 150
        Layout.fillHeight: true
      }
    }
  }
  standardButtons: StandardButton.Apply | StandardButton.Reset | StandardButton.Cancel
  onAccepted: {
    filterAcceptsItem = function(item) {
      if (!allTags.accepts(item)) return false
      if (!anyTags.accepts(item)) return false
      if (!anyIngredient.accepts(item)) return false
      return true
    }
    lessThan = function(left, right) {
      if (left.meal_type === right.meal_type)
        return sortGroup.checkedButton.filter(left, right)
      return left.meal_type < right.meal_type
    }
    close()
    accepted()
  }
  onReset: {
    filterAcceptsItem = defaultFilter
    lessThan = defaultLess
    close()
    accepted()
  }
  onRejected: {
    close();
  }
}
