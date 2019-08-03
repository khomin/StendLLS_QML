import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "qrc:/qml" as QmlCustomize
import "qrc:/qml/delegates/" as Delegates
import "qrc:/qml/toast/" as Toast
import Settings 1.0

//-- about
Item {
    id:aboutWidget
    ScrollView {
        anchors.fill: parent
        //-- about
        ColumnLayout {
            spacing: 5
            anchors.fill: parent
            Pane {
                Material.elevation: 2
                Layout.leftMargin: 10
                implicitWidth: aboutWidget.parent.width - 15
                Column {
                    spacing: 30

                    Label{ text: qsTr("DataBase"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}

                    GridLayout {
                        rows: 3
                        columns: 2

                        Label{ text: "Database name"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {
                            placeholderText: qsTr("Enter value"); implicitWidth: 200; font.pointSize: 8;
                            validator: RegExpValidator { regExp: /^[\w]{1,128}$/ }
                            color: acceptableInput ? "black" : "red"
                            text: Settings.databaseName
                            onEditingFinished: {
                                Settings.databaseName = text;
                            }
                            onAccepted: {
                                Settings.databaseName = text;
                            }
                        }

                        Label{ text: "Host"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {
                            placeholderText: qsTr("Enter value"); implicitWidth: 200; font.pointSize: 8;
                             validator: RegExpValidator { regExp: /^(?=.*[^\.]$)((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.?){4}$/ }
                            color: acceptableInput ? "black" : "red"
                            text: Settings.databaseHost
                            onEditingFinished: {
                                Settings.databaseName = text;
                            }
                            onAccepted: {
                                Settings.databaseName = text;
                            }
                        }

                        Label{ text: "User"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {
                            placeholderText: qsTr("Enter value"); implicitWidth: 200; font.pointSize: 8;
                            validator: RegExpValidator { regExp: /^[\w]{1,128}$/ }
                            color: acceptableInput ? "black" : "red"
                            text: Settings.databaseUser
                            onEditingFinished: {
                                Settings.databaseUser = text;
                            }
                            onAccepted: {
                                Settings.databaseUser = text;
                            }
                        }

                        Label{ text: "Password"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {
                            placeholderText: qsTr("Enter value"); implicitWidth: 200; font.pointSize: 8;
                            validator: RegExpValidator { regExp: /^.{1,128}$/ }
                            color: acceptableInput ? "black" : "red"
                            text: Settings.databasePassword
                            onEditingFinished: {
                                Settings.databasePassword = text;
                            }
                            onAccepted: {
                                Settings.databasePassword = text;
                            }
                        }

                        Label{ text: "Port"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {
                            placeholderText: qsTr("Enter value"); implicitWidth: 200; font.pointSize: 8;
                            validator: RegExpValidator { regExp: /^[\d]{1,10}$/ }
                            color: acceptableInput ? "black" : "red"
                            text: Settings.databasePort
                            onEditingFinished: {
                                Settings.databasePort = text;
                            }
                            onAccepted: {
                                Settings.databasePort = text;
                            }
                        }

                        Button {
                            Material.background: Material.Green;
                            Material.foreground: "white";
                            text: qsTr("Test connect");
                            font.pointSize: 8
                            implicitHeight: 50;
                            implicitWidth: 150
                            focus: true
                            onClicked: {
                                if(viewControl.testDatabaseConnect()) {
                                    toast.displayMessage(qsTr("Database сonnection established"), "good")
                                } else {
                                    toast.displayMessage(qsTr("Database сonnection not established"), "bad")
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
