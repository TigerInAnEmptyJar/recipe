import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import "common/"

Item {
  id: ingredientEditView
  property var object
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
        text: qsTr("Name")
        font.pointSize: Common.fontSize
        Layout.maximumHeight: Common.textHeight
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
      }
      TextInput {
        id: objectNameInput
        Layout.fillWidth: true
        Layout.maximumHeight: Common.textHeight
        height: Common.textHeight
        font.pointSize: Common.fontSize
        verticalAlignment: TextInput.AlignVCenter
        horizontalAlignment: TextInput.AlignLeft
      }
      Text {
        text: qsTr("Category")
        font.pointSize: Common.fontSize
        Layout.maximumHeight: Common.textHeight
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
      }
      ComboBox {
        id: objectCategoryInput
        Layout.maximumHeight: Common.textHeight
        Layout.fillWidth: true
        font.pointSize: Common.fontSize
        model: ingredients.categories()
      }
      Text {
        text: qsTr("Default amount type")
        font.pointSize: Common.fontSize
        Layout.maximumHeight: Common.textHeight
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
      }
      ComboBox {
        id: objectAmountInput
        Layout.maximumHeight: Common.textHeight
        Layout.fillWidth: true
        font.pointSize: Common.fontSize
        model: ingredients.amounts()
      }
      Text {
        text: qsTr("Image path")
        font.pointSize: Common.fontSize
        Layout.maximumHeight: Common.textHeight
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
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
          text: qsTr("Load Image")
          context: 0
          Layout.maximumHeight: Common.textHeight
          Layout.fillWidth: true
          FileDialog {
            id: loadDialog
            title: qsTr("Please choose an image file")
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
        id: sectionedLabel
        text: qsTr("Is sectioned")
        font.pointSize: Common.fontSize
        Layout.maximumHeight: Common.textHeight
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
      }
      CheckBox {
        id: objectSectioned
        text: ""
        font.pointSize: Common.fontSize
        enabled: true
        Layout.maximumHeight: Common.textHeight
      }
      Item{
        Layout.fillHeight: true
      }
      Item {
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      RButton {
        id: applyButton
        context: 0
        text: qsTr("Apply")
        Layout.preferredHeight: Common.textHeight
        Layout.preferredWidth: sectionedLabel.width
        onClicked: {
          object.image_path = objectImagePath.text
          object.default_amount = objectAmountInput.currentIndex
          object.isSectioned = objectSectioned.checked
          if (object.name !== objectNameInput.text || object.category !== objectCategoryInput.currentText){
            object.name = objectNameInput.text
            object.category = objectCategoryInput.currentIndex
            applyClicked()
          }
        }
      }
    }
  }
  onObjectChanged: {
    if (object !== undefined && object !== null) {
      if (object.image_path !== undefined) {
        objectImagePath.text = object.image_path
        objectImage.source = object.image
      } else {
        objectImagePath.text = ""
        objectImage.source = ""
      }
      objectSectioned.checked = object.isSectioned
      objectNameInput.text = object.name
      objectCategoryInput.currentIndex = objectCategoryInput.find(object.category)
      objectAmountInput.currentIndex = objectAmountInput.find(object.default_amount)
    }
  }
}
