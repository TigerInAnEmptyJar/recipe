import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import "common/"

Rectangle {
  id: page
  property int context: 0
  border{
    color: Common.borderColor[context]
    width: Common.borderWidth
  }

  RowLayout {
    anchors.fill: parent
    IngredientList{
      id: ingredientList
      width: 200
      Layout.fillHeight: true
      onCurrentObjectChanged: viewStack.currentIndex = 1
    }
    ColumnLayout{
      IngredientControls{
        Layout.fillWidth: true
        height: 100
        context: page.context
        model: ingredients
        currentObject: ingredientList.currentObject
        text: qsTr("ingredient")
        onEditRequested: viewStack.currentIndex = 0
        onNewRequested: ingredients.addItem()
        onDeleteRequested: ingredients.deleteItem(ingredientList.currentIndex)
      }

      StackLayout{
        id: viewStack
        currentIndex: 1
        visible: ingredientList.currentObject !== undefined
        IngredientEditView {
          id: ingredientEditView
          Layout.fillWidth: true
          Layout.fillHeight: true
          object: ingredientList.currentObject
          onApplyClicked: {
            ingredientList.unsetCurrentObject()
            viewStack.currentIndex = 1
          }
        }
        IngredientReadonlyView {
          id: ingredientView
          Layout.fillWidth: true
          Layout.fillHeight: true
          object: ingredientList.currentObject
        }
      }
      Item {
        visible: ingredientList.currentObject === undefined
        Layout.fillWidth: true
        Layout.fillHeight: true
      }
    }
  }
}
