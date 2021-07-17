import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Item {
  id: recipeListWidget
  property int context: 1

  signal back()

  function requestData() {
    var xmlhttp = new XMLHttpRequest();
    var url = "http://localhost:8080/v1/recipe";
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
        recipeList.model.append( {recipeTitle: arr[i].title, recipeId: arr[i].id })
    }
  }

  signal recipeSelected(string recipeId)

  ListModel {
    id: model
  }

  Component {
    id: recipeDelegate
    RBorderedText {
      context: recipeListWidget.context
      text: recipeTitle
      width: recipeList.width
      height: 30
      MouseArea {
        anchors.fill: parent
        onClicked: recipeListWidget.recipeSelected(recipeId)
      }
    }
  }

  ListView {
    id: recipeList
    delegate: recipeDelegate
    anchors.fill: parent
    orientation: ListView.Vertical
    clip: true
    model: model
  }
}
