import QtQuick 2.12
import QtQuick.Controls 2.5

Item {
  id: button
  signal clicked()
  property int context: 0
  property string text: "button"
  property bool enabled: true

  height: Common.textHeight
  implicitWidth: text.implicitWidth

  Rectangle {
    id: buttonRectangle
    anchors.fill: parent
    color: button.enabled ? Common.buttonColor[context] : Common.disabledButtonColor[context]
    Text {
      id: text
      anchors.fill: parent
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      text: button.text
      color: Common.buttonTextColor[context]
      font.pointSize: Common.fontSize
    }
    MouseArea {
      anchors.fill: parent
      onClicked: {
        if (button.enabled) {
          buttonAnimation.start()
          button.clicked()
        }
      }
    }
    ColorAnimation on color {
      id: buttonAnimation
      from: Common.buttonColor[context]
      to: Common.disabledButtonColor[context]
      duration: 200
      onFinished: buttonBackAnimation.start()
    }
    ColorAnimation on color {
      id: buttonBackAnimation
      from: Common.disabledButtonColor[context]
      to: Common.buttonColor[context]
      duration: 200
    }
  }
}
