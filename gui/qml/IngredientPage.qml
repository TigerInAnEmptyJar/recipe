import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3


Rectangle {
  id: page
  border{
    color: Qt.hsva(0.17, 1, 1)
    width: 3
  }

  RowLayout {
    anchors.fill: parent
    IngredientList{
      id: ingredientList
      width: 200
      Layout.fillHeight: true
    }
    ColumnLayout{
      IngredientControls{
        Layout.fillWidth: true
        height: 100
        context: 0
        model: ingredients
        currentObject: ingredientList.currentObject
        text: "ingredient"
      }

      IngredientView {
        id: ingredientView
        Layout.fillWidth: true
        Layout.fillHeight: true
        object: ingredientList.currentObject
      }
    }
  }
}
