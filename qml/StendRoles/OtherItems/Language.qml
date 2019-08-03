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
    id:settingsLanguage
    ScrollView {
        anchors.fill: parent
        //-- settings
        ColumnLayout {
            spacing: 5
            anchors.fill: parent
            Pane {
                Material.elevation: 2
                Layout.leftMargin: 10
                implicitWidth: settingsLanguage.parent.width - 15
                Column {
                    spacing: 30
                    Label{ text: qsTr("Settings"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                    GridLayout {
                        rows: 1
                        columns: 2
                        Label { text: qsTr("Language"); font.pointSize: 8 }
                        //-- languages
                        ComboBox {
                            id:languagesComBox
                            implicitHeight: 35
                            font.pointSize: 8
                            model: translate.getLanguageList()
                            delegate: Delegates.ComboxDelegateTypical{}
                            Connections {
                                target: translate
                                onSignalLanguageChanged: {
                                    languagesComBox.currentIndex = langIndex;
                                }
                            }
                        }
                    }
                    Button { Material.background: Material.Grey; Material.foreground: "white"; font.pointSize: 8; text: qsTr("Apply");
                        onClicked: {
                            switch(languagesComBox.currentIndex) {
                            case 0: translate.selectLanguage("en"); break;
                            case 1: translate.selectLanguage("ru"); break;
                            case 2: translate.selectLanguage("cn"); break;
                            }
                        }
                    }
                }
            }
        }
    }
}
