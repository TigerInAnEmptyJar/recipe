import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

ApplicationWindow {
  visible: true
  width: 400
  height: 600
//  width: Screen.orientation === Screen.Portrait ? Screen.width : Screen.height
//  height: Screen.orientation === Screen.Portrait ? Screen.height : Screen.width
  title: qsTr("Recipe")

  StackView {
      id: stack
      initialItem: mainView
      anchors.fill: parent
  }

  Component {
      id: mainView

      Row {
          spacing: 10

          Button {
              text: "Recipes"
              onClicked: stack.push(recipeList)
          }
          Button {
              text: "Plans"
              onClicked: stack.push(planList)
          }
      }
  }

  property string currentRecipeId
  property string currentPlanId

  Component {
    id: recipeList
    RecipeList {
      Component.onCompleted: requestData()
      onRecipeSelected: {
        currentRecipeId = recipeId
        stack.push(recipeView)
      }
      onBack: stack.pop()
    }
  }

  Component {
    id: recipeView

    RecipeView {
      id: recipeInnerView
      Component.onCompleted: requestRecipe(currentRecipeId)
      onBack: stack.pop()
    }
  }

  Component {
    id: planList
    PlanList {
      Component.onCompleted: requestData()
      onPlanSelected: {
        currentPlanId = planId
        stack.push(planView)
      }
      onBack: stack.pop()
    }
  }

  Component {
    id: planView

    PlanView {
      id: planInnerView
      Component.onCompleted: requestData(currentPlanId)
      onBack: stack.pop()
      onRecipeSelected: {
        currentRecipeId = recipeObject
        stack.push(recipeView)
      }
    }
  }
}
