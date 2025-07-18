import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 6.8

import "common/"

Rectangle {
  id: page
  property int context: 0
  property var model
  property var currentObject
  property string text: "ingredient"

  signal editRequested()
  signal newRequested()
  signal deleteRequested()
  signal bookRequested()

  border{
    color: Common.borderColor[context]
    width: Common.borderWidth
  }
  color: Qt.hsva(0.17, 0.1, 1)

  GridLayout {

    anchors.fill: parent
    anchors.margins: Common.margin
    columns: 3
    columnSpacing: Common.spacing
    rowSpacing: Common.spacing
    RButton {
      text: "Load"
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      FileDialog {
        id: loadDialog
        title: "Please choose a " + page.text + " file"
        nameFilters: [ page.text + " json files (*.json)", "All files (*)" ]
        //selectExisting: true
        //selectMultiple: false
        currentFolder: "file://" + model.databasePath() + "/"
        onAccepted: {
          page.model.load(loadDialog.fileUrl)
          close()
        }
        onRejected: {
          close()
        }
      }
      onClicked: {
        loadDialog.currentFolder = "file://" + model.databasePath() + "/"
        loadDialog.visible = true
      }
    }
    RButton {
      text: "Save"
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      onClicked: page.model.storeLast()
    }
    RButton {
      text: "Save as"
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      FileDialog {
        id: storeDialog
        title: "Please choose a file to store " + page.text + "s"
        nameFilters: [ page.text + " json files (*.json)", "All files (*)" ]
        // selectExisting: false
        // selectMultiple: false
        currentFolder: "file://" + model.databasePath() + "/"
        onAccepted: {
          page.model.store(storeDialog.fileUrl)
          close()
        }
        onRejected: {
          close()
        }
      }
      onClicked: {
        storeDialog.currentFolder = "file://" + model.databasePath() + "/"
        storeDialog.open()
      }
    }
    RButton {
      text: "Edit"
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      enabled: page.currentObject !== undefined
      onClicked: editRequested()
    }
    RButton {
      text: "New"
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      onClicked: newRequested()
    }
    RButton {
      text: "Delete"
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      enabled: page.currentObject !== undefined
      onClicked: deleteRequested()
    }
    RButton {
      text: "Export Cookbook"
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      enabled: page.currentObject !== undefined
      onClicked: bookRequested()
    }
  }
}
