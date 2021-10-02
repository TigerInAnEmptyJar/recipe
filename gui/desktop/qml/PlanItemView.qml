import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQml.Models 2.3

import "common/"

Rectangle {
  id: planItemList
  property var object
  property int objectIndex: -1
  property int context: 0
  color: Common.backgroundColor[context]
  border {
    color: Common.borderColor[context]
    width: Common.borderWidth
  }

  function eatersChanged() {
    subscriberView.model = plan.eaters()
  }

  ColumnLayout {
    anchors.fill: parent
    anchors.margins: Common.borderWidth
    clip: true
    CheckBox{
      Layout.fillWidth: true
      Layout.maximumHeight: Common.controlHeight
      icon.color: Common.buttonColor[context]
      text: qsTr("shopping")
      checked: object.shoppingBefore
      onClicked: object.shoppingBefore = checked
    }
    Rectangle {
      Layout.fillHeight: true
      Layout.fillWidth: true
      color: Common.backgroundColor[context]
      DropArea {
        anchors.fill: parent
        onDropped: plan.addRecipe(index, drag.source.id)
      }
      ColumnLayout {
        anchors.fill: parent
        TextInput {
          text: object.name
          color: Common.textColor[context]
          onAccepted: object.name = text
          Layout.fillWidth: true
        }
        ListView {
          id: planView
          Layout.fillHeight: true
          Layout.fillWidth: true
          clip: true
          model: object.item
          focus: true
          delegate: PlanMealView {
            dayIndex: objectIndex
            mealIndex: model.index
            context: planItemList.context
          }
        }
      }
    }
  }
}
