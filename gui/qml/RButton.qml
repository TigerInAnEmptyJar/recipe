import QtQuick 2.12
import QtQuick.Controls 2.5

Item {
  id: button
  signal clicked()
  property int context: 0
  property string text: "button"
  property bool enabled: true

  Rectangle {
    property real value: button.enabled ? 0.5 : 0.1
    property real saturation: button.enabled ? 1 : 0.5
    property var contextColor: [Qt.hsva(0.17, value, saturation),
      Qt.hsva(0, value, saturation),
      Qt.hsva(0.5, value, saturation)]
    anchors.fill: parent
    color: contextColor[context]
    Text {
      anchors.fill:parent
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      text: button.text
    }
    MouseArea{
      anchors.fill: parent
      onClicked: if (button.enabled) button.clicked()
    }
  }
}
