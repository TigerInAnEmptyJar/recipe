import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Item {
  id: labeledControl
  property alias label: labelText.text
  property int context: Common.context_ingredient
  default property alias _contentChildren: layout.data

  RowLayout {
    id: layout
    anchors.fill: parent
    clip: true
    Text {
      id: labelText
      Layout.fillHeight: true
      color: Common.textColor[context]
      font.pointSize: Common.fontSize
      font.bold: true
    }
  }
  Component.onCompleted: {
    for (var i = 0; i< layout.children.length; i++){
      layout.children[i].Layout.fillWidth = true
      layout.children[i].height = Common.textHeight
      layout.children[i].Layout.maximumHeight = Common.textHeight
    }
  }
  Layout.fillWidth: true
  Layout.minimumHeight: Common.textHeight
  Layout.preferredHeight: Common.controlHeight
}
