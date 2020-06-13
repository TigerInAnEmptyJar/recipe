import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3


Rectangle {
  property var currentObject
  border{
    color: Qt.hsva(0.17, 1, 1)
    width: 3
  }
  color: Qt.hsva(0.17, 0.1, 1)

  Component {
    id: ingredientDelegate
    Rectangle {
      height: 20
      width: iView.width
      color: Qt.hsva(0.17, 0.1, 1)
      RowLayout {
        width: parent.width
        Image {source: image_path}
        Text { text: name
          color: Qt.hsva(0.17, 1, 0.5)
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
    anchors.margins: 5
    model: ingredients
    delegate: ingredientDelegate
    focus: true
  }
}
