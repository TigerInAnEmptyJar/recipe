import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import "common/"

Rectangle {
  id: page
  property int context: 2
  border{
    color: Common.borderColor[context]
    width: Common.borderWidth
  }

  RowLayout {
    anchors.fill: parent
    PlanView{
      id: planList
      context: page.context
      Layout.fillWidth: true
      Layout.fillHeight: true
    }
    PlanControls{
      Layout.fillHeight: true
      width: 100
      context: page.context
      model: plan
      currentObject: planList.currentObject
      text: "recipe"
      onEditRequested: planView.readonly = false
      onNewRequested: plan.newPlan()
    }
  }
}
