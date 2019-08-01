import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.2 as QtQuickOld
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import Qt.labs.platform 1.0 as QtLabs
import QtGraphicalEffects 1.0

import "qrc:/qml" as QmlCustomize
import "qrc:/qml/delegates/" as Delegates
import "qrc:/qml/toast/" as Toast

Item {
    ColumnLayout {
        anchors.fill: parent
        Pane {
            Layout.fillWidth: true
            width: parent.width
            Material.elevation: 6
            Layout.alignment: Qt.AlignLeft
            implicitHeight: 50
            background: Rectangle {
                width: parent.width
                color: "#406D9E"
            }

            RowLayout {
                Layout.fillWidth: true
                width: parent.width
                implicitHeight: 50
                anchors.verticalCenter: parent.verticalCenter
                TabBar {
                    Material.accent: "white"
                    id: tabBar
                    implicitWidth: parent.width - toolBarButtonControl.width
                    background: Rectangle {
                        width: parent.width
                        color: "#406D9F"
                    }
                    TabButton {
                        text: qsTr("Communication"); font.pointSize: 8
                        icon.source: "qrc:/svg/resources/fonts/svgs/solid/satellite-dish.svg"
                        icon.width: 10; icon.height: 10; height: 40
                        onClicked: { settingsPannel.setCurrentIndex(propertyCentralSwipeLastIndex)}
                        contentItem: Delegates.TabButtonDelegate {}
                    }
                    TabButton {
                        text: qsTr("System"); font.pointSize: 8
                        icon.source: "qrc:/svg/resources/fonts/svgs/solid/tty.svg"
                        icon.width: 10; icon.height: 10; height: 40
                        onClicked: { settingsPannel.setCurrentIndex(propertyCentralSwipeLastIndex)}
                        contentItem: Delegates.TabButtonDelegate {}
                    }
                    TabButton {
                        text: qsTr("dddddddd"); font.pointSize: 8
                        icon.source: "qrc:/svg/resources/fonts/svgs/solid/feather-alt.svg"
                        icon.width: 10; icon.height: 10; height: 40
                        onClicked: { settingsPannel.setCurrentIndex(propertyCentralSwipeLastIndex)}
                        contentItem: Delegates.TabButtonDelegate {}
                    }
                }
                ToolBar {
                    id:toolBarButtonControl
                    RowLayout {
                        anchors.fill: parent
                        ToolButton {
                            contentItem: Text {
                                text: "..."
                                color: "white"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }
                            Menu {
                                id: toolButtonMenu
                                MenuItem { text: qsTr("Settings")
                                    icon.source: "qrc:/svg/resources/fonts/svgs/solid/cogs.svg"
                                    icon.width: 14; icon.height: 14
                                    font.pointSize: 8;
                                    onClicked: {
                                        settingsPannel.setCurrentWidget("settings")
                                    }
                                }
                                MenuItem { text: qsTr("Unit test")
                                    icon.source: "qrc:/svg/resources/fonts/svgs/solid/vial.svg"
                                    icon.width: 14; icon.height: 14
                                    font.pointSize: 8;
                                    onClicked: {
                                        settingsPannel.setCurrentWidget("unitTestsWidget")
                                    }
                                }
                                MenuItem { text: qsTr("About")
                                    icon.source: "qrc:/svg/resources/fonts/svgs/solid/address-card.svg"
                                    icon.width: 14; icon.height: 14
                                    font.pointSize: 8;
                                    onClicked: {
                                        settingsPannel.setCurrentWidget("about")
                                    }
                                }
                            }
                            onClicked: {
                                toolButtonMenu.popup()
                            }
                        }
                    }
                    background: Rectangle {
                        implicitWidth: 40
                        implicitHeight: 40
                        color: "transparent"
                    }
                }
            }
        }

        RowLayout {
            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true

                //                SettingsPannel { id: settingsPannel
                //                    onRequestRebootTracker: {
                //                        validatePassDialogWrite.command = "reboot"
                //                        validatePassDialogWrite.clearFilds()
                //                        validatePassDialogWrite.open();
                //                    }
                //                    onSettingIsRead: {
                //                        if(statusPannel.statusConnected) {
                //                            toast.displayMessage(qsTr("Settings successfully received"), "normal")
                //                        }
                //                    }
                //                    onSettingIsWrited: {
                //                        if(statusPannel.statusConnected) {
                //                            toast.displayMessage(qsTr("Settings successfully written"), "normal")
                //                        }
                //                    }
                //                    onTrackerIsRebooted: {
                //                        if(statusPannel.statusConnected) {
                //                            toast.displayMessage(qsTr("Tracker successfully restarted"), "normal")
                //                            viewControl.closeConnection()
                //                            busyIndicator.visible = false;
                //                            statusPannel.statusConnected = false
                //                            watchConnectTimer.stop()
                //                        }
                //                    }
                //                }
                //                StatusPannel {
                //                    id:statusPannel
                //                    onRequestReadSettingsFirtTime: {
                //                        busyIndicator.visible = true;
                //                        validatePassDialogRead.command = "read"
                //                        validatePassDialogRead.clearFilds()
                //                        validatePassDialogRead.open();
                //                    }
                //                    onRequestReadSettings: {
                //                        viewControl.readSettings(validatePassDialogRead.getPassword())
                //                    }
                //                    onRequestWriteSettings: {
                //                        validatePassDialogWrite.command = "write"
                //                        validatePassDialogWrite.clearFilds()
                //                        validatePassDialogWrite.open();
                //                    }
                //                    onConnectActivated: {
                //                        busyIndicator.visible = true;
                //                        watchConnectTimer.restart()
                //                    }
                //                    onDisconnectActivated: {
                //                        busyIndicator.visible = false;
                //                        watchConnectTimer.restart()
                //                        statusPannel.statusConnected = false
                //                    }
                //                }
                //                QmlCustomize.ValidatePassDialog {
                //                    property var command: ""
                //                    id:validatePassDialogRead
                //                    onOpened: {
                //                        validatePassDialogRead.forceActiveFocus()
                //                    }
                //                    onSignalReady: {
                //                        busyIndicator.visible = true;
                //                        var password = validatePassDialogRead.getPassword()
                //                        if(command === "read") {
                //                            viewControl.readSettings(password)
                //                        } else if(command === "write") {
                //                            var jsonSettings = settingsPannel.getJsonSettings()
                //                            viewControl.writeSettings(password, jsonSettings)
                //                        }
                //                        validatePassDialogRead.close()
                //                    }
                //                    onSignalExit: {
                //                        busyIndicator.visible = true;
                //                    }
                //                }
                //                QmlCustomize.ValidatePassDialog {
                //                    property var command: ""
                //                    id:validatePassDialogWrite
                //                    onOpened: {
                //                        validatePassDialogWrite.forceActiveFocus()
                //                    }
                //                    onSignalReady: {
                //                        busyIndicator.visible = true;
                //                        var password = validatePassDialogWrite.getPassword()
                //                        if(command === "write") {
                //                            var jsonSettings = settingsPannel.getJsonSettings()
                //                            viewControl.writeSettings(password, jsonSettings)
                //                        } else if(command == "reboot") {
                //                            viewControl.rebootTracker(password)
                //                        }
                //                        validatePassDialogWrite.close()
                //                    }
                //                    onSignalExit: {
                //                        busyIndicator.visible = true;
                //                    }
                //                }
            }

            //            RealTimePannel { id:realTimePannel }
        }
    }
}
