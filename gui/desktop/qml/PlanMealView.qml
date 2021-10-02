import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQml.Models 2.3

import "common/"

Rectangle {
  id: planMealView
  property int dayIndex: -1
  property int mealIndex: -1
  property int context: 0
  color: Common.backgroundColor[context]
  width: planView.width
  height: Math.min(planView.width /2, planView.height)

  Component {
    id: subscriberDelegate
    CheckBox {
      height: Common.controlHeight
      width: planView.width
      clip: true
      text: modelData
      checked: plan.subscribed(dayIndex, mealIndex, text)
      onCheckStateChanged: {
        if (checked)
          plan.addSubscriber(dayIndex, mealIndex, text)
        else
          plan.removeSubscriber(dayIndex, mealIndex, text)
      }
    }
  }
  Rectangle {
    width: planView.width
    height: Math.min(planView.width /2, planView.height)
    color: Common.backgroundColor[context]
    clip: true
    MouseArea {
      id: mouseArea
      anchors.fill: parent
      hoverEnabled: true
      onDoubleClicked: planView.model.deleteRecipe(model.index)
    }
    RowLayout{
      anchors.fill: parent
      Rectangle{
        Layout.fillHeight: true
        Layout.fillWidth: true
        Image {
          anchors.fill: parent
          source: image
          fillMode: Image.PreserveAspectFit
        }
        Rectangle {
          id: description
          anchors.top: parent.top
          anchors.left: parent.left
          anchors.right: parent.right
          height: Common.textHeight
          visible: mouseArea.containsMouse
          color: Common.backgroundColor[context]
          Text {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            text: model.title
            color: Common.textColor[context]
            font.pointSize: Common.fontSize
          }
        }
      }
      ListView {
        Layout.fillHeight: true
        Layout.fillWidth: true
        model: plan.eaters()
        delegate: subscriberDelegate
      }
    }
  }
}
