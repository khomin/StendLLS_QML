import QtQuick 2.2
import QtQuick.Controls.Material 2.12

Item {
    id: banner
    property alias message : messageText.text

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
        anchors.fill: banner
        color: getColorType(type)
        smooth: true
        opacity: 0.8
    }
    Text {
        font.pixelSize: 24
        renderType: Text.QtRendering
        width: 150
        height: 40

        id: messageText
        anchors.fill: banner
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        color: "white"
    }

    states: State {
        name: "portrait"
        PropertyChanges { target: banner; height: 50 }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            clickedHide()
        }
    }
}
