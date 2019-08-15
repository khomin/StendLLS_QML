import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import "qrc:/qml" as QmlCustomize
import "qrc:/qml/delegates/" as Delegates
import "qrc:/qml/toast/" as Toast
import "qrc:/qml/StendRoles/OtherItems" as StendOtherItems
import Settings 1.0

Rectangle {
    id:firmwareRectangle

    Connections {
        target: viewControl
        onSearchStendComplete: {
            if(viewControl.stendRole == "firmware") {
                busyIndicator.visible = false;
            }
        }
        onSignalDataBaseError: {
            if(viewControl.stendRole == "firmware") {
                toast.displayMessage(err, "bad")
            }
        }
        onGoodMessage: {
            if(viewControl.stendRole == "firmware") {
                toast.displayMessage(text, "good")
            }
        }
        onBadMessage: {
            if(viewControl.stendRole == "firmware") {
                toast.displayMessage(text, "bad")
            }
        }
    }

    Connections {
        target: stendProp
        onStendNotReply: {
            if(viewControl.stendRole == "firmware") {
                toast.displayMessage(qsTr("Connection lost"), "bad")
                viewControl.closeConnection()
            }
        }
    }

    Connections {
        target: stendInterface
        onSignalOpened: {
            if(viewControl.stendRole == "firmware") {
            }
        }
        onSignalError: {
            if(viewControl.stendRole == "firmware") {
                busyIndicator.visible = false;
                toast.displayMessage(qsTr("Opening a host returned an error"), "bad")
            }
        }
        onSignalClosed: {
            if(viewControl.stendRole == "firmware") {
                busyIndicator.visible = false;
            }
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
                implicitWidth: firmwareRectangle.width - 30
                implicitHeight: 50
                anchors.verticalCenter: parent.verticalCenter
                TabBar {
                    Material.accent: "white"
                    id: tabBar
                    implicitWidth: (firmwareRectangle.width - toolBarButtonControl.width > 600) ? 600 : firmwareRectangle.width - toolBarButtonControl.width
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
                        onClicked: { firmwareSwipeView.currentIndex = 0 }
                        contentItem: Delegates.TabButtonDelegate {}
                    }
                    TabButton {
                        text: qsTr("DataBase"); font.pointSize: 8
                        icon.source: "qrc:/svg/resources/fonts/svgs/solid/tty.svg"
                        icon.width: 10; icon.height: 10; height: 40
                        onClicked: { firmwareSwipeView.currentIndex = 1 }
                        contentItem: Delegates.TabButtonDelegate {}
                    }
                    TabButton {
                        text: qsTr("Testing"); font.pointSize: 8
                        icon.source: "qrc:/svg/resources/fonts/svgs/solid/vial.svg"
                        icon.width: 10; icon.height: 10; height: 40
                        onClicked: { firmwareSwipeView.currentIndex = 2 }
                        contentItem: Delegates.TabButtonDelegate {}
                    }
                    TabButton {
                        text: qsTr("Firmware programming"); font.pointSize: 8
                        icon.source: "qrc:/svg/resources/fonts/svgs/solid/microchip.svg"
                        icon.width: 10; icon.height: 10; height: 40
                        onClicked: { firmwareSwipeView.currentIndex = 3 }
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
                                        onClicked: { firmwareSwipeView.currentIndex = 5 }
                                    }
                                }
                                MenuItem { text: qsTr("About")
                                    icon.source: "qrc:/svg/resources/fonts/svgs/solid/address-card.svg"
                                    icon.width: 14; icon.height: 14
                                    font.pointSize: 8;
                                    onClicked: {
                                        onClicked: { firmwareSwipeView.currentIndex = 6 }
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
                StackLayout { id: firmwareSwipeView
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    //-- firmware panel
                    StendOtherItems.FirmwarePanel {}

                    //-- dataBase
                    StendOtherItems.StendDataBaseSettings {}

                    //-- test settings
                    StendOtherItems.StendTestSettings {}

                    //-- firmware programming settings
                    StendOtherItems.StendFirmwareProgrammingSettings {}

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
                        firmwareSwipeView.currentIndex = 4
                    }
                    onConnectActivated: {
                        busyIndicator.visible = true;
                    }
                }
            }
        }
    }
}
