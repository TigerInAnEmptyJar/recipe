import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQml.Models 2.15

import "common/"

Item {
  id: planViewWidget
  property int context: 2
  property var object:{"name": "", "items": [], "eaters": []}
  property real relativeSize: 1

  signal back()

  function requestData(planId) {
    var xmlhttp = new XMLHttpRequest();
    var url = "http://localhost:8080/v1/plan/" + planId;
    xmlhttp.onreadystatechange = function() {
      if (xmlhttp.readyState === XMLHttpRequest.DONE && xmlhttp.status == 200) {
          myFunction(xmlhttp.responseText);
      }
    }
    xmlhttp.open("GET", url, true);
    xmlhttp.send();
  }

  function myFunction(response) {
    var current = view.currentIndex
    dayModel.clear()
    var obj = JSON.parse(response);
    object = obj
    eatersBox.model = obj.eaters
    for(var i = 0; i < obj.items.length; i++) {
      var rec = new Array(0)
      for (var j = 0; j < obj.items[i].recipes.length; j++) {
        var s = []
        for (var k = 0; k < obj.items[i].recipes[j].subscribers.length; k++)
          s.push({"name":obj.items[i].recipes[j].subscribers[k]})

        rec.push({"title": obj.items[i].recipes[j].name, "recipeId": obj.items[i].recipes[j].id, "subscribers": s})
      }
      dayModel.append( {title: obj.items[i].name,
                     shopping: obj.items[i].shoppingBefore,
                     recipes: rec
                   });
    }
    view.setCurrentIndex(current)
  }

  function requestSubscribe(meal, recipe, subscriber) {
    var xmlhttp = new XMLHttpRequest();
    var url = "http://localhost:8080/v1/plan/subscribe/" + object.id;
    let json = JSON.stringify({
      day: meal,
      recipe: recipe,
      subscriber: subscriber
    });
    xmlhttp.onreadystatechange = function() {
      if (xmlhttp.readyState === XMLHttpRequest.DONE && xmlhttp.status == 200) {
          myFunction(xmlhttp.responseText);
      }
    }
    xmlhttp.open("PUT", url, true);
    xmlhttp.setRequestHeader("Content-type", "application/json")
    xmlhttp.setRequestHeader("Accept", "application/problem+json")
    xmlhttp.send(json);
  }

  function requestUnsubscribe(meal, recipe, subscriber) {
    var xmlhttp = new XMLHttpRequest();
    var url = "http://localhost:8080/v1/plan/unsubscribe/" + object.id;
    let json = JSON.stringify({
      day: meal,
      recipe: recipe,
      subscriber: subscriber
    });
    xmlhttp.onreadystatechange = function() {
      if (xmlhttp.readyState === XMLHttpRequest.DONE && xmlhttp.status == 200) {
          myFunction(xmlhttp.responseText);
      }
    }
    xmlhttp.open("PUT", url, true);
    xmlhttp.setRequestHeader("Content-type", "application/json")
    xmlhttp.setRequestHeader("Accept", "application/problem+json")
    xmlhttp.send(json);
  }

  function containsSubscriber(list, item) {
    for (var i = 0; i < list.count; i++) {
      if (list.get(i).name === item) {
        return true
      }
    }
    return false
  }

  signal daySelected(var dayObject)
  signal recipeSelected(var recipeObject)

  ListModel {
    id: dayModel
  }

  Component {
    id: recipeDelegate
    Rectangle {
      width: parent.width
      height: Common.controlHeight * planViewWidget.relativeSize * 1.5
      border {
        width: Common.borderWidth
        color: Common.borderColor[planViewWidget.context]
      }
      color: Common.backgroundColor[planViewWidget.context]
      RowLayout {
        anchors.fill: parent
        CheckBox {
          id: subscribedBox
          Layout.fillHeight: true
          onClicked: {
            if (checked) {
              requestSubscribe(view.currentIndex, index, eatersBox.currentText)
            } else {
              requestUnsubscribe(view.currentIndex, index, eatersBox.currentText)
            }
          }
        }
        Text {
          Layout.fillHeight: true
          Layout.fillWidth: true
          color: Common.textColor[planViewWidget.context]
          text: title
          verticalAlignment: Text.AlignVCenter
          MouseArea {
            anchors.fill: parent
            onClicked: recipeSelected(model.recipeId)
          }
        }
      }
      Component.onCompleted: subscribedBox.checked = containsSubscriber(model.subscribers, eatersBox.currentText)
      Connections {
        target: eatersBox
        function onCurrentTextChanged(text) {
          subscribedBox.checked = containsSubscriber(model.subscribers, eatersBox.currentText)
        }
      }
    }
  }

  ColumnLayout{
    anchors.fill: parent
    RText {
      id: objectTitle
      Layout.fillWidth: true
      Layout.maximumHeight: planViewWidget.height * 0.1
      context: planViewWidget.context
      relativeSize: planViewWidget.relativeSize * 2
      text: object.name
      MouseArea {
        anchors.fill: parent
        scrollGestureEnabled: false
        onWheel: {
          relativeSize = relativeSize + wheel.angleDelta.y/1200
        }
        onClicked: planViewWidget.back()
      }
    }
    Rectangle {
      Layout.fillWidth: true
      color: Common.backgroundColor[planViewWidget.context]
      implicitHeight: eatersBox.implicitHeight
      RowLayout {
        anchors.fill: parent
        RText {
          text: qsTr("I am:")
          Layout.fillHeight: true
          context: planViewWidget.context
          relativeSize: planViewWidget.relativeSize
        }
        ComboBox {
          id: eatersBox
          Layout.fillHeight: true
          Layout.fillWidth: true
        }
      }
    }
    SwipeView {
      id: view
      Repeater {
        model: dayModel
        Loader {
          active: SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem
          sourceComponent:     Rectangle {
            width: planViewWidget.width
            height: planViewWidget.height
            border{
              color: Common.borderColor[context]
              width: Common.borderWidth
            }

            ColumnLayout {
              anchors.fill: parent
              anchors.margins: Common.borderWidth
              RBorderedText {
                id: d_title
                context: planViewWidget.context
                text: title
                Layout.fillWidth: true
                Layout.preferredHeight: Common.textHeight
              }
              CheckBox {
                id: d_shopping
                text: qsTr("go shopping")
                checked: shopping
                enabled: false
                Layout.fillWidth: true
              }
              ListView {
                id: d_recipes
                model: recipes
                delegate: recipeDelegate
                Layout.fillHeight: true
                Layout.fillWidth: true
              }
            }
          }
        }
      }
      currentIndex: 0
      Layout.fillWidth: true
      Layout.fillHeight: true
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
