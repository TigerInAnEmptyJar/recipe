import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import "common/"

Rectangle {
  id: recipeview
  property bool portrait: false
  property var object:{"title": "", "ingredients": [], "instructions": ""}
  property real relativeSize: 1
  property int context: 1

  signal back();

  function requestRecipe(recipeId) {
      var xmlhttp = new XMLHttpRequest();
      var url = "http://localhost:8080/v1/recipe/" + recipeId;

      xmlhttp.onreadystatechange=function() {
        if (xmlhttp.readyState === XMLHttpRequest.DONE && xmlhttp.status == 200) {
            myFunction(xmlhttp.responseText);
        }
      }
      xmlhttp.open("GET", url, true);
      xmlhttp.send();
  }

  function myFunction(response) {
    var obj = JSON.parse(response);
    object = obj
    for(var i = 0; i < obj.ingredients.length; i++) {
        recipemodel.append( {name: obj.ingredients[i].name,
                                  unit: obj.ingredients[i].unit,
                                  value: obj.ingredients[i].value })
    }
  }


  ListModel {
    id: recipemodel
  }

  Component{
    id: ingredientDelegate
    Rectangle {
      height: Common.controlHeight * relativeSize
      width: ingredientView.width
      color: Common.backgroundColor[recipeview.context]
      border{
        color: Common.borderColor[recipeview.context]
        width: Common.borderWidth
      }

      RowLayout {
        anchors.fill: parent
        anchors.margins: Common.borderWidth
        RText {
          text: name
          Layout.fillHeight: true
          Layout.fillWidth: true
          relativeSize: recipeview.relativeSize
          context: recipeview.context
        }
        Item{Layout.fillWidth: true}
        RText {
          text: value
          relativeSize: recipeview.relativeSize
          context: recipeview.context
          Layout.fillHeight: true
          Layout.preferredWidth: implicitWidth
        }
        ComboBox {
          currentIndex: unit
//          color: Common.textColor[context]
          Layout.fillHeight: true
          font.pointSize: relativeSize * 10
          enabled: false
          model: [qsTr("Milliliter"),
            qsTr("Cups"),
            qsTr("Grams"),
            qsTr("Kilograms"),
            qsTr("Ounces"),
            qsTr("Pounds"),
            qsTr("Pieces"),
            qsTr("Bundles"),
            qsTr("Cans"),
            qsTr("Table spoons"),
            qsTr("Tea spoons"),
            qsTr("Pinces")]
        }
      }
    }
  }

  ColumnLayout{
    anchors.fill: parent
    RText {
      id: objectTitle
      Layout.fillWidth: true
      Layout.maximumHeight: recipeview.height * 0.1
      context: recipeview.context
      relativeSize: recipeview.relativeSize * 2
      text: object.title
      MouseArea {
        anchors.fill: parent
        scrollGestureEnabled: false
        onWheel: {
          relativeSize = relativeSize + wheel.angleDelta.y/1200
        }
        onClicked: recipeview.back()
      }
    }

    SwipeView {
      id: view

      currentIndex: 0
      Layout.fillWidth: true
      Layout.fillHeight: true

      ListView {
        id: ingredientView
        orientation: ListView.Vertical
        clip: true
        delegate: ingredientDelegate
        model: recipemodel
      }

      RBorderedText {
        id: objectInstructions
        relativeSize: recipeview.relativeSize
        context: recipeview.context
        wrapMode: Text.WordWrap
        text: object.instructions.split(/[\r\n]/g).join("\n\n");
      }
    }
    PageIndicator {
      id: indicator

      count: view.count
      currentIndex: view.currentIndex
      interactive: true

      Layout.fillWidth: true
//      anchors.bottom: view.bottom
//      anchors.horizontalCenter: parent.horizontalCenter
    }
  }

  PinchArea {
    anchors.fill: parent
    onPinchUpdated: relativeSize = relativeSize * (1 + pinch.scale - pinch.previousScale)
  }
}
