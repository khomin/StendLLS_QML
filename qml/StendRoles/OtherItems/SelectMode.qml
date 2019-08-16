import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import "qrc:/qml" as QmlCustomize
import "qrc:/qml/delegates/" as Delegates
import "qrc:/qml/toast/" as Toast
import "qrc:/qml/StendRoles/" as StendRoles
import "qrc:/qml/StendRoles/OtherItems" as StendOtherItems

Item {
    ColumnLayout {
        id:rootColumnLayout
        anchors.fill: parent

        Connections {
            target: viewControl
            onSignalDataBaseError: {
                if(viewControl.stendRole == "undefined") {
                    toast.displayMessage(err, "bad")
                }
            }
            onGoodMessage: {
                if(viewControl.stendRole == "undefined") {
                    toast.displayMessage(text, "good")
                }
            }
            onBadMessage: {
                if(viewControl.stendRole == "undefined") {
                    toast.displayMessage(text, "bad")
                }
            }
        }

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
                width: rootColumnLayout.width - 30
                height: 50
                anchors.verticalCenter: parent.verticalCenter
                TabBar {
                    Material.accent: "white"
                    id: tabBar
                    width: (rootColumnLayout.width - toolBarButtonControl.width > 400) ? 400 : rootColumnLayout.width - toolBarButtonControl.width
                    height: 50
                    background: Rectangle {
                        width: tabBar.width
                        color: "#406D9F"
                    }
                    TabButton {
                        text: qsTr("Stend"); font.pointSize: 8
                        icon.source: "qrc:/svg/resources/fonts/svgs/solid/satellite-dish.svg"
                        icon.width: 10; icon.height: 10; height: 40
                        implicitWidth: 200
                        onClicked: { qchSwipeView.setCurrentIndex(0) }
                        contentItem: Delegates.TabButtonDelegate {}
                    }
                    TabButton {
                        text: qsTr("DataBase"); font.pointSize: 8
                        icon.source: "qrc:/svg/resources/fonts/svgs/solid/tty.svg"
                        icon.width: 10; icon.height: 10; height: 40
                        onClicked: { qchSwipeView.setCurrentIndex(1) }
                        contentItem: Delegates.TabButtonDelegate {}
                    }
                }
                ToolBar {
                    id:toolBarButtonControl
//                    anchors.right: parent.right
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
                                        onClicked: { qchSwipeView.setCurrentIndex(2) }
                                    }
                                }
                                MenuItem { text: qsTr("About")
                                    icon.source: "qrc:/svg/resources/fonts/svgs/solid/address-card.svg"
                                    icon.width: 14; icon.height: 14
                                    font.pointSize: 8;
                                    onClicked: {
                                        onClicked: { qchSwipeView.setCurrentIndex(3) }
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

                    Item {
                        RowLayout {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            Button {
                                implicitWidth: 200; implicitHeight: 200
                                text: qsTr("Quality check 1"); font.pointSize: 12
                                icon.source: "qrc:/svg/resources/fonts/svgs/solid/tasks.svg"
                                onClicked: {
                                    viewControl.stendRole = "qch1";
                                    rootSwipeView.currentIndex = 1
                                }
                            }
                            Button {
                                implicitWidth: 200; implicitHeight: 200
                                text: qsTr("quality check 2"); font.pointSize: 12
                                icon.source: "qrc:/svg/resources/fonts/svgs/solid/shopping-cart.svg"
                                onClicked: {
                                    viewControl.stendRole = "qch2";
                                    rootSwipeView.currentIndex = 2
                                }
                            }
                            Button {
                                implicitWidth: 200; implicitHeight: 200
                                text: qsTr("Firmware"); font.pointSize: 12
                                icon.source: "qrc:/svg/resources/fonts/svgs/solid/microchip.svg"
                                onClicked: {
                                    viewControl.stendRole = "firmware";
                                    rootSwipeView.currentIndex = 3
                                }
                            }
                        }
                    }

                    //-- dataBase
                    StendOtherItems.StendDataBaseSettings {}

                    //-- language
                    StendOtherItems.Language {}

                    //-- about
                    StendOtherItems.About {}
                }
            }
        }
    }
}
