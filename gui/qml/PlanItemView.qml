import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQml.Models 2.3

import "common/"

Rectangle {
  id: planItemList
  property var object
  property int context: 0
  color: Common.backgroundColor[context]

  Component {
    id: planItemDelegate
    Rectangle {
      height: Common.textHeight
      width: iView.width
      color: Common.backgroundColor[context]
      RowLayout {
        anchors.fill: parent
        Image {
          source: image
          Layout.maximumHeight: Common.textHeight
          Layout.maximumWidth: Common.textHeight
          fillMode: Image.PreserveAspectFit
        }
        Text {
          text: model.title
          color: Common.textColor[context]
          font.pointSize: Common.fontSize
          Layout.fillHeight: true
          Layout.fillWidth: true
        }
      }
    }
  }

  ColumnLayout {
    anchors.fill: parent
    CheckBox{
      text: qsTr("shopping")
      checked: object.shoppingBefore
      onClicked: object.shoppingBefore = checked
    }
    Rectangle {
      Layout.fillHeight: true
      Layout.fillWidth: true
      border{
        color: Common.borderColor[context]
        width: Common.borderWidth
      }
      color: Common.backgroundColor[context]
      ColumnLayout {
        anchors.fill: parent
        TextInput {
          text: object.name
          onAccepted: object.name = text
          Layout.fillWidth: true
        }
        ListView {
          id: planView
          Layout.fillHeight: true
          Layout.fillWidth: true
          model: object.item
          focus: true
          delegate: planItemDelegate
        }
      }
    }
  }
}
