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
    id:qc1Rectangle
    Connections {
        target: viewControl
        onSearchStendComplete: {
            if(viewControl.stendRole == "qch2") {
                busyIndicator.visible = false;
            }
        }
    }

    StackLayout {
        id:stackLayout
        anchors.fill: parent
        currentIndex: 0

        StendOtherItems.SelectLlsType {}

        Item {
            ColumnLayout {
                anchors.fill: parent
                Pane {
                    implicitWidth: parent.width
                    Material.elevation: 6
                    Layout.alignment: Qt.AlignLeft
                    implicitHeight: 50
                    background: Rectangle {
                        width: parent.width
                        color: "#406D9E"
                    }
                    RowLayout {
                        implicitWidth: parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        TabBar {
                            Material.accent: "white"
                            id: tabBar
                            implicitWidth: (qc1Rectangle.width - toolBarButtonControl.width > 500) ? 500 : qc1Rectangle.width - toolBarButtonControl.width
                            background: Rectangle {
                                width: tabBar.width
                                color: "#406D9F"
                            }
                            TabButton {
                                text: qsTr("Stend"); font.pointSize: 8
                                icon.source: "qrc:/svg/resources/fonts/svgs/solid/satellite-dish.svg"
                                icon.width: 10; icon.height: 10; height: 40
                                implicitWidth: 200
                                onClicked: { qchSwipeView.currentIndex = 0 }
                                contentItem: Delegates.TabButtonDelegate {}
                            }
                            TabButton {
                                text: qsTr("DataBase"); font.pointSize: 8
                                icon.source: "qrc:/svg/resources/fonts/svgs/solid/tty.svg"
                                icon.width: 10; icon.height: 10; height: 40
                                onClicked: { qchSwipeView.currentIndex = 1 }
                                contentItem: Delegates.TabButtonDelegate {}
                            }
                            TabButton {
                                text: qsTr("Testing"); font.pointSize: 8
                                icon.source: "qrc:/svg/resources/fonts/svgs/solid/vial.svg"
                                icon.width: 10; icon.height: 10; height: 40
                                onClicked: { qchSwipeView.currentIndex = 2 }
                                contentItem: Delegates.TabButtonDelegate {}
                            }
                            TabButton {
                                text: qsTr("Scaner"); font.pointSize: 8
                                icon.source: "qrc:/svg/resources/fonts/svgs/solid/qrcode.svg"
                                icon.width: 10; icon.height: 10; height: 40
                                onClicked: { qchSwipeView.currentIndex = 4 }
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
                                                onClicked: { qchSwipeView.currentIndex = 5 }
                                            }
                                        }
                                        MenuItem { text: qsTr("About")
                                            icon.source: "qrc:/svg/resources/fonts/svgs/solid/address-card.svg"
                                            icon.width: 14; icon.height: 14
                                            font.pointSize: 8;
                                            onClicked: {
                                                onClicked: { qchSwipeView.currentIndex = 6 }
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
                        StackLayout { id: qchSwipeView
                            Layout.fillHeight: true
                            Layout.fillWidth: true

                            //-- firmware panel
                            StendOtherItems.Qch2Panel {}

                            //-- dataBase
                            StendOtherItems.StendDataBaseSettings {}

                            //-- test settings
                            StendOtherItems.StendTestSettings {}

                            //-- find stend
                            StendOtherItems.FindStend {}

                            //-- scaner settings
                            StendOtherItems.StendScanerSettings {}

                            //-- language
                            StendOtherItems.Language {}

                            //-- about
                            StendOtherItems.About {}
                        }

                        QmlCustomize.StatusPannel {
                            id:statusPannel
                            onRequestFindStends: {
                                busyIndicator.visible = true;
                                qchSwipeView.currentIndex = 3
                            }
                            onConnectActivated: {
                                busyIndicator.visible = true;
                            }
                        }
                    }
                }
            }
        }
    }
}
