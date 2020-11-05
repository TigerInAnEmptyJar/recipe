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
        Text { // Title
          id: objectTitle
          height: Common.textHeight
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight *2

          font.pointSize: Common.fontSize *2
          verticalAlignment: Text.AlignVCenter
          horizontalAlignment: Text.AlignLeft
        }
        RowLayout { // Type
          Layout.fillWidth: true
          Text {
            text: qsTr("Type")
            font.pointSize: Common.fontSize
            font.bold: true
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          Text {
            id: objectType
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            font.pointSize: Common.fontSize
            color: Common.textColor[context]
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
        }
        RowLayout { // Servings
          Layout.fillWidth: true
          Text {
            text: qsTr("Servings")
            font.pointSize: Common.fontSize
            font.bold: true
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
          Text {
            id: objectServings
            height: Common.textHeight
            Layout.fillWidth: true
            Layout.maximumHeight: Common.textHeight
            font.pointSize: Common.fontSize
            color: Common.textColor[context]
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
          }
        }
        RowLayout { // Ingredients - Image
          id: iiLayout
          Layout.fillHeight: true
          Layout.fillWidth: true
          Layout.minimumHeight: Common.textHeight
          Layout.maximumHeight: recipeView.height/4
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
                    }
                    Item {
                      Layout.fillWidth: true
                    }
                    Text {
                      Layout.maximumHeight: Common.textHeight

                      text: value
                      verticalAlignment: Text.AlignVCenter
                      horizontalAlignment: Text.AlignLeft
                      color: Common.textColor[context]
                      font.pointSize: Common.fontSize
                    }
                    Text {
                      Layout.maximumHeight: Common.textHeight
                      text: amount
                      verticalAlignment: Text.AlignVCenter
                      horizontalAlignment: Text.AlignLeft
                      color: Common.textColor[context]
                      font.pointSize: Common.fontSize
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
            clip: true
            color: Common.backgroundColor[context]
            border {
              color: Common.borderColor[context]
              width: Common.borderWidth -2
            }
            Text {
              id: objectInstructions
              anchors.fill: parent
              font.pointSize: Common.fontSize
              color: Common.textColor[context]
              horizontalAlignment: Text.AlignLeft
              Layout.minimumHeight: implicitHeight
              wrapMode: Text.WordWrap
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
          Text {
            id: objectCalories
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            color: Common.textColor[context]
            font.pointSize: Common.fontSize
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
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
          Text {
            id: objectJoules
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            font.pointSize: Common.fontSize
            color: Common.textColor[context]
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
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
          Text {
            id: objectFat
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            color: Common.textColor[context]
            font.pointSize: Common.fontSize
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
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
          Text {
            id: objectProtein
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            color: Common.textColor[context]
            font.pointSize: Common.fontSize
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
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
          Text {
            id: objectCarbs
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            color: Common.textColor[context]
            font.pointSize: Common.fontSize
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
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
          Text {
            id: objectSource
            Layout.fillWidth: true
            Layout.maximumHeight: Common.textHeight
            font.pointSize: Common.fontSize
            color: Common.textColor[context]
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
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
              Text {
                id: tagText
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: modelData
                color: Common.buttonTextColor[context]
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
              Text {
                id: eaterText
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: modelData
                color: Common.buttonTextColor[context]
              }
            }
          }
        }
      }
    }
  }

  onObjectChanged: {
    if (object !== undefined) {
      objectTitle.text = object.title

      objectType.text = object.meal_type
      objectServings.text = object.servings
      if (object.image_path !== undefined) {
        objectImage.pathName = object.image_path
        objectImage.source = object.image
      } else {
        objectImage.pathName = ""
        objectImage.source = ""
      }
      objectInstructions.text = object.instructions
      objectCalories.text = object.calories
      objectJoules.text = object.joules
      objectFat.text = object.fat
      objectProtein.text = object.protein
      objectCarbs.text = object.carbohydrates
      objectSource.text = object.source
      tagview.model = object.tags
      eaterview.model = object.eaters
      ingredientView.model = object.ingredient
    }
  }
  visible: object !== undefined
}
