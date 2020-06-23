import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import "common/"

Item {
  id: recipeView
  property var object
  property var objectIndex
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
    RowLayout {
      anchors.fill: parent
      anchors.margins: Common.margin
      ColumnLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        StackLayout { // Title
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
        RowLayout { // Type
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
                anchors.fill: parent
                anchors.margins: Common.margin
                orientation: ListView.Vertical
                delegate: Rectangle {
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
                      source: image
                    }
                    Text {
                      text: name
                      Layout.fillHeight: true
                      Layout.fillWidth: true
                      verticalAlignment: Text.AlignVCenter
                      horizontalAlignment: Text.AlignLeft
                      color: Common.textColor[context]
                      font.pointSize: Common.fontSize
                    }
                    StackLayout {
                      Layout.maximumHeight: Common.textHeight
                      Layout.fillWidth: true
                      currentIndex: recipeView.readonly ? 0 : 1
                      Text {
                        text: value
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        color: Common.textColor[context]
                        font.pointSize: Common.fontSize
                      }
                      TextInput {
                        text: value
                        validator: DoubleValidator{bottom: 0; top: 1000; decimals: 3}
                        font.pointSize: Common.fontSize
                        Layout.fillWidth: true
                        onAccepted: value = text
                      }
                    }
                    StackLayout {
                      Layout.maximumHeight: Common.textHeight
                      Layout.fillWidth: true
                      currentIndex: recipeView.readonly ? 0 : 1
                      Text {
                        text: amount
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        color: Common.textColor[context]
                        font.pointSize: Common.fontSize
                      }
                      ComboBox {
                        id: ingredientInputBox
                        font.pointSize: Common.fontSize
                        Layout.maximumHeight: Common.textHeight
                        Layout.fillWidth: true
                        currentIndex: amountIndex
                        model: ingredientView.model.amountList()
                        onActivated: amount = currentIndex
                      }
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
                if (!recipeView.readonly) {
                  loadDialog.folder = "file://" + recipes.databasePath() + "/"
                  loadDialog.open()
                }
              }
            }
          }
        }
        ColumnLayout { // Instructions
          Layout.fillHeight: true
          Layout.fillWidth: true
          Layout.minimumHeight: objectInstructions.implicitHeight + instructionsTitle.implicitHeight
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
            color: Common.backgroundColor[context]
            border {
              color: Common.borderColor[context]
              width: Common.borderWidth -2
            }
            StackLayout {
              anchors.fill: parent
              currentIndex: recipeView.readonly ? 0 : 1
              Text {
                id: objectInstructions
                font.pointSize: Common.fontSize
                Layout.fillWidth: true
                Layout.fillHeight: true
                horizontalAlignment: Text.AlignLeft
                Layout.minimumHeight: implicitHeight
              }
              TextEdit {
                id: objectInstructionInput
                font.pointSize: Common.fontSize
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: implicitHeight
              }
            }
          }
        }
        RowLayout { // Nutrition
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight
          RowLayout { // calories
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
          RowLayout { // KJoules
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
          RowLayout { // Fat
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
          RowLayout { // Proteins
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
          RowLayout { // Carbohydrates
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
        RowLayout { // Source
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
                  visible: !recipeView.readonly
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
            visible: !recipeView.readonly
            Rectangle {
              Layout.preferredWidth: tagInput.implicitWidth
              color: "white"
              anchors.fill: parent
              z: -1
            }
          }

          RButton {
            id: plusTagButton
            visible: !recipeView.readonly
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
                  visible: !recipeView.readonly
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
            visible: !recipeView.readonly
            Rectangle {
              Layout.preferredWidth: tagInput.implicitWidth
              color: "white"
              anchors.fill: parent
              z: -1
            }
          }

          RButton {
            id: plusEaterButton
            visible: !recipeView.readonly
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
      IngredientList{
        visible: !recipeView.readonly
        Layout.fillHeight: true
        Layout.preferredWidth: 200
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
      tagview.model = object.tags
      eaterview.model = object.eaters
      ingredientView.model = object.ingredient
      recipeView.readonly = true
    }
  }
  visible: object !== undefined
}
