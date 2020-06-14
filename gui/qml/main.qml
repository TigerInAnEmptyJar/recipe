import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import "common/"

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
        ListElement {name: "info"; value: 3}
        ListElement {name: "Ingredients"; value: 0}
        ListElement {name: "Recipes"; value: 1}
        ListElement {name: "Week-plan"; value: 2}
        ListElement {name: "Shopping list"; value: 3}
      }
      delegate: Rectangle {
        height: Common.controlHeight
        width: control.width
        color: Common.backgroundColor[value]
        border.color: Common.borderColor[value]
        border.width: Common.borderWidth
        Text {
          anchors.fill: parent
          anchors.margins: Common.margin
          text: model.name
          color: Common.textColor[value]
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
      Text {
        text: qsTr("Program: Recipe")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
      }
      IngredientPage{
      }
      RecipePage {
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
