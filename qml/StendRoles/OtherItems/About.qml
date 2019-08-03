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

                    Label{ text: qsTr("About"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}

                    GridLayout {
                        rows: 3
                        columns: 1
                        Label{ text: softwareName; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        Label{ text: companiName; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                        Label{ text: softwareVersion; font.pointSize: 8; color: Material.color(Material.Grey, Material.Shade800)}
                    }
                }
            }
        }
    }
}
