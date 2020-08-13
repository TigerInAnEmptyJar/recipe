import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQml.Models 2.3

import "common/"

Rectangle {
  id: planList
  property int context: 0
  property int currentIndex : -1

  border{
    color: Common.borderColor[context]
    width: Common.borderWidth
  }
  color: Common.backgroundColor[context]

  ColumnLayout{
    anchors.fill: parent
    anchors.margins: Common.margin
    TextInput {
      text: plan.title
      onAccepted: plan.title = text
    }

    GridView {
      id: planView
      Layout.fillHeight: true
      Layout.fillWidth: true
      cellWidth: width / plan.meals
      cellHeight: cellWidth

      model: plan
      delegate: PlanItemView {
        object: model
        context: planList.context
      }
    }
  }
}
