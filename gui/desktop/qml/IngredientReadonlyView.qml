import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQml.Models 2.3

import "common/"

Item {
  id: ingredientView
  property var object
  property int context: 0

  QtObject {
    id: object_values
    property string object_name
    property string object_category
    property string object_amount
    property bool object_sectioned
    property string object_image_source
    property string object_image_path
  }

  Component {
    id: label
    Text {
      id: labelText
      height: Common.textHeight
      Layout.fillHeight: true
      color: Common.textColor[context]
      font.pointSize: Common.fontSize
      font.bold: true
    }
  }

  Component {
    id: ingredientText
    Text {
      id: valueText
      height: Common.textHeight
      Layout.fillHeight: true
      color: Common.textColor[context]
      font.pointSize: Common.fontSize
    }
  }

  Component {
    id: ingredientCheckbox
    CheckBox {
      id: valueCheck
      display: AbstractButton.IconOnly
      enabled: false
    }
  }

  Component {
    id: ingredientImage
    RowLayout {
      property alias source: valueImage.source
      property alias path: valuePath.text
      height: Common.textHeight
      Layout.minimumHeight: Common.textHeight
      Layout.minimumWidth: Common.textHeight
      Layout.preferredWidth: valuePath.implicitWidth
      Image {
        id: valueImage
        Layout.maximumHeight: Common.textHeight
        Layout.maximumWidth: Common.textHeight
        fillMode: Image.PreserveAspectFit
        width: height
        cache: false
      }
      Text {
        id: valuePath
        font.pointSize: Common.fontSize
        Layout.maximumHeight: Common.textHeight
        Layout.fillWidth: true
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
      }
    }
  }

  ObjectModel {
    id: items
    Loader {sourceComponent: label; Component.onCompleted: children[0].text = qsTr("Name")}
    Loader {
      sourceComponent: ingredientText
      Component.onCompleted: children[0].text = Qt.binding(function(){return object_values.object_name})
    }
    Loader {sourceComponent: label; Component.onCompleted: children[0].text = qsTr("Category")}
    Loader {
      sourceComponent: ingredientText
      Component.onCompleted: children[0].text = Qt.binding(function(){return object_values.object_category})
    }
    Loader {sourceComponent: label; Component.onCompleted: children[0].text = qsTr("Default amount type")}
    Loader {
      sourceComponent: ingredientText
      Component.onCompleted: children[0].text = Qt.binding(function(){return object_values.object_amount})
    }
    Loader {sourceComponent: label; Component.onCompleted: children[0].text = qsTr("Image path")}
    Loader {
      sourceComponent: ingredientImage
      Component.onCompleted: {
        children[0].source = Qt.binding(function(){return object_values.object_image_source})
        children[0].path = Qt.binding(function(){return object_values.object_image_path})
      }
    }
    Loader {sourceComponent: label; Component.onCompleted: children[0].text = qsTr("Is sectioned")}
    Loader {
      sourceComponent: ingredientCheckbox
      Component.onCompleted: children[0].checked = Qt.binding(function(){return object_values.object_sectioned})
    }
  }

  Rectangle {
    anchors.fill: parent
    border {
      color: Common.borderColor[context]
      width: Common.borderWidth
    }
    color: Common.backgroundColor[context]

    Grid {
      anchors.fill: parent
      anchors.margins: Common.margin
      columns: 2
      rows: 5
      spacing: Common.spacing
      Repeater {model: items}
    }

    GridLayout {
      visible: false
      anchors.fill: parent
      anchors.margins: Common.margin
      columns: 2
      RLabeledControl {
        context: ingredientView.context
        label: qsTr("Name")
        Text {
          id: objectName
          font.pointSize: Common.fontSize
          verticalAlignment: Text.AlignVCenter
          horizontalAlignment: Text.AlignLeft
        }
      }
      RLabeledControl {
        context: ingredientView.context
        label: qsTr("Category")
        Text {
          id: objectCategory
          font.pointSize: Common.fontSize
          horizontalAlignment: Text.AlignLeft
          verticalAlignment: Text.AlignVCenter
        }
      }
      RLabeledControl {
        context: ingredientView.context
        label: qsTr("Default amount type")
        Text {
          id: objectAmount
          font.pointSize: Common.fontSize
          horizontalAlignment: Text.AlignLeft
          verticalAlignment: Text.AlignVCenter
        }
      }
      RLabeledControl {
        context: ingredientView.context
        label: qsTr("Image path")
        RowLayout {
          Image {
            id: objectImage
            Layout.maximumHeight: Common.textHeight
            Layout.maximumWidth: Common.textHeight
            fillMode: Image.PreserveAspectFit
            width: height
          }
          Text {
            id: objectImagePath
            font.pointSize: Common.fontSize
            Layout.maximumHeight: Common.textHeight
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
          }
        }
      }
      RLabeledControl {
        context: ingredientView.context
        label: qsTr("Is sectioned")
        CheckBox {
          id: objectSectioned
          display: AbstractButton.IconOnly
          enabled: false
        }
      }
      Item {Layout.fillHeight: true}
      Item {
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
    }
  }
  onObjectChanged: {
    if (object !== undefined&& object !== null) {
      object_values.object_name = object.name
      object_values.object_sectioned = object.isSectioned
      object_values.object_amount = object.default_amount
      object_values.object_category = object.category
      if (object.image_path !== undefined) {
        object_values.object_image_path = object.image_path
        object_values.object_image_source = object.image
      } else {
        object_values.object_image_path = "buh"
        object_values.object_image_source = ""
      }

      objectName.text = object.name
      objectCategory.text = object.category
      objectAmount.text = object.default_amount
      objectSectioned.checked = object.isSectioned
      if (object.image_path !== undefined) {
        objectImagePath.text = object.image_path
        objectImage.source = object.image
      } else {
        objectImagePath.text = "buh"
        objectImage.source = ""
      }
    }
  }
}
