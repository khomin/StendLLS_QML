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

                    RowLayout {
                        Label{ text: qsTr("Current port"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                        Label{ id:currentPort; text: qrScanerInterface.interfaceName.length !== 0 ? qrScanerInterface.interfaceName : "NA";
                            color: qrScanerInterface.interfaceName.length !== 0 ? "black" : "red";
                            font.pointSize: 8
                        }
                    }

                    RowLayout {
                        Label{ text: qsTr("Port"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                        ComboBox {
                            id:serialPortComBox
                            implicitHeight: 35
                            font.pointSize: 8
                            delegate: Delegates.ComboxDelegateTypical{}
                            model: {
                                var result = [];
                                var json = JSON.parse(qrScanerInterface.getListConnections());
                                for(var i in json.portName)
                                    result.push(json.portName[i]);
                                return result;
                            }
                        }
                        Button { Material.background: Material.Grey; Material.foreground: "white"; font.pointSize: 8; text: qsTr("Refresh");
                            onClicked: {
                                var result = [];
                                var json = JSON.parse(qrScanerInterface.getListConnections());
                                for(var i in json.portName)
                                    result.push(json.portName[i]);
                                serialPortComBox.model = result;
                            }
                        }
                        Button { Material.background: Material.Green; Material.foreground: "white";
                            font.pointSize: 8; text: qsTr("Make active");
                            enabled: (serialPortComBox.currentText != currentPort.text) && (serialPortComBox.currentText.length !== 0)
                            onClicked: {
                                if(stendInterface.isOpened()) {
                                    stendInterface.close()
                                }
                                Settings.scanerPort = serialPortComBox.currentText

                                var params = {}
                                params.baudrate = 115200;
                                console.log(JSON.stringify(params));
                                qrScanerInterface.addConnection(Settings.scanerPort, JSON.stringify(params));
                            }
                        }
                    }
                }
            }
        }
    }
}
