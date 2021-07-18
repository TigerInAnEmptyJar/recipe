import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQml.Models 2.3

import "common/"

Rectangle {
  id: recipeList
  property var currentObject
  property int context: Common.context_recipe
  property int currentIndex: -1
  property bool filterFavorites: false
  function unsetCurrentObject(){
    currentIndex = -1
    currentObject = undefined
    iView.currentIndex = -1
  }

  border {
    color: Common.borderColor[context]
    width: Common.borderWidth
  }
  color: Common.backgroundColor[context]

  Component {
    id: recipeDelegate
    Rectangle {
      height: Common.textHeight + Common.borderWidth*2
      width: iView.width
      border {
        width: iView.currentIndex == index ? Common.borderWidth : 0
        color: Common.textColor[context]
      }
      color: Common.backgroundColor[context]
      clip: true
      Drag.active: mouseArea.drag.active
      Drag.dragType: Drag.Automatic
      Drag.supportedActions: Qt.LinkAction
      Drag.source: model
      Drag.mimeData: {"text/plain": "Copied recipe: " + title}
      RowLayout {
        anchors.fill: parent
        anchors.margins: Common.borderWidth
        Image {
          source: image !== undefined ? image : ""
          Layout.maximumHeight: Common.textHeight
          Layout.maximumWidth: Common.textHeight
          fillMode: Image.PreserveAspectFit
          sourceSize.width: 256
        }
        Rectangle {
          Layout.fillHeight: true
          Layout.fillWidth: true
          color: Common.backgroundColor[context]
          border{
            color: Common.borderColor[context]
            width: sessionFavorite ? Common.borderWidth : 0
          }
          Text {
            anchors.fill: parent
            anchors.margins: parent.border.width
            text: model.title
            color: Common.textColor[context]
            font.pointSize: Common.fontSize
          }
        }
      }
      MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: parent
        onPressed: parent.grabToImage(function(result){parent.Drag.imageSource = result.url})
        onClicked: {
          iView.currentIndex = index
          recipeList.currentIndex = index
          recipeList.currentObject = model
        }
        onDoubleClicked: sessionFavorite = !sessionFavorite
      }
    }
  }

  Component {
    id: highlightDelegate
    Rectangle {
      height: Common.textHeight + 2
      width: iView.width
      border {
        width: Common.borderWidth
        color: Common.textColor[context]
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

  RecipeFilterDialog {
    id: filterDialog
    visible: false
    onAccepted: {
      sortModel.lessThan = filterDialog.lessThan
      sortModel.filterAcceptsItem = filterDialog.filterAcceptsItem
      sortModel.update()
    }
  }

  ColumnLayout {
    anchors.fill: parent
    anchors.margins: Common.borderWidth
    RowLayout {
      RButton {
        id: enableFilter
        context: recipeList.context
        Layout.fillWidth: true
        text: qsTr("Filter & Sort")
        onClicked: {
          filterDialog.open()
        }
      }
    }
    ListView {
      id: iView
      clip: true
      Layout.fillHeight: true
      Layout.fillWidth: true
      model: SortFilterModel{
        id: sortModel
        model: recipes
        delegate: recipeDelegate
        lessThan: (a, b) => (a.meal_type === b.meal_type ? a.title < b.title : a.meal_type < b.meal_type)
        filterAcceptsItem: (a) => (enableFilter.checked ? a.tags.includes(tagFilter.text) && !filterFavorites || a.sessionFavorite : !filterFavorites || a.sessionFavorite)
        Component.onCompleted: update()
      }
      highlight: highlightDelegate
      highlightFollowsCurrentItem: true
      focus: true
      section {
        property: "meal_type"
        delegate: sectionDelegate
      }
    }
  }
}
