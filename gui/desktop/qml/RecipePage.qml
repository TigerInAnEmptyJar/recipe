import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 6.8

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
      onCurrentObjectChanged: viewStack.currentIndex = 1
    }
    ColumnLayout{
      IngredientControls{
        Layout.fillWidth: true
        height: 100
        context: page.context
        model: recipes
        currentObject: recipeList.currentObject
        text: qsTr("recipe")
        FileDialog {
          id: exportDialog
          title: qsTr("Please choose a path to export to")
          nameFilters: ["Tex files (*.tex)"]
          fileMode: FileDialog.SaveFile
          currentFolder: "file://" + recipes.databasePath() + "/"
          onAccepted: {
            recipes.bookRequested(selectedFile)
            close()
          }
          onRejected: close()
        }

        onEditRequested: viewStack.currentIndex = 0
        onNewRequested: recipes.addItem()
        onDeleteRequested: recipes.deleteItem(recipeList.currentIndex)
        onBookRequested: exportDialog.open()
      }

      StackLayout {
        id: viewStack
        currentIndex: 1
        visible:recipeList.currentObject !== undefined
        clip: true
        RecipeEditView {
          id: recipeEditView
          Layout.fillWidth: true
          Layout.fillHeight: true
          object: recipeList.currentObject
          objectIndex: recipeList.currentIndex
          context: 1
          onApplyClicked: {
            recipeList.unsetCurrentObject()
            viewStack.currentIndex = 1
          }
        }

        RecipeReadonlyView {
          id: recipeView
          Layout.fillWidth: true
          Layout.fillHeight: true
          object: recipeList.currentObject
          objectIndex: recipeList.currentIndex
          context: 1
        }
      }
      Item {
        visible: recipeList.currentObject === undefined
        Layout.fillWidth: true
        Layout.fillHeight: true
      }
    }
  }
}
