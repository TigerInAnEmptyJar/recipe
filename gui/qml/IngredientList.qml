import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQml.Models 2.3

import "common/"

Rectangle {
  id: ingredientList
  property var currentObject
  property int context: 0
  property int currentIndex : -1
  function sort() {
    sortFilterModel.sort(sortFilterModel.lessThan)
  }

  border{
    color: Common.borderColor[context]
    width: Common.borderWidth
  }
  color: Common.backgroundColor[context]

  Component {
    id: ingredientDelegate
    Rectangle {
      height: Common.textHeight
      width: iView.width
      color: Common.backgroundColor[context]
      RowLayout {
        anchors.fill: parent
        Image {
          source: image
          Layout.maximumHeight: Common.textHeight
          Layout.maximumWidth: Common.textHeight
          fillMode: Image.PreserveAspectFit
        }
        Text {
          text: name
          color: Common.textColor[context]
          font.pointSize: Common.fontSize
          Layout.fillHeight: true
          Layout.fillWidth: true
        }
      }
      MouseArea{
        anchors.fill: parent
        onClicked: {
          ingredientList.currentIndex = index
          ingredientList.currentObject = model
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

  SortFilterModel {
    id: sortFilterModel
    model: ingredients
    filterAcceptsItem: function(item) {
      return true
    }
    lessThan: function(left, right) {
      if (left.category === right.category)
        return left.name < right.name ? -1 : 1
      return left.category < right.category ? -1 : 1;
    }
    delegate: ingredientDelegate
  }

  ListView {
    id: iView
    anchors.fill: parent
    anchors.margins: Common.margin
    model: sortFilterModel
    highlight: highlightDelegate
    highlightFollowsCurrentItem: true
    focus: true
    section{
      property: "category"
      delegate: sectionDelegate
    }
  }
}
