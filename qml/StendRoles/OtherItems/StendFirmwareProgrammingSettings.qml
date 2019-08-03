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
    id:settingsWidget
    ScrollView {
        anchors.fill: parent
        //-- settings
        ColumnLayout {
            spacing: 5
            anchors.fill: parent
            Pane {
                Material.elevation: 2
                Layout.leftMargin: 10
                implicitWidth: settingsWidget.parent.width - 15
                Column {
                    spacing: 30
                    Label{ text: qsTr("Firmware programming settings"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                    GridLayout {
                        rows: 1
                        columns: 2

                        Label{ text: "Path st-link CLI.exe"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {
                            placeholderText: qsTr("Enter value"); implicitWidth: 500; font.pointSize: 8;
                            validator: RegExpValidator { regExp: /^.{1,128}$/ }
                            color: acceptableInput ? "black" : "red"
                            text: Settings.fimrwareStLinkPath
                            onEditingFinished: {
                                Settings.fimrwareStLinkPath = text;
                            }
                            onAccepted: {
                                Settings.fimrwareStLinkPath = text;
                            }
                        }

                        Label{ text: "Path lls.bin"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {
                            placeholderText: qsTr("Enter value"); implicitWidth: 500; font.pointSize: 8;
                            validator: RegExpValidator { regExp: /^.{1,128}$/ }
                            color: acceptableInput ? "black" : "red"
                            text: Settings.firmwareDutPath
                            onEditingFinished: {
                                Settings.firmwareDutPath = text;
                            }
                            onAccepted: {
                                Settings.firmwareDutPath = text;
                            }
                        }

                        Label{ text: "Path lls-boot.bin"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {
                            placeholderText: qsTr("Enter value"); implicitWidth: 500; font.pointSize: 8;
                            validator: RegExpValidator { regExp: /^.{1,128}$/ }
                            color: acceptableInput ? "black" : "red"
                            text: Settings.frmwareBootPath
                            onEditingFinished: {
                                Settings.frmwareBootPath = text;
                            }
                            onAccepted: {
                                Settings.frmwareBootPath = text;
                            }
                        }
                    }
                }
            }
        }
    }
}
