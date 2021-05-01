import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQml.Models 2.3

import "common/"

Rectangle {
  id: planItemList
  property var object
  property int objectIndex: -1
  property int context: 0
  color: Common.backgroundColor[context]
  border {
    color: Common.borderColor[context]
    width: Common.borderWidth
  }

  function eatersChanged() {
    subscriberView.model = plan.eaters()
  }

  Component {
    id: planItemDelegate
    Rectangle {
      height: Common.controlHeight + Common.borderWidth * 2
      width: planView.width
      color: Common.backgroundColor[context]
      clip: true
      RowLayout {
        anchors.fill: parent
        anchors.margins: Common.borderWidth
        Image {
          source: image
          Layout.fillHeight: true
          Layout.maximumWidth: Common.controlHeight * 2
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
    }
  }
  Component {
    id: planImageDelegate
    Rectangle {
      width: planView.width
      height: Math.min(planView.width /2, planView.height)
      color: Common.backgroundColor[context]
      clip: true
      MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
      }
      Image {
        anchors.fill: parent
        source: image
        fillMode: Image.PreserveAspectFit
      }
      Rectangle {
        id: description
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: Common.textHeight
        visible: mouseArea.containsMouse
        color: Common.backgroundColor[context]
        Text {
          anchors.fill: parent
          horizontalAlignment: Text.AlignHCenter
          text: model.title
          color: Common.textColor[context]
          font.pointSize: Common.fontSize
        }
      }
    }
  }
  Component {
    id: subscriberDelegate
    CheckBox {
      height: Common.controlHeight
      width: planView.width
      clip: true
      text: modelData
      checked: plan.subscribed(objectIndex, text)
      onCheckStateChanged: {
        if (checked)
          plan.addSubscriber(objectIndex, text)
        else
          plan.removeSubscriber(objectIndex, text)
      }
    }
  }

  ColumnLayout {
    anchors.fill: parent
    anchors.margins: Common.borderWidth
    clip: true
    CheckBox{
      Layout.fillWidth: true
      Layout.maximumHeight: Common.controlHeight
      icon.color: Common.buttonColor[context]
      text: qsTr("shopping")
      checked: object.shoppingBefore
      onClicked: object.shoppingBefore = checked
    }
    Rectangle {
      Layout.fillHeight: true
      Layout.fillWidth: true
      color: Common.backgroundColor[context]
      DropArea {
        anchors.fill: parent
        onDropped: plan.addRecipe(index, drag.source.id)
      }
      ColumnLayout {
        anchors.fill: parent
        TextInput {
          text: object.name
          color: Common.textColor[context]
          onAccepted: object.name = text
          Layout.fillWidth: true
        }
        ListView {
          id: planView
          Layout.fillHeight: true
          Layout.fillWidth: true
          clip: true
          model: object.item
          focus: true
          delegate: planImageDelegate
        }
        ListView {
          id: subscriberView
          clip: true
          Layout.fillHeight: true
          Layout.fillWidth: true
          model: plan.eaters()
          delegate: subscriberDelegate
        }
      }
    }
  }
}
