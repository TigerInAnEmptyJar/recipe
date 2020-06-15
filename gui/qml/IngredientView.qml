import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import "common/"

Item {
  id: ingredientView
  property var object
  property bool readonly: true
  property int context: 0
  signal applyClicked()

  Rectangle{
    anchors.fill: parent
    border{
      color: Common.borderColor[context]
      width: Common.borderWidth
    }
    color: Common.backgroundColor[context]
    GridLayout {
      anchors.fill: parent
      anchors.margins: Common.margin
      columns: 2
      Text {
        text: "Name"
        font.pointSize: Common.fontSize
        Layout.maximumHeight: Common.textHeight
        Layout.fillWidth: true
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
      }
      StackLayout {
        height: Common.textHeight
        Layout.fillWidth: true
        Layout.maximumHeight: Common.textHeight
        currentIndex: ingredientView.readonly ? 0 : 1
        Text {
          id: objectName
          font.pointSize: Common.fontSize
          verticalAlignment: Text.AlignVCenter
          horizontalAlignment: Text.AlignLeft
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight
        }
        TextInput {
          id: objectNameInput
          font.pointSize: Common.fontSize
          verticalAlignment: TextInput.AlignVCenter
          horizontalAlignment: TextInput.AlignLeft
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight
        }
      }
      Text {
        text: "Category"
        font.pointSize: Common.fontSize
        Layout.maximumHeight: Common.textHeight
        Layout.fillWidth: true
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
      }
      StackLayout {
        id: categoryFlip
        Layout.maximumHeight: Common.textHeight
        Layout.fillWidth: true
        currentIndex: ingredientView.readonly ? 0 : 1
        Text {
          id: objectCategory
          font.pointSize: Common.fontSize
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight
          horizontalAlignment: Text.AlignLeft
          verticalAlignment: Text.AlignVCenter
        }
        ComboBox {
          id: objectCategoryInput
          font.pointSize: Common.fontSize
          Layout.maximumHeight: Common.textHeight
          Layout.fillWidth: true
          model: ingredients.categories()
        }
      }
      Text {
        text: "Default amount type"
        font.pointSize: Common.fontSize
        Layout.maximumHeight: Common.textHeight
        Layout.fillWidth: true
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
      }
      StackLayout {
        id: amountFlip
        Layout.maximumHeight: Common.textHeight
        Layout.fillWidth: true
        currentIndex: ingredientView.readonly ? 0 : 1
        Text {
          id: objectAmount
          font.pointSize: Common.fontSize
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight
          horizontalAlignment: Text.AlignLeft
          verticalAlignment: Text.AlignVCenter
        }
        ComboBox {
          id: objectAmountInput
          font.pointSize: Common.fontSize
          Layout.fillWidth: true
          Layout.maximumHeight: Common.textHeight
          model: ingredients.amounts()
        }
      }
      Text {
        text: "Image path"
        font.pointSize: Common.fontSize
        Layout.maximumHeight: Common.textHeight
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        Layout.fillWidth: true
      }
      RowLayout{
        Layout.fillWidth: true
        Layout.maximumHeight: Common.textHeight
        Image {
          id: objectImage
          Layout.maximumHeight: Common.textHeight
          Layout.maximumWidth: Common.textHeight
          fillMode: Image.PreserveAspectFit
          width: height
        }
        Text {
          id: objectImagePath
          font.pointSize: Common.fontSize
          Layout.maximumHeight: Common.textHeight
          Layout.fillWidth: true
          horizontalAlignment: Text.AlignLeft
          verticalAlignment: Text.AlignVCenter
        }
        RButton{
          id: objectImagePathButton
          text: "Load Image"
          context: 0
          visible: !ingredientView.readonly
          Layout.maximumHeight: Common.textHeight
          Layout.fillWidth: true
          FileDialog {
            id: loadDialog
            title: "Please choose an image file"
            nameFilters: [ "Image files (*.jpeg, *.jpg)" ]
            folder: "file://" + ingredients.databasePath()+"/"
            selectExisting: true
            selectMultiple: false
            onAccepted: {
              objectImagePath.text = loadDialog.fileUrl
              close()
            }
            onRejected: {
              close()
            }
          }
          onClicked: {
            loadDialog.folder = "file://" + ingredients.databasePath() + "/"
            loadDialog.open()
          }
        }
      }
      Text {
        text: "Is sectioned"
        font.pointSize: Common.fontSize
        Layout.maximumHeight: Common.textHeight
        Layout.fillWidth: true
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
      }
      CheckBox {
        id: objectSectioned
        text: "Sectioned"
        font.pointSize: Common.fontSize
        enabled: !ingredientView.readonly
        Layout.maximumHeight: Common.textHeight
        Layout.fillWidth: true
      }
      Item{
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      Item {
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      RButton {
        id: applyButton
        visible: !ingredientView.readonly
        context: 0
        text: "Apply"
        Layout.fillWidth: true
        Layout.preferredHeight: Common.textHeight
        onClicked: {
          object.name = objectNameInput.text
          object.image_path = objectImagePath.text
          object.category = objectCategoryInput.currentIndex
          object.default_amount = objectAmountInput.currentindex
          object.isSectioned = objectSectioned.checked
          applyClicked()
        }
      }
    }
  }
  onObjectChanged: {
    if (object !== undefined) {
      objectName.text = object.name
      objectCategory.text = object.category
      objectImagePath.text = object.image_path
      objectImage.source = object.image
      objectAmount.text = object.default_amount
      objectSectioned.checked = object.isSectioned
      objectNameInput.text = object.name
      objectCategoryInput.currentIndex = objectCategoryInput.find(object.category)
      objectAmountInput.currentIndex = objectAmountInput.find(object.default_amount)
      ingredientView.readonly = true
    }
  }
  visible: object !== undefined
}
