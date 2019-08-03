import QtQuick 2.2
import QtQuick.Controls.Material 2.12

Item {
    id: banner
    property alias message : messageText.text
    property int fullHeight: background.height
    signal clickedHide();

    property string type: "normal"

    function getColorType(messageType) {
        switch(type) {
        case "normal": return "#406D9E";
        case "bad": return Material.color(Material.Red);
        case "neutral" : return Material.color(Material.Grey);
        case "good" : return Material.color(Material.Green);
        }
        return Material.color(Material.Red);
    }

    Rectangle {
        id: background
        width: parent.width
        height: messageText.paintedHeight > 70 ? messageText.paintedHeight : 70
        color: getColorType(type)
        smooth: true
        opacity: 0.8
        MouseArea {
            anchors.fill: parent
            Text {
                id: messageText
                font.pixelSize: 24
                renderType: Text.QtRendering
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                color: "white"
            }
            onClicked: {
                clickedHide()
            }
        }
    }

    states: State {
        name: "portrait"
        PropertyChanges { target: banner; height: 50 }
    }
}
