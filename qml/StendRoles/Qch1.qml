import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "qrc:/qml" as QmlCustomize
import "qrc:/qml/delegates/" as Delegates
import "qrc:/qml/toast/" as Toast
import "qrc:/qml/StendRoles/OtherItems" as StendOtherItems

import Settings 1.0

Rectangle {
    id:qc1Rectangle
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
                implicitWidth: qc1Rectangle.width - 30
                implicitHeight: 50
                anchors.verticalCenter: parent.verticalCenter
                TabBar {
                    Material.accent: "white"
                    id: tabBar
                    implicitWidth: (qc1Rectangle.width - toolBarButtonControl.width > 600) ? 600 : qc1Rectangle.width - toolBarButtonControl.width
                    implicitHeight: 50
                    background: Rectangle {
                        width: tabBar.width
                        color: "#406D9F"
                    }
                    TabButton {
                        text: qsTr("Stend"); font.pointSize: 8
                        icon.source: "qrc:/svg/resources/fonts/svgs/solid/satellite-dish.svg"
                        icon.width: 10; icon.height: 10; height: 40
                        implicitWidth: 200
                        onClicked: { qchSwipeView.setCurrentIndex(0)}
                        contentItem: Delegates.TabButtonDelegate {}
                    }
                    TabButton {
                        text: qsTr("DataBase"); font.pointSize: 8
                        icon.source: "qrc:/svg/resources/fonts/svgs/solid/tty.svg"
                        icon.width: 10; icon.height: 10; height: 40
                        onClicked: { qchSwipeView.setCurrentIndex(1)}
                        contentItem: Delegates.TabButtonDelegate {}
                    }
                    TabButton {
                        text: qsTr("Testing"); font.pointSize: 8
                        icon.source: "qrc:/svg/resources/fonts/svgs/solid/vial.svg"
                        icon.width: 10; icon.height: 10; height: 40
                        onClicked: { qchSwipeView.setCurrentIndex(2)}
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
                                MenuItem { text: qsTr("Settings")
                                    icon.source: "qrc:/svg/resources/fonts/svgs/solid/address-card.svg"
                                    icon.width: 14; icon.height: 14
                                    font.pointSize: 8;
                                    onClicked: {
                                        onClicked: { qchSwipeView.setCurrentIndex(4)}
                                    }
                                }
                                MenuItem { text: qsTr("About")
                                    icon.source: "qrc:/svg/resources/fonts/svgs/solid/address-card.svg"
                                    icon.width: 14; icon.height: 14
                                    font.pointSize: 8;
                                    onClicked: {
                                        onClicked: { qchSwipeView.setCurrentIndex(5)}
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
                SwipeView { id: qchSwipeView
                    spacing: 50
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    interactive: false

                    //-- firmware panel
                    StendOtherItems.Qch1Panel {}

                    //-- dataBase
                    StendOtherItems.StendDataBaseSettings {}

                    //-- test settings
                    StendOtherItems.StendTestSettings {}

                    //-- find stend
                    StendOtherItems.FindStend {}

                    //-- language
                    StendOtherItems.Language {}

                    //-- about
                    StendOtherItems.About {}
                }

                QmlCustomize.StatusPannel {
                    id:statusPannel
                    onRequestFindStends: {
                        busyIndicator.visible = true;
                        qchSwipeView.setCurrentIndex(4)
                    }
                    onConnectActivated: {
                        busyIndicator.visible = true;
                    }
                }
            }
        }
    }
}
