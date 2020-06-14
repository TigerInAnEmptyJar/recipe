import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import "common/"

Rectangle {
  property var currentObject
  property int context: 0
  border{
    color: Common.borderColor[context]
    width: Common.borderWidth
  }
  color: Common.backgroundColor[context]

  Component {
    id: ingredientDelegate
    Rectangle {
      height: Common.textHeight
      width: iView.width
      color: Common.backgroundColor[context]
      RowLayout {
        width: parent.width
        Image {source: image_path}
        Text {
          text: name
          color: Common.textColor[context]
          font.pointSize: Common.fontSize
          height: Common.textHeight
        }
      }
      MouseArea{
        anchors.fill: parent
        onClicked: currentObject = model
      }
    }
  }

  ListView {
    id: iView
    anchors.fill: parent
    anchors.margins: Common.margin
    model: ingredients
    delegate: ingredientDelegate
    focus: true
  }
}
