import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

ApplicationWindow {
  visible: true
  width: 640
  height: 480
  title: qsTr("Scroll")

  RowLayout {
    anchors.fill: parent

    ListView {
      id: control
      Layout.fillHeight: true
      width: 100
      model: ListModel {
        ListElement {name: "Ingredients"; hue: 0.17}
        ListElement {name: "Recipes"; hue: 0}
        ListElement {name: "Week-plan"; hue: 0.33}
        ListElement {name: "Shopping list"; hue: 0.66}
      }
      delegate: Rectangle {
        height: 30
        width: control.width
        color: Qt.hsva(hue, 0.1, 1)
        border.color: Qt.hsva(hue, 0.9, 1)
        border.width: 2
        Text {
          anchors.fill: parent
          anchors.margins: 3
          text: model.name
          color: Qt.hsva(hue, 1, 0.8)
          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter
          MouseArea{
            anchors.fill: parent
            onClicked: view.currentIndex = index
          }
        }
      }
    }

    StackLayout {
      id: view
      Layout.fillHeight: true
      Layout.fillWidth: true
      IngredientPage{
      }
      Text {
        text: qsTr("Recipes")
      }
      Text {
        text: qsTr("Week-plan")
      }
      Text {
        text: qsTr("Shopping list")
      }
    }
  }
}
