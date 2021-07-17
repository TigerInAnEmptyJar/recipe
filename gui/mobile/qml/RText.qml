import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "common/"

Rectangle {
  id: rtext
  property int context: context
  property real relativeSize: 1
  property alias text: rtextText.text
  property alias wrapMode: rtextText.wrapMode

  color: Common.backgroundColor[context]
  implicitHeight: rtextText.implicitHeight + Common.borderWidth * 2
  implicitWidth: rtextText.implicitWidth + Common.borderWidth * 2

  clip: true

  Text {
    id: rtextText
    anchors.verticalCenter: parent.verticalCenter
    anchors.left: parent.left
    anchors.leftMargin: Common.borderWidth
    anchors.right: parent.right
    anchors.rightMargin: Common.borderWidth
    font.pointSize: Common.fontSize * relativeSize
    color: Common.textColor[context]
  }
}
