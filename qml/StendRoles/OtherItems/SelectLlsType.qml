import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12


Item {
    ColumnLayout {
        anchors.fill: parent
        Pane {
            id:rootPanel
            Material.elevation: 6
            Layout.leftMargin: 10
            implicitWidth: parent.parent.width - 20
            implicitHeight: parent.parent.height - 20
            Layout.alignment: Qt.AlignTop

            ScrollView {
                anchors.fill: parent
                clip: true
                Column {
                    Label{ text: qsTr("Select LLS type"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                    spacing: 15
                    Row {
                        spacing: 10
                        ListView {
                            width: rootPanel.width / 2 - 100
                            height: rootPanel.height
                            clip: true
                            model: llsTypeModel
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
                                        text: model.text
                                        color: "black"
                                        font.bold: false
                                        font.pointSize: 9
                                        readOnly: true
                                        Layout.alignment: Qt.AlignVCenter
                                    }
                                    Button {
                                        Material.background: "#406d9e";
                                        Material.foreground: "white";
                                        text: qsTr("Select");
                                        font.pointSize: 8
                                        implicitHeight: 50;
                                        implicitWidth: 150
                                        Layout.alignment: Qt.AlignRight
                                        onClicked: {
                                            stackLayout.currentIndex = 1;
                                            selectLlsTestType.makeSelectedAsActive(index)
                                        }
                                    }
                                }
                            }
                            ScrollIndicator.horizontal: ScrollIndicator {}
                            ScrollIndicator.vertical: ScrollIndicator {}
                        }

                        ColumnLayout {
                            anchors.verticalCenter: parent.verticalCenter
                            width: rootPanel.width / 2
                            spacing: 0
                            TextEdit {
                                width: rootPanel.width / 2
                                readOnly: true
                                Layout.alignment: Qt.AlignVCenter
                                wrapMode: TextEdit.Wrap
                                text: qsTr("At the verification stage, when the device does not yet have a serial number,\nthe program does not know what type of device the device belongs to.\nTherefore, please select a type from the available\nThen the program will be able to load the \"minimum-maximum\"\nparameters for tests that are suitable specifically for this type of device")
                            }
                            Button {
                                Material.background: Material.Green;
                                Material.foreground: "white";
                                text: qsTr("Refresh");
                                font.pointSize: 8
                                implicitHeight: 45;
                                focus: false
                                onClicked: {
                                    selectLlsTestType.researchLlsTypes()
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        selectLlsTestType.researchLlsTypes()
    }
}
