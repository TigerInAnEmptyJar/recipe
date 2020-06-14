import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import "common/"

Item {
  id: ingredientView
  property var object
  property bool readonly: true
  property int context: 0

  Rectangle{
    anchors.fill: parent
    border{
      color: Common.borderColor[context]
      width: Common.borderWidth
    }
    color: Common.backgroundColor[context]
    GridLayout {
      anchors.fill: parent
      anchors.margins: Common.margin
      columns: 2
      Text {
        text: "Name"
        font.pointSize: Common.fontSize
      }
      Text {
        id: objectName
        font.pointSize: Common.fontSize
      }
      Text {
        text: "Category"
        font.pointSize: Common.fontSize
      }
      Text {
        id: objectCategory
        font.pointSize: Common.fontSize
      }
      Text {
        text: "Default amount type"
        font.pointSize: Common.fontSize
      }
      Text {
        id: objectAmount
        font.pointSize: Common.fontSize
      }
      Text {
        text: "Image path"
        font.pointSize: Common.fontSize
      }
      Text {
        id: objectImagePath
        font.pointSize: Common.fontSize
      }
      Text {
        text: "Is sectioned"
        font.pointSize: Common.fontSize
      }
      CheckBox {
        id: objectSectioned
        text: "Sectioned"
        font.pointSize: Common.fontSize
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
