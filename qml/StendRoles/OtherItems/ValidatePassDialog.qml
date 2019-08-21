import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.12
import DataBase 1.0

Dialog {
    id:validatePassDialog
    visible: false
    title: qsTr("Enter login/password")
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    property string userPermission: ""

    signal signalExit();
    signal signalReady();

    function getUserPermission() {
        return userPermission;
    }

    onFocusChanged: {
        clear()
    }

    Component.onCompleted: {
        loginTextField.focus = true
    }

    function clear() {
        loginTextField.text = ""
        passwordTextField.text = ""
    }

    Connections {
        target: DataBase
        onUserPermissionReadyRead: {
            userPermission = permission
            signalReady()
        }
    }

    contentItem: Rectangle {
        color: "transparent"
        implicitWidth: validatePassColumn.width
        implicitHeight: validatePassColumn.height
        ColumnLayout {
            id:validatePassColumn
            TextField {
                id:loginTextField
                maximumLength: 128
                validator: RegExpValidator { regExp: /[\w]{1,128}/ }
                placeholderText: qsTr("Login")
                implicitWidth: parent.width
                onAccepted: {
                    if(passwordTextField.length !== 0) {
                        DataBase.checkUserPermission(loginTextField.text, passwordTextField.text)
                    }
                }
            }
            TextField {
                id:passwordTextField
                maximumLength: 128
                echoMode: TextInput.Password
                validator: RegExpValidator { regExp: /[\w]{1,128}/ }
                placeholderText: qsTr("Password")
                implicitWidth: parent.width
                onAccepted: {
                    if(loginTextField.length !== 0) {
                        DataBase.checkUserPermission(loginTextField.text, passwordTextField.text)
                    }
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
                        DataBase.checkUserPermission(loginTextField.text, passwordTextField.text)
                    }
                }
            }
        }
    }
}
