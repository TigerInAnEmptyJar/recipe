import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 6.8

Item {
  id: rStringlistView
  property var model
  property string title
  property int context
  property bool readonly: true
  signal remove(string element)
  signal add(string element)
  property var getDisplay: function(item){return model[item];}

  Layout.fillWidth: true
  Layout.maximumHeight: Common.controlHeight
  Layout.minimumHeight: Common.controlHeight

  Component {
    id: listElement
    Rectangle {
      height: listview.height
      width: listText.implicitWidth * 1.5
      radius: Common.spacing
      border{
        color: Common.borderColor[rStringlistView.context]
        width: Common.borderWidth -2
      }
      color: Common.buttonColor[rStringlistView.context]
      RowLayout {
        anchors.fill: parent
        Text {
          id: listText
          Layout.fillHeight: true
          Layout.fillWidth: true
          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter
          text: getDisplay(index)
          color: Common.buttonTextColor[rStringlistView.context]
        }
        RButton {
          context: rStringlistView.context
          text: "x"
          Layout.maximumWidth: Common.textHeight
          Layout.maximumHeight: Common.textHeight * 0.5
          visible: !rStringlistView.readonly
          onClicked: {
            remove(listText.text)
          }
        }
      }
    }
  }

  RowLayout {
    anchors.fill: parent
    Text {
      id: titleText
      text: title
      font.pointSize: Common.fontSize
      font.bold: true
      Layout.maximumHeight: Common.textHeight
      Layout.maximumWidth: implicitWidth
      horizontalAlignment: Text.AlignLeft
      verticalAlignment: Text.AlignVCenter
    }
    ListView {
      id: listview
      Layout.fillWidth: true
      Layout.preferredHeight: Common.textHeight
      orientation: ListView.Horizontal
      delegate: listElement
      model: rStringlistView.model
    }
    TextInput {
      id: listInput
      visible: !rStringlistView.readonly
      Layout.maximumHeight: Common.textHeight
      Layout.minimumWidth: Common.textHeight
      Rectangle {
        Layout.preferredWidth: listInput.implicitWidth
        color: "white"
        anchors.fill: parent
        z: -1
      }
    }

    RButton {
      id: plusButton
      visible: !rStringlistView.readonly
      context: rStringlistView.context
      text: "+"
      Layout.maximumWidth: Common.textHeight
      Layout.maximumHeight: Common.textHeight * 0.5
      onClicked: {
        if (listInput.text !== ""){
          add(listInput.text)
          listInput.text = ""
        }
      }
    }
  }
}
