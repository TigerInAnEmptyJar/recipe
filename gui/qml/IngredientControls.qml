import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Rectangle {
  id: page
  property int context: 0
  property var model
  property var currentObject
  property string text: "ingredient"

  border{
    color: Qt.hsva(0.17, 1, 1)
    width: 3
  }
  color: Qt.hsva(0.17, 0.1, 1)

  GridLayout {

    anchors.fill: parent
    anchors.margins: 10
    columns: 2
    columnSpacing: 10
    rowSpacing: 10
    RButton {
      text: "Load"
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      FileDialog {
        id: loadDialog
        title: "Please choose a " + page.text + " file"
        nameFilters: [ page.text + " json files (*.json)", "All files (*)" ]
        selectExisting: true
        selectMultiple: false
        onAccepted: {
          page.model.load(loadDialog.fileUrl)
          visible = false
        }
        onRejected: {
          visible = false
        }
      }
      onClicked: loadDialog.visible = true
    }
    RButton {
      text: "Store"
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      FileDialog {
        id: storeDialog
        title: "Please choose a file to store " + page.text + "s"
        nameFilters: [ page.text + " json files (*.json)", "All files (*)" ]
        selectExisting: false
        selectMultiple: false
        onAccepted: {
          page.model.store(storeDialog.fileUrl)
          visible = false
        }
        onRejected: {
          visible = false
        }
      }
      onClicked: storeDialog.visible = true
    }
    RButton {
      text: "New"
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
    }
    RButton {
      text: "Delete"
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      enabled: page.currentObject !== undefined
    }
  }
}
