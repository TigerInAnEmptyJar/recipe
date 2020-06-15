import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import "common/"

Item {
  id: recipeView
  property var object
  property bool readonly: true
  property int context: 1
  signal applyClicked()

  Rectangle{
    anchors.fill: parent
    border{
      color: Common.borderColor[context]
      width: Common.borderWidth
    }
    color: Common.backgroundColor[context]
    ColumnLayout {
      anchors.fill: parent
      anchors.margins: Common.margin
      StackLayout {
        height: Common.textHeight
        Layout.fillWidth: true
        Layout.maximumHeight: Common.textHeight
        currentIndex: recipeView.readonly ? 0 : 1
        Text {
          id: objectTitle
          font.pointSize: Common.fontSize *2
          verticalAlignment: Text.AlignVCenter
          horizontalAlignment: Text.AlignLeft
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight *2
        }
        TextInput {
          id: objectTitleInput
          font.pointSize: Common.fontSize *2
          verticalAlignment: TextInput.AlignVCenter
          horizontalAlignment: TextInput.AlignLeft
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight *2
        }
      }
      RowLayout{
        Text {
          text: qsTr("Type")
          font.pointSize: Common.fontSize
          Layout.maximumHeight: Common.textHeight
          Layout.fillWidth: true
          horizontalAlignment: Text.AlignLeft
          verticalAlignment: Text.AlignVCenter
        }
        StackLayout {
          Layout.maximumHeight: Common.textHeight
          Layout.fillWidth: true
          currentIndex: recipeView.readonly ? 0 : 1
          Text {
            id: objectType
            font.pointSize: Common.fontSize
            Layout.fillWidth: true
            Layout.maximumHeight: Common.textHeight
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          ComboBox {
            id: objectTypeInput
            font.pointSize: Common.fontSize
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            model: recipes.meal_types()
          }
        }
      }
      RowLayout{
        Text {
          text: qsTr("Servings")
          font.pointSize: Common.fontSize
          font.bold: true
          Layout.maximumHeight: Common.textHeight
          Layout.fillWidth: true
          horizontalAlignment: Text.AlignLeft
          verticalAlignment: Text.AlignVCenter
        }
        StackLayout {
          height: Common.textHeight
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight
          currentIndex: recipeView.readonly ? 0 : 1
          Text {
            id: objectServings
            font.pointSize: Common.fontSize
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            Layout.fillWidth: true
            Layout.maximumHeight: Common.textHeight
          }
          SpinBox {
            id: objectServingsInput
            Layout.fillWidth: true
            Layout.maximumHeight: Common.textHeight
          }
        }
      }
      RowLayout{
        Layout.fillHeight: true
        Layout.fillWidth: true
        ColumnLayout{
          Text {
            text: qsTr("Ingredients")
            font.pointSize: Common.fontSize
            font.bold: true
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          ListView{
            id: ingredientView
            Layout.fillHeight: true
            Layout.fillWidth: true
          }
        }
        Image {
          id: objectImage
          property string pathName: ""
          Layout.maximumHeight: ingredientView.height
          Layout.maximumWidth: ingredientView.width
          fillMode: Image.PreserveAspectFit
          MouseArea{
            anchors.fill: parent
            FileDialog {
              id: loadDialog
              title: qsTr("Please choose an image file")
              nameFilters: [ "Image files (*.jpeg, *.jpg)" ]
              folder: "file://" + recipes.databasePath()+"/"
              selectExisting: true
              selectMultiple: false
              onAccepted: {
                objectImage.pathName = loadDialog.fileUrl
                close()
              }
              onRejected: {
                close()
              }
            }
            onClicked: {
              if (!recipeView.readonly){
                loadDialog.folder = "file://" + recipes.databasePath() + "/"
                loadDialog.open()
              }
            }
          }
        }
      }
      Text {
        text: qsTr("Instructions")
        font.pointSize: Common.fontSize
        font.bold: true
        Layout.maximumHeight: Common.textHeight
        Layout.fillWidth: true
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
      }
      StackLayout {
        Layout.maximumHeight: Common.textHeight
        Layout.fillWidth: true
        currentIndex: recipeView.readonly ? 0 : 1
        Text {
          id: objectInstructions
          font.pointSize: Common.fontSize
          Layout.fillWidth: true
          horizontalAlignment: Text.AlignLeft
        }
        TextEdit {
          id: objectInstructionInput
          font.pointSize: Common.fontSize
          Layout.fillWidth: true
          Layout.fillHeight: true
        }
      }
      RowLayout{
        Layout.fillWidth: true
        Layout.maximumHeight: Common.textHeight
        RowLayout{
          Layout.fillWidth: true
          Layout.fillHeight: true
          Text {
            text: qsTr("Calories")
            font.pointSize: Common.fontSize
            Layout.maximumHeight: Common.textHeight
            Layout.minimumWidth: implicitWidth
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          StackLayout {
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            currentIndex: recipeView.readonly ? 0 : 1
            Text {
              id: objectCalories
              font.pointSize: Common.fontSize
              Layout.fillWidth: true
              horizontalAlignment: Text.AlignLeft
              verticalAlignment: Text.AlignVCenter
            }
            SpinBox {
              id: objectCaloriesInput
              font.pointSize: Common.fontSize
              Layout.fillWidth: true
            }
          }
        }
        RowLayout{
          Layout.fillWidth: true
          Layout.fillHeight: true
          Text {
            text: qsTr("Joules")
            font.pointSize: Common.fontSize
            Layout.maximumHeight: Common.textHeight
            Layout.minimumWidth: implicitWidth
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          StackLayout {
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            currentIndex: recipeView.readonly ? 0 : 1
            Text {
              id: objectJoules
              font.pointSize: Common.fontSize
              Layout.fillWidth: true
              horizontalAlignment: Text.AlignLeft
              verticalAlignment: Text.AlignVCenter
            }
            SpinBox {
              id: objectJoulesInput
              font.pointSize: Common.fontSize
              Layout.fillWidth: true
            }
          }
        }
        RowLayout{
          Layout.fillWidth: true
          Layout.fillHeight: true
          Text {
            text: qsTr("Fat[g]")
            font.pointSize: Common.fontSize
            Layout.maximumHeight: Common.textHeight
            Layout.minimumWidth: implicitWidth
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          StackLayout {
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            currentIndex: recipeView.readonly ? 0 : 1
            Text {
              id: objectFat
              font.pointSize: Common.fontSize
              Layout.fillWidth: true
              horizontalAlignment: Text.AlignLeft
              verticalAlignment: Text.AlignVCenter
            }
            SpinBox {
              id: objectFatInput
              font.pointSize: Common.fontSize
              Layout.fillWidth: true
            }
          }
        }
        RowLayout{
          Layout.fillWidth: true
          Layout.fillHeight: true
          Text {
            text: qsTr("Protein[g]")
            font.pointSize: Common.fontSize
            Layout.minimumWidth: implicitWidth
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          StackLayout {
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            currentIndex: recipeView.readonly ? 0 : 1
            Text {
              id: objectProtein
              font.pointSize: Common.fontSize
              Layout.fillWidth: true
              horizontalAlignment: Text.AlignLeft
              verticalAlignment: Text.AlignVCenter
            }
            SpinBox {
              id: objectProteinInput
              font.pointSize: Common.fontSize
              Layout.fillWidth: true
            }
          }
        }
        RowLayout{
          Layout.fillWidth: true
          Layout.fillHeight: true
          Text {
            text: qsTr("Carbohydrate[g]")
            font.pointSize: Common.fontSize
            Layout.maximumHeight: Common.textHeight
            Layout.minimumWidth: implicitWidth
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          StackLayout {
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            currentIndex: recipeView.readonly ? 0 : 1
            Text {
              id: objectCarbs
              font.pointSize: Common.fontSize
              Layout.fillWidth: true
              verticalAlignment: Text.AlignVCenter
              horizontalAlignment: Text.AlignLeft
            }
            SpinBox {
              id: objectCarbsInput
              font.pointSize: Common.fontSize
              Layout.fillWidth: true
            }
          }
        }
      }
      RowLayout{
        Layout.fillWidth: true
        Layout.maximumHeight: Common.textHeight
        Text {
          text: qsTr("Source")
          font.pointSize: Common.fontSize
          font.bold: true
          Layout.maximumHeight: Common.textHeight
          Layout.fillWidth: true
          horizontalAlignment: Text.AlignLeft
          verticalAlignment: Text.AlignVCenter
        }
        StackLayout {
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight
          currentIndex: recipeView.readonly ? 0 : 1
          Text {
            id: objectSource
            font.pointSize: Common.fontSize
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            Layout.fillWidth: true
            Layout.maximumHeight: Common.textHeight
          }
          TextInput {
            id: objectSourceInput
            font.pointSize: Common.fontSize
            verticalAlignment: TextInput.AlignVCenter
            horizontalAlignment: TextInput.AlignLeft
            Layout.fillWidth: true
            Layout.maximumHeight: Common.textHeight
          }
        }
      }
      RButton {
        id: applyButton
        visible: !recipeView.readonly
        context: 1
        text: qsTr("Apply")
        Layout.fillWidth: true
        Layout.preferredHeight: Common.textHeight
        onClicked: {
          object.title = objectTitleInput.text
          object.meal_type =  objectTypeInput.currentIndex
          object.servings = objectServingsInput.value
          object.image_path = objectImage.pathName
          object.instructions = objectInstructionInput.text
          object.calories = objectCaloriesInput.value
          object.joules = objectJoulesInput.value
          object.fats = objectFatInput.value
          object.proteins = objectProteinInput.value
          object.carbohydrates = objectCarbsInput.value
          object.source = objectSourceInput.text
          applyClicked()
        }
      }
    }
  }
  onObjectChanged: {
    if (object !== undefined) {
      objectTitle.text = object.title
      objectTitleInput.text = object.title

      objectType.text = object.meal_type
      objectTypeInput.currentIndex = objectTypeInput.find(object.meal_type)
      objectServings.text = object.servings
      objectServingsInput.value = object.servings
      objectImage.pathName = object.image_path
      objectImage.source = object.image
      objectInstructions.text = object.instructions
      objectInstructionInput.text = object.instructions
      objectCalories.text = object.calories
      objectCaloriesInput.value = object.calories
      objectJoules.text = object.joules
      objectJoulesInput.value = object.joules
      objectFat.text = object.fat
      objectFatInput.value = object.fat
      objectProtein.text = object.protein
      objectProteinInput.value = object.protein
      objectCarbs.text = object.carbohydrates
      objectCarbsInput.value = object.carbohydrates
      objectSource.text = object.source
      objectSourceInput.text = object.source
      recipeView.readonly = true
    }
  }
  visible: object !== undefined
}
