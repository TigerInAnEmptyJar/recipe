import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3


Item {
  id: ingredientView
  property var object
  property bool readonly: true

  Rectangle{
    anchors.fill: parent
    border{
      color: Qt.hsva(0.17, 1, 1)
      width: 3
    }
    color: Qt.hsva(0.17, 0.1, 1)
    GridLayout {
      anchors.fill: parent
      anchors.margins: 5
      columns: 2
      Text {
        text: "Name"
      }
      Text {
        id: objectName
      }
      Text {
        text: "Category"
      }
      Text {
        id: objectCategory
      }
      Text {
        text: "Default amount type"
      }
      Text {
        id: objectAmount
      }
      Text {
        text: "Image path"
      }
      Text {
        id: objectImagePath
      }
      Text {
        text: "Is sectioned"
      }
      CheckBox {
        id: objectSectioned
        text: "Sectioned"
        enabled: !ingredientView.readonly
      }
    }
  }
  onObjectChanged: {
    if (object !== undefined) {
      objectName.text = object.name
      objectCategory.text = object.category
      objectImagePath.text = object.image_path
      objectAmount.text = object.default_amount
      objectSectioned.checked = object.isSectioned
    }
  }
  visible: object !== undefined
}
