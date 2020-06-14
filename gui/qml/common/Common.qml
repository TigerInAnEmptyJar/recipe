pragma Singleton
import QtQuick 2.12

QtObject {
  readonly property var hues: [0.17, 0, 0.50, 0.7]
  property var textColor: [Qt.hsva(hues[0], 1, 0.5), Qt.hsva(hues[1], 1, 0.5),
                           Qt.hsva(hues[2], 1, 0.5), Qt.hsva(hues[3], 1, 0.5)]
  property var backgroundColor: [Qt.hsva(hues[0], 0.1, 1), Qt.hsva(hues[1], 0.1, 1),
                                 Qt.hsva(hues[2], 0.1, 1), Qt.hsva(hues[3], 0.1, 1)]
  property var buttonColor: [Qt.hsva(hues[0], 0.9, 1), Qt.hsva(hues[1], 0.9, 1),
                                 Qt.hsva(hues[2], 0.9, 1), Qt.hsva(hues[3], 0.9, 1)]
  property var disabledButtonColor: [Qt.hsva(hues[0], 0.9, 0.5), Qt.hsva(hues[1], 0.9, 0.5),
                                     Qt.hsva(hues[2], 0.9, 0.5), Qt.hsva(hues[3], 0.9, 0.5)]
  property var buttonTextColor: [Qt.hsva(hues[0], 0.9, 0.1), Qt.hsva(hues[1], 0.9, 0.1),
                                 Qt.hsva(hues[2], 0.9, 0.1), Qt.hsva(hues[3], 0.9, 0.1)]
  property var borderColor: [Qt.hsva(hues[0], 1, 1), Qt.hsva(hues[1], 1, 1),
                                 Qt.hsva(hues[2], 1, 1), Qt.hsva(hues[3], 1, 1)]
  property int margin: 10
  property int spacing: 5
  property int borderWidth: 3
  property int textHeight: 25
  property real fontSize: 10
  property int controlHeight: 30
}
