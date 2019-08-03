import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12

Loader {
    id: messages

    function displayMessage(message, type) {
        flush()
        messages.source = "";
        messages.source = Qt.resolvedUrl("InfoBannerComponent.qml");
        messages.item.message = message;
        messages.item.type = type
        timer.interval = 99999999;
        timer.restart()
        messages.item.clickedHide.connect(function() {
            messages.state = ""
            messages.source = ""
        });
    }

    function flush() {
        messages.state = ""
        timer.interval = 0
        timer.restart()
    }

    width: parent.width
    anchors.bottom: parent.bottom
    anchors.bottomMargin: -50
    z: 1

    onLoaded: {
        messages.item.state = "portrait";
        timer.running = true
        messages.state = "show"
    }

    Timer { id: timer
        interval: 8000
        onTriggered: {
            messages.state = ""
        }
    }

    states: [
        State {
            name: "show"
            AnchorChanges { target: messages; anchors { bottom: parent.bottom; top: undefined} }
            PropertyChanges { target: messages; anchors.bottomMargin: 70 + messages.item.fullHeight}
        }
    ]

    transitions: Transition {
        AnchorAnimation { easing.type: Easing.OutBack; duration: 1000 }
    }
}
