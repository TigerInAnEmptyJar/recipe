import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import "common/"

Item {
  id: ingredientView
  property var object
  property int context: 0

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
      Text {
        id: objectName
        height: Common.textHeight
        Layout.fillWidth: true
        Layout.maximumHeight: Common.textHeight
        font.pointSize: Common.fontSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
      }
      Text {
        text: qsTr("Category")
        font.pointSize: Common.fontSize
        Layout.maximumHeight: Common.textHeight
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
      }
      Text {
        id: objectCategory
        Layout.maximumHeight: Common.textHeight
        Layout.fillWidth: true
        font.pointSize: Common.fontSize
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
      }
      Text {
        text: qsTr("Default amount type")
        font.pointSize: Common.fontSize
        Layout.maximumHeight: Common.textHeight
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
      }
      Text {
        id: objectAmount
        Layout.maximumHeight: Common.textHeight
        Layout.fillWidth: true
        font.pointSize: Common.fontSize
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
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
      }
      Text {
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
        enabled: false
        Layout.maximumHeight: Common.textHeight
      }
      Item{
        Layout.fillHeight: true
      }
      Item {
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
    }
  }
  onObjectChanged: {
    if (object !== undefined) {
      objectName.text = object.name
      objectCategory.text = object.category
      objectAmount.text = object.default_amount
      objectSectioned.checked = object.isSectioned
      if (object.image_path !== undefined) {
        objectImagePath.text = object.image_path
        objectImage.source = object.image
      } else {
        objectImagePath.text = ""
        objectImage.source = ""
      }
    }
  }
}
