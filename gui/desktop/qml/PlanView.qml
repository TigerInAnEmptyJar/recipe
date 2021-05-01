import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQml.Models 2.3

import "common/"

Rectangle {
  id: planList
  property int context: Common.context_plan
  property int currentIndex : -1

  border{
    color: Common.borderColor[context]
    width: Common.borderWidth
  }
  color: Common.backgroundColor[context]

  ColumnLayout{
    anchors.fill: parent
    anchors.margins: Common.borderWidth
    TextInput {
      Layout.fillWidth: true
      Layout.maximumHeight: Common.textHeight
      Layout.minimumHeight: Common.textHeight
      text: plan.title
      onAccepted: plan.title = text
    }

    RStringListView {
      id: eatersView
      signal eatersChanged()
      context: planList.context
      model: plan.eaters()
      readonly: false
      title: qsTr("Eaters")
      onRemove: {
        plan.removeEater(element)
        eatersChanged()
      }
      onAdd: {
        plan.addEater(element)
        eatersChanged()
      }
      onEatersChanged: model = plan.eaters()
    }

    GridView {
      id: planView
      Layout.fillHeight: true
      Layout.fillWidth: true
      cellWidth: width / plan.meals
      cellHeight: cellWidth
      clip: true
      model: plan
      delegate: PlanItemView {
        id: viewItem
        object: model
        objectIndex: index
        context: planList.context
        width: planView.cellWidth
        height: planView.cellHeight
        Connections {
          target: eatersView
          function onEatersChanged() {viewItem.eatersChanged()}
        }
      }
    }
  }
}
