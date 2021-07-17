import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Item {
  id: planListWidget
  property int context: 2

  signal back()

  function requestData() {
    var xmlhttp = new XMLHttpRequest();
    var url = "http://localhost:8080/v1/plan";
    xmlhttp.onreadystatechange = function() {
      if (xmlhttp.readyState === XMLHttpRequest.DONE && xmlhttp.status == 200) {
          myFunction(xmlhttp.responseText);
      }
    }
    xmlhttp.open("GET", url, true);
    xmlhttp.send();
  }

  function myFunction(response) {
    var arr = JSON.parse(response);
    for(var i = 0; i < arr.length; i++) {
        planList.model.append( {planTitle: arr[i].title, planId: arr[i].id })
    }
  }

  signal planSelected(string planId)

  ListModel {
    id: model
  }

  Component {
    id: recipeDelegate
    RBorderedText {
      context: planListWidget.context
      text: planTitle
      width: planList.width
      height: 30
      MouseArea {
        anchors.fill: parent
        onClicked: planListWidget.planSelected(planId)
      }
    }
  }

  ListView {
    id: planList
    delegate: recipeDelegate
    anchors.fill: parent
    orientation: ListView.Vertical
    clip: true
    model: model
  }
}
