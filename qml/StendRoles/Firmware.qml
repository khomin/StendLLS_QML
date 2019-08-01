import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

import "qrc:/qml" as QmlCustomize
import "qrc:/qml/delegates/" as Delegates
import "qrc:/qml/toast/" as Toast

Rectangle {
    Connections {
        target: viewControl
        onSearchStendComplete: {
            busyIndicator.visible = false;
        }
        onSignalInterfaceReady: {
            statusPannel.setStatusConnected()
        }
        onSignalInterfaceError: {
            busyIndicator.visible = false;
            statusPannel.setStatusDisconnected()
            toast.displayMessage(qsTr("Opening a host returned an error"), "bad")
        }
        onSignalInterfaceClosed: {
            busyIndicator.visible = false;
            statusPannel.setStatusDisconnected()
        }
        onSignalStendLost: {
            toast.displayMessage(qsTr("Connection lost"), "bad")
            viewControl.closeConnection()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        Pane {
            Layout.fillWidth: true
            Material.elevation: 6
            Layout.alignment: Qt.AlignLeft
            implicitHeight: 50
            background: Rectangle {
                width: parent.width
                color: "#406D9E"
            }
            RowLayout {
                Layout.fillWidth: true
                implicitHeight: 50
                anchors.verticalCenter: parent.verticalCenter
                TabBar {
                    Material.accent: "white"
                    id: tabBar
                    implicitWidth: 300
                    implicitHeight: 50
                    background: Rectangle {
                        width: tabBar.width
                        color: "#406D9F"
                    }
                    TabButton {
                        text: qsTr("Communication"); font.pointSize: 8
                        icon.source: "qrc:/svg/resources/fonts/svgs/solid/satellite-dish.svg"
                        icon.width: 10; icon.height: 10; height: 40
                        onClicked: { firmwareSwipeView.setCurrentIndex(0)}
                        contentItem: Delegates.TabButtonDelegate {}
                    }
                    TabButton {
                        text: qsTr("Communication"); font.pointSize: 8
                        icon.source: "qrc:/svg/resources/fonts/svgs/solid/satellite-dish.svg"
                        icon.width: 10; icon.height: 10; height: 40
                        onClicked: { firmwareSwipeView.setCurrentIndex(0)}
                        contentItem: Delegates.TabButtonDelegate {}
                    }
                    TabButton {
                        text: qsTr("DataBase"); font.pointSize: 8
                        icon.source: "qrc:/svg/resources/fonts/svgs/solid/tty.svg"
                        icon.width: 10; icon.height: 10; height: 40
                        onClicked: { firmwareSwipeView.setCurrentIndex(1)}
                        contentItem: Delegates.TabButtonDelegate {}
                    }
                }
                ToolBar {
                    id:toolBarButtonControl
                    Layout.alignment: Qt.AlignRight
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
                SwipeView { id: firmwareSwipeView
                    spacing: 50
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    FirmwarePanel {
                        id:firmwareTestItem
                    }
                    Item {
                        id:firmwareDatabaseItem
                    }
                    Item {
                        id:firmwareFindStendItem
                        //-- find stend panel
                        Pane {
                            id: findStendsPanel
                            Material.elevation: 2
                            implicitWidth: parent.width - 50
                            implicitHeight: parent.height - 50
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            anchors.top: parent.top
                            anchors.topMargin: 10
                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                            ColumnLayout {
                                width: parent.width
                                spacing: 30
                                Label{ text: qsTr("Find stend panel"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}

                                RowLayout {
                                    spacing: 15
                                    Label {text: qsTr("Active stend:")}
                                    Label {id:stendFindActiveLabel; text: settings.activeStend; }
                                }
                                RowLayout {
                                    spacing: 15
                                    Label { text: qsTr("Total found:")}
                                    Label { id:stendFindTotalLabel; text: stendFindListView.count  }
                                    Button { id:stendRestartFindButton;
                                        Material.background: Material.Green;
                                        Material.foreground: "white";
                                        text: qsTr("Restart search");
                                        font.pointSize: 8
                                        implicitHeight: 50;
                                        implicitWidth: 150
                                        Layout.alignment: Qt.AlignTop
                                        onClicked: {
                                            viewControl.startFindStends();
                                            busyIndicator.visible = true;
                                            firmwareSwipeView.setCurrentIndex(2);
                                        }
                                    }
                                }
                                RowLayout {
                                    ListView { id:stendFindListView
                                        width: 400
                                        height: 500
                                        clip: true
                                        model: stendFindModel
                                        delegate: Rectangle {
                                            property int indexOfThisDelegate: index
                                            width: parent.width
                                            height: 50
                                            color: "#f0f0f5"
                                            RowLayout {
                                                width: parent.width
                                                anchors.right: parent.right
                                                anchors.left: parent.left
                                                anchors.rightMargin: 10
                                                anchors.leftMargin: 10
                                                TextEdit {
                                                    text: model.ip
                                                    color: "black"
                                                    font.bold: false
                                                    font.pointSize: 9
                                                    readOnly: true
                                                    Layout.alignment: Qt.AlignVCenter
                                                }
                                                Button {
                                                    Material.background: "#406d9e";
                                                    Material.foreground: "white";
                                                    text: qsTr("Add to active");
                                                    font.pointSize: 8
                                                    implicitHeight: 50;
                                                    implicitWidth: 150
                                                    Layout.alignment: Qt.AlignRight
                                                    onClicked: {
                                                        viewControl.setStendActiveIp(indexOfThisDelegate)
                                                    }
                                                }
                                            }
                                        }
                                        ScrollIndicator.horizontal: ScrollIndicator {}
                                        ScrollIndicator.vertical: ScrollIndicator {}
                                    }
                                }
                            }
                        }
                    }
                }

                QmlCustomize.StatusPannel {
                    id:statusPannel
                    onRequestFindStends: {
                        busyIndicator.visible = true;
                        firmwareSwipeView.setCurrentIndex(2)
                    }
                    onConnectActivated: {
                        busyIndicator.visible = true;
                    }
                }
            }
        }
    }
}
