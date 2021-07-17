import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "common/"

Rectangle {
  id: rtext
  property int context: context
  property alias text: rtextText.text
  property real relativeSize: 1
  property alias wrapMode: rtextText.wrapMode

  color: Common.backgroundColor[context]
  border {
    color: Common.borderColor[context]
    width: Common.borderWidth
  }

  Flickable {
    anchors.fill: parent
    anchors.margins: Common.borderWidth
    flickableDirection: Flickable.VerticalFlick
    contentHeight: rtextText.height
    contentWidth: rtextText.width
    clip: true
    Text {
      id: rtextText
      font.pointSize: Common.fontSize * relativeSize
      color: Common.textColor[context]
      width: rtext.width
      height: lineCount * font.pointSize * 2
    }
  }
}
