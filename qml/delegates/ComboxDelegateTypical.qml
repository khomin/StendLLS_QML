import QtQuick 2.2
import QtQuick.Controls 2.12

ItemDelegate {
    width: parent.width
    text: modelData
    font.weight: parent.currentIndex === index ? Font.DemiBold : Font.Normal
    font.pointSize: 8
    highlighted: parent.highlightedIndex === index
    hoverEnabled: parent.hoverEnabled
}
