import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import "common/"

Rectangle {
  id: page
  property int context: Common.context_plan
  border{
    color: Common.borderColor[context]
    width: Common.borderWidth
  }

  RowLayout {
    anchors.fill: parent
    PlanView{
      id: planList
      context: page.context
      clip: true
      Layout.fillWidth: true
      Layout.fillHeight: true
    }
    RecipeList {
      width: 200
      Layout.fillHeight: true
      context: Common.context_recipe
    }
    PlanControls{
      Layout.fillHeight: true
      width: 100
      context: page.context
      model: plan
      currentObject: planList.currentObject
      text: "recipe"
      onNewRequested: {
        console.log("new plan requested")
        plan.newPlan(7, 2)
      }
    }
  }
}
