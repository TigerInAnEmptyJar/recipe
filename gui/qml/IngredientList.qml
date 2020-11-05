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
  function unsetCurrentObject() {
    currentIndex = -1
    currentObject = undefined
    iView.currentIndex = -1
  }

  border{
    color: Common.borderColor[context]
    width: Common.borderWidth
  }
  color: Common.backgroundColor[context]

  Component {
    id: ingredientDelegate
    Rectangle {
      height: Common.textHeight + 2
      width: iView.width
      color: Common.backgroundColor[context]
      border {
        width: index == iView.currentIndex ? Common.borderWidth : 0
        color: Common.borderColor[context]
      }
      clip: true

      Drag.active: mouseArea.drag.active
      Drag.dragType: Drag.Automatic
      Drag.supportedActions: Qt.LinkAction
      Drag.mimeData: {
          "text/plain": "Copied ingredient: " + name
      }
      Drag.source: model
      RowLayout {
        anchors.fill: parent
        anchors.margins: Common.borderWidth
        Image {
          source: image !== undefined ? image : ""
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
        id: mouseArea
        anchors.fill: parent
        drag.target: parent
        onPressed: parent.grabToImage(function(result) {
            parent.Drag.imageSource = result.url
        })
        onClicked: {
          iView.currentIndex = index
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

  ListView {
    id: iView
    anchors.fill: parent
    anchors.margins: Common.borderWidth
    model: ingredients
    highlight: highlightDelegate
    highlightFollowsCurrentItem: true
    boundsBehavior: Flickable.StopAtBounds
    delegate: ingredientDelegate
    clip: true
    focus: true
    section{
      property: "category"
      delegate: sectionDelegate
    }
  }
}
