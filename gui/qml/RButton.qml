import QtQuick 2.12
import QtQuick.Controls 2.5

import "common/"

Item {
  id: button
  signal clicked()
  property int context: 0
  property string text: "button"
  property bool enabled: true

  height: Common.textHeight
  implicitWidth: text.implicitWidth

  Rectangle {
    anchors.fill: parent
    color: button.enabled ? Common.buttonColor[context] : Common.disabledButtonColor[context]
    Text {
      id: text
      anchors.fill:parent
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      text: button.text
      color: Common.buttonTextColor[context]
      font.pointSize: Common.fontSize
    }
    MouseArea{
      anchors.fill: parent
      onClicked: if (button.enabled) button.clicked()
    }
  }
}
