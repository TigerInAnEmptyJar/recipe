import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import "common/"

Item {
  id: recipeView
  property var object
  property var objectIndex
  property int context: 1
  signal applyClicked()

  Rectangle {
    anchors.fill: parent
    border {
      color: Common.borderColor[context]
      width: Common.borderWidth
    }
    color: Common.backgroundColor[context]
    RowLayout {
      anchors.fill: parent
      anchors.margins: Common.margin
      ColumnLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        TextInput {
          id: objectTitleInput
          height: Common.textHeight
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight * 2
          font.pointSize: Common.fontSize * 2
          verticalAlignment: TextInput.AlignVCenter
          horizontalAlignment: TextInput.AlignLeft
        }
        RowLayout { // Type
          Text {
            text: qsTr("Type")
            font.pointSize: Common.fontSize
            font.bold: true
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          ComboBox {
            id: objectTypeInput
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            font.pointSize: Common.fontSize
            model: recipes.meal_types()
          }
        }
        RowLayout { // Servings
          Text {
            text: qsTr("Servings")
            font.pointSize: Common.fontSize
            font.bold: true
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          SpinBox {
            id: objectServingsInput
            height: Common.textHeight
            Layout.fillWidth: true
            Layout.maximumHeight: Common.textHeight
          }
        }
        RowLayout { // Ingredients - Image
          id: iiLayout
          Layout.fillHeight: true
          Layout.fillWidth: true
          Layout.minimumHeight: Common.textHeight
          Layout.maximumHeight: recipeView.height / 4
          ColumnLayout { // Ingredients
            Layout.fillHeight: true
            Layout.fillWidth: true
            Text {
              text: qsTr("Ingredients")
              font.pointSize: Common.fontSize
              font.bold: true
              Layout.maximumHeight: Common.textHeight
              Layout.fillWidth: true
              horizontalAlignment: Text.AlignLeft
              verticalAlignment: Text.AlignVCenter
            }
            Rectangle {
              Layout.fillHeight: true
              Layout.fillWidth: true
              Layout.preferredHeight: Common.textHeight * ingredientView.count
              color: Common.backgroundColor[context]
              border {
                color: Common.borderColor[context]
                width: Common.borderWidth - 2
              }
              DropArea {
                anchors.fill: parent
                onDropped: recipes.addIngredient(objectIndex, drag.source.id)
              }
              ListView {
                id: ingredientView
                MessageDialog {
                  id: removeDialog
                  property int index
                  title: qsTr("Remove ingredient")
                  standardButtons: StandardButton.Yes | StandardButton.No
                  onYes: {
                    visible = false
                    ingredientView.model.deleteItem(index)
                  }
                }

                anchors.fill: parent
                anchors.margins: Common.margin
                orientation: ListView.Vertical
                clip: true
                delegate: Rectangle {
                  id: ingredientDelegate
                  height: Common.textHeight
                  width: ingredientView.width
                  color: Common.backgroundColor[context]
                  RowLayout {
                    anchors.fill: parent
                    Image {
                      Layout.maximumHeight: Common.textHeight
                      Layout.maximumWidth: Common.textHeight
                      Layout.minimumHeight: Common.textHeight
                      Layout.minimumWidth: Common.textHeight
                      fillMode: Image.PreserveAspectFit
                      source: image !== undefined ? image : ""
                      sourceSize.width: 256
                      cache: false
                    }
                    Text {
                      text: name
                      Layout.fillHeight: true
                      Layout.fillWidth: true
                      verticalAlignment: Text.AlignVCenter
                      horizontalAlignment: Text.AlignLeft
                      color: Common.textColor[context]
                      font.pointSize: Common.fontSize
                      MouseArea {
                        anchors.fill: parent
                        onDoubleClicked: {
                          removeDialog.visible = true
                          removeDialog.index = index
                          removeDialog.text = qsTr("Remove ingredient %1").arg(name)
                        }
                      }
                    }
                    Item{Layout.fillWidth: true}
                    Rectangle {
                      Layout.maximumHeight: Common.textHeight
                      Layout.minimumHeight: Common.textHeight
                      Layout.maximumWidth: ingredientInputBox.width / 4
                      Layout.minimumWidth: ingredientInputBox.width / 4
                      border {
                        color: Common.borderColor[context]
                        width: Common.borderWidth - 2
                      }
                      color: Common.backgroundColor[context]

                      TextInput {
                        anchors.fill: parent
                        anchors.leftMargin: Common.margin
                        text: value
                        color: Common.textColor[context]
                        validator: DoubleValidator {bottom: 0; top: 1000; decimals: 3}
                        font.pointSize: Common.fontSize
                        onAccepted: value = text
                      }
                    }
                    ComboBox {
                      id: ingredientInputBox
                      Layout.maximumHeight: Common.textHeight
                      Layout.maximumWidth: ingredientInputBox.implicitWidth
                      Layout.minimumWidth: ingredientInputBox.implicitWidth
                      font.pointSize: Common.fontSize
                      currentIndex: amountIndex
                      model: ingredientView.model.amountList()
                      onActivated: amount = currentIndex
                    }
                  }
                }
              }
            }
          }
          Image {
            id: objectImage
            property string pathName: ""
            Layout.maximumHeight: iiLayout.height
            Layout.maximumWidth: iiLayout.height
            Layout.minimumHeight: Common.textHeight
            Layout.minimumWidth: Common.textHeight
            fillMode: Image.PreserveAspectFit
            cache: false
            MouseArea {
              anchors.fill: parent
              FileDialog {
                id: loadDialog
                title: qsTr("Please choose an image file")
                nameFilters: ["Image files (*.jpeg, *.jpg)"]
                folder: "file://" + recipes.databasePath() + "/"
                selectExisting: true
                selectMultiple: false
                onAccepted: {
                  objectImage.pathName = loadDialog.fileUrl
                  close()
                }
                onRejected: close()
              }
              onClicked: {
                loadDialog.folder = "file://" + recipes.databasePath() + "/"
                loadDialog.open()
              }
            }
          }
        }
        ColumnLayout { // Instructions
          Layout.fillHeight: true
          Layout.fillWidth: true
          Layout.minimumHeight: objectInstructionInput.implicitHeight + instructionsTitle.implicitHeight
          Text {
            id: instructionsTitle
            text: qsTr("Instructions")
            font.pointSize: Common.fontSize
            font.bold: true
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.minimumHeight: objectInstructionInput.implicitHeight
            clip: true
            color: Common.backgroundColor[context]
            border {
              color: Common.borderColor[context]
              width: Common.borderWidth - 2
            }
            TextEdit {
              id: objectInstructionInput
              anchors.fill: parent
              color: Common.textColor[context]
              font.pointSize: Common.fontSize
              wrapMode: TextEdit.WordWrap
            }
          }
        }
        RowLayout { // Nutrition
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight
          Text {
            text: qsTr("Calories")
            font.pointSize: Common.fontSize
            Layout.maximumHeight: Common.textHeight
            Layout.minimumWidth: implicitWidth
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          SpinBox {
            id: objectCaloriesInput
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            font.pointSize: Common.fontSize
            editable: true
            to: 3000
          }
          Text {
            text: qsTr("Joules")
            font.pointSize: Common.fontSize
            Layout.maximumHeight: Common.textHeight
            Layout.minimumWidth: implicitWidth
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          SpinBox {
            id: objectJoulesInput
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            font.pointSize: Common.fontSize
            to: 12000
            editable: true
          }
          Text {
            text: qsTr("Fat[g]")
            font.pointSize: Common.fontSize
            Layout.maximumHeight: Common.textHeight
            Layout.minimumWidth: implicitWidth
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          SpinBox {
            id: objectFatInput
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            font.pointSize: Common.fontSize
            editable: true
          }
          Text {
            text: qsTr("Protein[g]")
            font.pointSize: Common.fontSize
            Layout.minimumWidth: implicitWidth
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          SpinBox {
            id: objectProteinInput
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            font.pointSize: Common.fontSize
            editable: true
          }
          Text {
            text: qsTr("Carbohydrate[g]")
            font.pointSize: Common.fontSize
            Layout.maximumHeight: Common.textHeight
            Layout.minimumWidth: implicitWidth
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          SpinBox {
            id: objectCarbsInput
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            font.pointSize: Common.fontSize
            editable: true
          }
        }
        RowLayout { // Source
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight
          Text {
            text: qsTr("Source")
            font.pointSize: Common.fontSize
            font.bold: true
            Layout.maximumHeight: Common.textHeight
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          Rectangle {
            Layout.fillWidth: true
            Layout.maximumHeight: Common.textHeight
            Layout.minimumHeight: Common.textHeight
            border {
              color: Common.borderColor[context]
              width: Common.borderWidth - 2
            }
            TextInput {
              id: objectSourceInput
              anchors.fill: parent
              font.pointSize: Common.fontSize
              verticalAlignment: TextInput.AlignVCenter
              horizontalAlignment: TextInput.AlignLeft
            }
          }
        }
        RStringListView {
          id: tagView
          title: qsTr("Tags")
          readonly: false
          context: recipeEditView.context
          onRemove: {
            recipes.removeTag(recipeView.objectIndex, element)
            tagView.model = object.tags
          }
          onAdd: {
            recipes.addTag(recipeView.objectIndex, element)
            tagView.model = object.tags
          }
        }
        RStringListView {
          id: eaterView
          title: qsTr("Eaters")
          readonly: false
          context: recipeEditView.context
          onRemove: {
            recipes.removeEater(recipeView.objectIndex, element)
            eaterView.model = object.eaters
          }
          onAdd: {
            recipes.addEater(recipeView.objectIndex, element)
            eaterView.model = object.eaters
          }
        }

        RButton {
        id: applyButton
        context: 1
        text: qsTr("Apply")
        Layout.fillWidth: true
        Layout.preferredHeight: Common.textHeight
        onClicked: {
            object.servings = objectServingsInput.value
            object.image_path = objectImage.pathName
            object.instructions = objectInstructionInput.text
            object.calories = objectCaloriesInput.value
            object.joules = objectJoulesInput.value
            object.fat = objectFatInput.value
            object.protein = objectProteinInput.value
            object.carbohydrates = objectCarbsInput.value
            object.source = objectSourceInput .text
            if (object.title !== objectTitleInput.text || object.meal_type !== objectTypeInput.currentText) {
              object.title = objectTitleInput.text
              object.meal_type = objectTypeInput.currentIndex
              applyClicked()
            }
          }
        }
      }
      IngredientList {
        Layout.fillHeight: true
        Layout.preferredWidth: 200
      }
    }
  }
  onObjectChanged: {
    if (object !== undefined) {
      objectTitleInput.text = object.title

      objectTypeInput.currentIndex = objectTypeInput.find(object.meal_type)
      objectServingsInput.value = object.servings
      if (object.image_path !== undefined) {
        objectImage.pathName = object.image_path
        objectImage.source = object.image
      } else {
        objectImage.pathName = ""
        objectImage.source = ""
      }
      objectInstructionInput.text = object.instructions
      objectCaloriesInput.value = object.calories
      objectJoulesInput.value = object.joules
      objectFatInput.value = object.fat
      objectProteinInput.value = object.protein
      objectCarbsInput.value = object.carbohydrates
      objectSourceInput.text = object.source
      tagView.model = object.tags
      eaterView.model = object.eaters
      ingredientView.model = object.ingredient
    }
  }
  visible: object !== undefined
}
