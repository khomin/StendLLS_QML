import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.12

Dialog {
    id:validatePassDialog
    visible: false
    title: qsTr("Please enter password")
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    signal signalExit();
    signal signalReady();

    function getPassword() {
        return passwordTextField.text;
    }

    function clearFilds() {
        passwordTextField.text = ""
    }

    Component.onCompleted: {
        passwordTextField.focus = true
    }

    contentItem: Rectangle {
        color: "transparent"
        implicitWidth: validatePassColumn.width
        implicitHeight: validatePassColumn.height
        ColumnLayout {
            id:validatePassColumn
            TextField {
                id:passwordTextField
                maximumLength: 6
                echoMode: TextInput.Password
                validator: RegExpValidator { regExp: /[\w]{1,6}/ }
                placeholderText: qsTr("Password")
                onAccepted: {
                    signalReady()
                }
            }
            RowLayout {
                Button {
                    implicitWidth: 80
                    background: Rectangle {
                        radius: 3
                        color: "#f2f2f2"
                    }
                    text: qsTr("Close");
                    onClicked: {
                        signalExit();
                        close();
                    }
                }
                Button {
                    id:acceptButton
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                    implicitWidth: 80
                    background: Rectangle {
                        radius: 3
                        color: Material.color(Material.Green)
                    }
                    text: qsTr("Ok");
                    onClicked: {
                        signalReady()
                    }
                }
            }
        }
    }
}
