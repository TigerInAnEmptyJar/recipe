import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import "common/"

Rectangle {
  id: page
  property int context: Common.context_shopping
  border{
    color: Common.borderColor[context]
    width: Common.borderWidth
  }

  Component {
    id: shoppingDayDelegate
    Rectangle {
      color: Common.textColor[context]
      ColumnLayout {
        anchors.fill: parent
        anchors.margins: Common.borderWidth
        Text {
          Layout.fillWidth: true
          height: Common.textHeight
          text: name
          color: Common.backgroundColor[context]
          MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: shoppingList.currentIndex = index
          }
        }
        ShoppingListView{
          Layout.fillWidth: true
          Layout.fillHeight: true
          model: day
          visible: shoppingList.currentIndex == index
        }
      }
      height: (shoppingList.currentIndex == index ? shoppingList.height : Common.textHeight + Common.borderWidth *2)
      width: shoppingList.width
    }
  }

  RowLayout {
    anchors.fill: parent
    anchors.margins: Common.margin
    ListView {
      id: shoppingList
      Layout.fillWidth: true
      Layout.fillHeight: true
      delegate: shoppingDayDelegate
      model: shopping
      clip: true
//      focus: true
    }
    ShoppingControls{
      Layout.fillHeight: true
      width: 100
      context: page.context
      model: shopping
      text: "shopping list"
    }
  }
}
