import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import "common/"

Rectangle {
  id: page
  property int context: 0
  property var model
  property var currentObject
  property string text: "plan"

  signal editRequested()
  signal newRequested()
  signal deleteRequested()

  border{
    color: Common.borderColor[context]
    width: Common.borderWidth
  }
  color: Qt.hsva(0.17, 0.1, 1)

  ColumnLayout {
    anchors.fill: parent
    anchors.margins: Common.margin
    spacing: Common.spacing
    RButton {
      text: qsTr("Load")
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      FileDialog {
        id: loadDialog
        title: qsTr("Please choose a %1 file", page.text)
        nameFilters: [ page.text + " json files (*.json)", "All files (*)" ]
        selectExisting: true
        selectMultiple: false
        folder: "file://" + model.databasePath() + "/"
        onAccepted: {
          page.model.load(loadDialog.fileUrl)
          close()
        }
        onRejected: {
          close()
        }
      }
      onClicked: {
        loadDialog.folder = "file://" + model.databasePath() + "/"
        loadDialog.visible = true
      }
    }
    RButton {
      text: qsTr("Save")
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      onClicked: page.model.store()
    }
    RButton {
      text: qsTr("Save as")
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      FileDialog {
        id: storeDialog
        title: qsTr("Please choose a file to store %1", page.text)
        nameFilters: [ page.text + " json files (*.json)", "All files (*)" ]
        selectExisting: false
        selectMultiple: false
        folder: "file://" + model.databasePath() + "/"
        onAccepted: {
          page.model.storeAs(storeDialog.fileUrl)
          close()
        }
        onRejected: {
          close()
        }
      }
      onClicked: {
        storeDialog.folder = "file://" + model.databasePath() + "/"
        storeDialog.open()
      }
    }
    RButton {
      text: qsTr("Export")
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      FileDialog {
        id: exportDialog
        title: qsTr("Please choose a file to export %1", page.text)
        nameFilters: model.exportFormats()
        selectExisting: false
        selectMultiple: false
        folder: "file://" + model.databasePath() + "/"
        onAccepted: {
          page.model.exportPlan(exportDialog.fileUrl, exportDialog.selectedNameFilterIndex)
          close()
        }
        onRejected: {
          close()
        }
      }
      onClicked: {
        exportDialog.folder = "file://" + model.databasePath() + "/"
        exportDialog.open()
      }
    }
    RButton {
      text: qsTr("New")
      Layout.fillHeight: true
      Layout.fillWidth: true
      context: page.context
      onClicked: newRequested()
    }
  }
}
