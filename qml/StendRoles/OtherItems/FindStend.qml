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
    id:firmwareFindStendItem
    //-- find stend panel
    Pane {
        id: findStendsPanel
        Material.elevation: 2
        implicitWidth: parent.width - 50
        implicitHeight: parent.height - 50
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        Layout.alignment: Qt.AlignTop | Qt.AlignLeft
        ColumnLayout {
            width: parent.width
            spacing: 30
            Label{ text: qsTr("Find stend panel"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}

            RowLayout {
                spacing: 15
                Label {text: qsTr("Active stend:")}
                Label {id:stendFindActiveLabel; text: Settings.activeStend; }
            }
            RowLayout {
                spacing: 15
                Label { text: qsTr("Total found:")}
                Label { id:stendFindTotalLabel; text: stendFindListView.count  }
                Button { id:stendRestartFindButton;
                    Material.background: Material.Green;
                    Material.foreground: "white";
                    text: qsTr("Restart search");
                    font.pointSize: 8
                    implicitHeight: 50;
                    implicitWidth: 150
                    Layout.alignment: Qt.AlignTop
                    onClicked: {
                        stendFind.startfind()
                        busyIndicator.visible = true;
                    }
                }
            }
            RowLayout {
                ListView { id:stendFindListView
                    width: 400
                    height: 500
                    clip: true
                    model: stendFindModel
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
                                text: model.ip
                                color: "black"
                                font.bold: false
                                font.pointSize: 9
                                readOnly: true
                                Layout.alignment: Qt.AlignVCenter
                            }
                            Button {
                                Material.background: "#406d9e";
                                Material.foreground: "white";
                                text: qsTr("Add to active");
                                font.pointSize: 8
                                implicitHeight: 50;
                                implicitWidth: 150
                                Layout.alignment: Qt.AlignRight
                                onClicked: {
                                    Settings.activeStend = model.ip
                                }
                            }
                        }
                    }
                    ScrollIndicator.horizontal: ScrollIndicator {}
                    ScrollIndicator.vertical: ScrollIndicator {}
                }
            }
        }
    }
}
