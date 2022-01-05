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
        onDropped: {
          if (drop.proposedAction == Qt.MoveAction) {
            planView.model.addRecipe(drag.source.id)
            drop.acceptProposedAction()
          }
          if (drop.proposedAction == Qt.CopyAction){
            planFullView.model.addRecipe(drag.source.id)
            drop.acceptProposedAction()
          }
        }
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
          visible: count > 0
          delegate: PlanMealView {
            dayIndex: objectIndex
            mealIndex: model.index
            context: planItemList.context
          }
        }
        ListView {
          id: planFullView
          Layout.fillHeight: true
          Layout.fillWidth: true
          clip: true
          model: object.fullRecipes
          visible: count > 0
          focus: true
          delegate:   Rectangle {
            width: planFullView.width
            height: Math.min(planFullView.width /2, planFullView.height)
            color: Common.backgroundColor[context]
            clip: true
            MouseArea {
              id: mouseArea
              anchors.fill: parent
              hoverEnabled: true
              onDoubleClicked: model.deleteRecipe(model.index)
            }
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
        }
      }
    }
  }
}
