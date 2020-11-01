import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQml.Models 2.3

import "common/"

Rectangle {
  id: recipeList
  property var currentObject
  property int context: 0
  property int currentIndex : -1

  border{
    color: Common.borderColor[context]
    width: Common.borderWidth
  }
  color: Common.backgroundColor[context]

  Component {
    id: recipeDelegate
    Rectangle {
      height: Common.textHeight
      width: iView.width
      color: Common.backgroundColor[context]
      RowLayout {
        anchors.fill: parent
        Image {
          source: image !== undefined ? image : ""
          Layout.maximumHeight: Common.textHeight
          Layout.maximumWidth: Common.textHeight
          fillMode: Image.PreserveAspectFit
        }
        Text {
          text: model.title
          color: Common.textColor[context]
          font.pointSize: Common.fontSize
          Layout.fillHeight: true
          Layout.fillWidth: true
        }
      }
      MouseArea{
        anchors.fill: parent
        onClicked: {
          recipeList.currentIndex = index
          recipeList.currentObject = model
        }
      }
    }
  }

  Component {
    id: highlightDelegate
    Rectangle {
      height: Common.textHeight
      width: iView.width
      border {
        width: Common.borderWidth
        color: Common.backgroundColor[context]
      }
    }
  }

  Component {
    id: sectionDelegate
    Rectangle {
      height: Common.textHeight
      width: iView.width
      anchors.margins: Common.margin
      color: Common.textColor[context]
      Text {
        anchors.fill: parent
        text: section
        color: Common.backgroundColor[context]
        font.pointSize: Common.fontSize
        horizontalAlignment: Text.AlignHCenter
      }
    }
  }

  ListView {
    id: iView
    anchors.fill: parent
    anchors.margins: Common.margin
    model: recipes
    delegate: recipeDelegate
    highlight: highlightDelegate
    highlightFollowsCurrentItem: true
    focus: true
    section{
      property: "meal_type"
      delegate: sectionDelegate
    }
  }
}
