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

                    Label{ text: qsTr("Current port"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}

                    RowLayout {
                        Label{ text: qsTr("Port"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                        ComboBox {
                            id:serialPortComBox
                            implicitHeight: 35
                            font.pointSize: 8
//                            enabled: false
                            delegate: Delegates.ComboxDelegateTypical{}
                            model: {
                                var result = [];
                                var json = JSON.parse(viewControl.getScanersList());
                                for(var i in json.portName)
                                    result.push(json.portName[i]);
                                return result;
                            }
                        }
                        Button { Material.background: Material.Grey; Material.foreground: "white"; font.pointSize: 8; text: qsTr("Make active");
                            onClicked: {
                                if(viewControl.isScanerConnected()) {
                                    viewControl.closeScanerConnection()
                                }
                                Settings.scanerPort = serialPortComBox.currentText

                                var params = []
                                if(!viewControl.addScanerConnection(Settings.scanerPort, JSON.stringify(params))) {

                                }
                            }
                        }
                    }

                    GridLayout {
                        rows: 3
                        columns: 1

                        Label{ text: "Cap 1 min value"; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        TextField {

                        }
                    }


                    Label{ text: qsTr("Available ports"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}

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
                    }
                }
            }
        }
    }
}
