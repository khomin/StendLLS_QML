import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.0

Rectangle {
    anchors.fill: parent
    color: "transparent"
    RowLayout {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        Item { width: 16; height: 16
            Image {
                id: tabIcon
                source: parent.parent.parent.parent.icon.source
                sourceSize: Qt.size(parent.width, parent.height)
                smooth: true
                visible: false
            }
            ColorOverlay { anchors.fill: tabIcon; source: tabIcon; color: "white" }
        }
        Text {

            visible: parent.parent.width - tabIcon.width > 80 ? true : false
            renderType: Text.NativeRendering
            text: parent.parent.parent.text
            font: parent.parent.parent.font
            opacity: parent.enabled ? 1.0 : 0.3
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.alignment: Qt.AlignCenter
            elide: Text.ElideRight
        }
    }
}
