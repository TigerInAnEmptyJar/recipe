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

  Rectangle{
    anchors.fill: parent
    border{
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
          Layout.maximumHeight: Common.textHeight *2
          font.pointSize: Common.fontSize *2
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
          Layout.fillHeight: true
          Layout.fillWidth: true
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
              border{
                color: Common.borderColor[context]
                width: Common.borderWidth -2
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
                    Item {
                      Layout.fillWidth: true
                    }
                    Rectangle{
                      Layout.maximumHeight: Common.textHeight
                      Layout.minimumHeight: Common.textHeight
                      Layout.maximumWidth: ingredientInputBox.width /4
                      Layout.minimumWidth: ingredientInputBox.width /4
                      border{
                        color: Common.borderColor[context]
                        width: Common.borderWidth -2
                      }
                      color: Common.backgroundColor[context]

                      TextInput {
                        anchors.fill: parent
                        anchors.leftMargin: Common.margin
                        text: value
                        validator: DoubleValidator{bottom: 0; top: 1000; decimals: 3}
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
            Layout.maximumHeight: ingredientView.height
            Layout.maximumWidth: ingredientView.width
            Layout.minimumHeight: Common.textHeight
            Layout.minimumWidth: Common.textHeight
            Layout.fillHeight: true
            Layout.fillWidth: true
            fillMode: Image.PreserveAspectFit
            MouseArea {
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
              width: Common.borderWidth -2
            }
            TextEdit {
              id: objectInstructionInput
              anchors.fill: parent
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
              width: Common.borderWidth -2
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
        RowLayout { // Tags
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight
          Text {
            text: qsTr("Tags")
            font.pointSize: Common.fontSize
            font.bold: true
            Layout.maximumHeight: Common.textHeight
            Layout.maximumWidth: implicitWidth
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          ListView {
            id: tagview
            Layout.fillWidth: true
            Layout.preferredHeight: Common.textHeight
            orientation: ListView.Horizontal
            delegate: Rectangle {
              height: tagview.height
              width: tagText.implicitWidth * 1.5
              radius: Common.spacing
              border {
                color: Common.borderColor[context]
                width: Common.borderWidth -2
              }
              color: Common.buttonColor[context]
              RowLayout {
                anchors.fill: parent
                Text {
                  id: tagText
                  Layout.fillHeight: true
                  Layout.fillWidth: true
                  horizontalAlignment: Text.AlignHCenter
                  verticalAlignment: Text.AlignVCenter
                  text: modelData
                  color: Common.buttonTextColor[context]
                }
                RButton {
                  context: 1
                  text: "x"
                  Layout.maximumWidth: Common.textHeight
                  Layout.maximumHeight: Common.textHeight * 0.5
                  onClicked: {
                    recipes.removeTag(recipeView.objectIndex, modelData)
                    tagview.model = object.tags
                  }
                }
              }
            }
          }
          TextInput {
            id: tagInput
            Layout.maximumHeight: Common.textHeight
            Layout.minimumWidth: Common.textHeight
            Rectangle {
              Layout.preferredWidth: tagInput.implicitWidth
              color: "white"
              anchors.fill: parent
              z: -1
            }
          }

          RButton {
            id: plusTagButton
            context: 1
            text: "+"
            Layout.maximumWidth: Common.textHeight
            Layout.maximumHeight: Common.textHeight * 0.5
            onClicked: {
              if (tagInput.text != "") {
                recipes.addTag(recipeView.objectIndex, tagInput.text)
                tagview.model = object.tags
              }
            }
          }
        }
        RowLayout { // Eaters
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight
          Text {
            text: qsTr("Eaters")
            font.pointSize: Common.fontSize
            font.bold: true
            Layout.maximumHeight: Common.textHeight
            Layout.maximumWidth: implicitWidth
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          ListView {
            id: eaterview
            Layout.fillWidth: true
            Layout.preferredHeight: Common.textHeight
            orientation: ListView.Horizontal
            delegate: Rectangle {
              height: eaterview.height
              width: eaterText.implicitWidth * 1.5
              radius: Common.spacing
              border{
                color: Common.borderColor[context]
                width: Common.borderWidth -2
              }
              color: Common.buttonColor[context]
              RowLayout {
                anchors.fill: parent
                Text {
                  id: eaterText
                  Layout.fillHeight: true
                  Layout.fillWidth: true
                  horizontalAlignment: Text.AlignHCenter
                  verticalAlignment: Text.AlignVCenter
                  text: modelData
                  color: Common.buttonTextColor[context]
                }
                RButton {
                  context: 1
                  text: "x"
                  Layout.maximumWidth: Common.textHeight
                  Layout.maximumHeight: Common.textHeight * 0.5
                  onClicked: {
                    recipes.removeEater(recipeView.objectIndex, modelData)
                    eaterview.model = object.eaters
                    console.log("remove " + modelData)
                  }
                }
              }
            }
          }
          TextInput {
            id: eaterInput
            Layout.maximumHeight: Common.textHeight
            Layout.minimumWidth: Common.textHeight
            Rectangle {
              Layout.preferredWidth: tagInput.implicitWidth
              color: "white"
              anchors.fill: parent
              z: -1
            }
          }

          RButton {
            id: plusEaterButton
            context: 1
            text: "+"
            Layout.maximumWidth: Common.textHeight
            Layout.maximumHeight: Common.textHeight * 0.5
            onClicked: {
              if (eaterInput.text !== ""){
                recipes.addEater(recipeView.objectIndex, eaterInput.text)
                eaterview.model = object.eaters
              }
            }
          }
        }

        RButton {
          id: applyButton
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
            object.fat = objectFatInput.value
            object.protein = objectProteinInput.value
            object.carbohydrates = objectCarbsInput.value
            object.source = objectSourceInput.text
            applyClicked()
          }
        }
      }
      IngredientList{
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
      }
      objectInstructionInput.text = object.instructions
      objectCaloriesInput.value = object.calories
      objectJoulesInput.value = object.joules
      objectFatInput.value = object.fat
      objectProteinInput.value = object.protein
      objectCarbsInput.value = object.carbohydrates
      objectSourceInput.text = object.source
      tagview.model = object.tags
      eaterview.model = object.eaters
      ingredientView.model = object.ingredient
    }
  }
  visible: object !== undefined
}