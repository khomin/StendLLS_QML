import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "qrc:/qml" as QmlCustomize
import "qrc:/qml/delegates/" as Delegates
import "qrc:/qml/toast/" as Toast
import Settings 1.0

Item {
    id:testSettingsWidget
    ScrollView {
        anchors.fill: parent
        ColumnLayout {
            spacing: 5
            anchors.fill: parent
            Pane {
                Material.elevation: 2
                Layout.leftMargin: 10
                implicitWidth: testSettingsWidget.parent.width - 15
                Column {
                    spacing: 30

                    Label{ text: qsTr("Test settings"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}

                    GridLayout {
                        rows: 3
                        columns: 1

                        Label{ text: "Cap 1 min value"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {
                            placeholderText: qsTr("Enter value"); implicitWidth: 200; font.pointSize: 8;
                            validator: RegExpValidator { regExp: /^[\d]{1,128}$/ }
                            color: acceptableInput ? "black" : "red"
                            text: Settings.cap1Min
                            onEditingFinished: {
                                Settings.cap1Min = text;
                            }
                            onAccepted: {
                                Settings.cap1Min = text;
                            }
                        }

                        Label{ text: "Cap 2 min value"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {
                            placeholderText: qsTr("Enter value"); implicitWidth: 200; font.pointSize: 8;
                            validator: RegExpValidator { regExp: /^[\d]{1,128}$/ }
                            color: acceptableInput ? "black" : "red"
                            text: Settings.cap2Min
                            onEditingFinished: {
                                Settings.cap2Min = text;
                            }
                            onAccepted: {
                                Settings.cap2Min = text;
                            }
                        }

                        Label{ text: "Cap 3 min value"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {
                            placeholderText: qsTr("Enter value"); implicitWidth: 200; font.pointSize: 8;
                            validator: RegExpValidator { regExp: /^[\d]{1,128}$/ }
                            color: acceptableInput ? "black" : "red"
                            text: Settings.cap3Min
                            onEditingFinished: {
                                Settings.cap3Min = text;
                            }
                            onAccepted: {
                                Settings.cap3Min = text;
                            }
                        }

                        Label{ text: "Cap 1 max value"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {
                            placeholderText: qsTr("Enter value"); implicitWidth: 200; font.pointSize: 8;
                            validator: RegExpValidator { regExp: /^[\d]{1,128}$/ }
                            color: acceptableInput ? "black" : "red"
                            text: Settings.cap1Max
                            onEditingFinished: {
                                Settings.cap1Max = text;
                            }
                            onAccepted: {
                                Settings.cap1Max = text;
                            }
                        }

                        Label{ text: "Cap 2 max value"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {
                            placeholderText: qsTr("Enter value"); implicitWidth: 200; font.pointSize: 8;
                            validator: RegExpValidator { regExp: /^[\d]{1,128}$/ }
                            color: acceptableInput ? "black" : "red"
                            text: Settings.cap2Max
                            onEditingFinished: {
                                Settings.cap2Max = text;
                            }
                            onAccepted: {
                                Settings.cap2Max = text;
                            }
                        }

                        Label{ text: "Cap 3 max value"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {
                            placeholderText: qsTr("Enter value"); implicitWidth: 200; font.pointSize: 8;
                            validator: RegExpValidator { regExp: /^[\d]{1,128}$/ }
                            color: acceptableInput ? "black" : "red"
                            text: Settings.cap3Max
                            onEditingFinished: {
                                Settings.cap3Max = text;
                            }
                            onAccepted: {
                                Settings.cap3Max = text;
                            }
                        }
                    }
                }
            }
        }
    }
}
