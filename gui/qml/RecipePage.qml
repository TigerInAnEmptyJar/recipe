import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import "common/"

Rectangle {
  id: page
  property int context: 1
  border{
    color: Common.borderColor[context]
    width: Common.borderWidth
  }

  RowLayout {
    anchors.fill: parent
    RecipeList{
      id: recipeList
      context: page.context
      width: 200
      Layout.fillHeight: true
    }
    ColumnLayout{
      IngredientControls{
        Layout.fillWidth: true
        height: 100
        context: page.context
        model: recipes
        currentObject: recipeList.currentObject
        text: "recipe"
        onEditRequested: recipeView.readonly = false
        onNewRequested: recipes.addItem()
        onDeleteRequested: recipes.deleteItem(recipeList.currentIndex)
      }

      RecipeView {
        id: recipeView
        Layout.fillWidth: true
        Layout.fillHeight: true
        object: recipeList.currentObject
        objectIndex: recipeList.currentIndex
        context: 1
      }
      Item {
        visible: recipeList.currentObject === undefined
        Layout.fillWidth: true
        Layout.fillHeight: true
      }
    }
  }
}
