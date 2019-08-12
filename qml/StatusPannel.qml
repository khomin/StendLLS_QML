import QtQuick 2.0
import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.2 as QtQuickOld
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import Settings 1.0
import "qrc:/qml/delegates/" as Delegates

Pane {
    id: statusPannel
    Material.elevation: 6
    Layout.fillWidth: true

    signal connectActivated();
    signal disconnectActivated();
    //    signal requestReadSettings();
    //    signal requestReadSettingsFirtTime();
    //    signal requestWriteSettings();
    signal requestFindStends();

    property bool statusConnected: false

    function setStatusSerialOpened() {
        connectButton.statusSerialPort = true;
    }
    function setStatusSerialClosed() {
        connectButton.statusSerialPort = false;
    }
    function setStatusConnected() {
        statusConnected = true;
    }
    function setStatusDisconnected() {
        statusConnected = false;
    }
    function readSettingsFirstTime() {
        requestReadSettingsFirtTime()
    }
    function readSettings() {
        requestReadSettings()
    }

    RowLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10
        RowLayout { spacing: 10;
            id:statusOpenPortButton
            Label {text: qsTr("Stend") }
            Label {text: Settings.activeStend; }
            Button { id:connectButton;
                Material.background: statusConnected ? Material.Green : "#406D9E";
                Material.foreground: "white";
                text: statusConnected ? qsTr("Disconnect") : qsTr("Connect");
                font.pointSize: 8
                implicitHeight: 45;
                focus: false
                onClicked: {
                    if(statusConnected) {
                        disconnectActivated();
                        viewControl.closeConnection();
                    } else {
                        var params = {};
                        params.port = 45454;
                        connectActivated();
                        stendInterface.addConnection(Settings.activeStend, JSON.stringify(params));
                    }
                }
            }
            Button { Material.background: Material.BlueGrey;
                font.pointSize: 8; implicitHeight: 45; Material.foreground: "white";
                text: qsTr("Find stend");
                enabled: !connectButton.statusSerialPort
                onClicked: {
                    viewControl.startFindStends();
                    requestFindStends()
                }
            }
        }
        ColumnLayout {
            Layout.alignment: Qt.AlignLeft
            Label { id:statusLabel; font.pointSize: 8;
                Layout.alignment: Qt.AlignCenter;
                text: statusConnected ? (qsTr("Connected")) : (qsTr("Disconnected"))
                color: statusConnected ? (Material.color(Material.Green, Material.Shade800)) : (Material.color(Material.Red, Material.Shade800))
            }
            Rectangle {
                radius: 5
                width: 150
                height: 5
                color: statusConnected ? (Material.color(Material.Green, Material.Shade10)) : (Material.color(Material.Red, Material.Shade800))
            }
        }
    }
}
