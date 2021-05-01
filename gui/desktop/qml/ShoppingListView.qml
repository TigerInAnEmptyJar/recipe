import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQml.Models 2.3

import "common/"

Rectangle {
  id: shoppingListView
  property var model
  property int context: Common.context_shopping
  border {
    color: Common.borderColor[Common.context_shopping]
    width: Common.borderWidth
  }
  color: Common.backgroundColor[context]

  Component {
    id: shoppingListDelegate
    Item {
      id: delegateItem
      Row {
        anchors.fill: parent
        Text {
          text: name
          color: Common.textColor[context]
          font.pointSize: Common.fontSize
          Layout.fillHeight: true
          width: delegateItem.width / 5
        }
        TextInput {
          text: needed
          color: Common.textColor[context]
          font.pointSize: Common.fontSize
          Layout.fillHeight: true
          width: delegateItem.width / 5
        }
        CheckBox {
          id: checker
          checked: check_all
          Layout.fillHeight: true
          onCheckedChanged: check_all = checked
        }
        TextInput {
          text: available_value
          validator: DoubleValidator {bottom: 0; top: 1000; decimals: 3}
          font.pointSize: Common.fontSize
          onAccepted: available_value = text
          color: Common.textColor[context]
          Layout.fillHeight: true
          width: delegateItem.width / 5
          enabled: !checker.checked
        }
        ComboBox {
          model: recipes.amount_types()
          currentIndex: available_unit
          onActivated: available_unit = currentIndex
          font.pointSize: Common.fontSize
          Layout.maximumHeight: Common.textHeight
          width: delegateItem.width / 5
          enabled: !checker.checked
        }
      }
      height: Common.controlHeight
      width: ingredientList.width
    }
  }

  Component {
    id: sectionDelegate
    Rectangle {
      height: Common.textHeight
      width: ingredientList.width
      anchors.margins: Common.margin
      color: Common.textColor[context]
      Text {
        anchors.fill: parent
        text: section
        color: Common.backgroundColor[context]
        font.pointSize: Common.fontSize
        horizontalAlignment: Text.AlignHCenter
      }
    }
  }

  ListView {
    id: ingredientList
    anchors.fill: parent
    anchors.margins: Common.margin
    model: shoppingListView.model
    delegate: shoppingListDelegate
    section {
      property: "category"
      delegate: sectionDelegate
    }
    boundsBehavior: Flickable.StopAtBounds
    clip: true
    focus: true
  }
}
