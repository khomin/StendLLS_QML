import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "qrc:/qml" as QmlCustomize
import "qrc:/qml/delegates/" as Delegates
import "qrc:/qml/toast/" as Toast
import "qrc:/qml/StendRoles/" as StendRoles
import "qrc:/qml/StendRoles/OtherItems" as StendOtherItems

ApplicationWindow {
    id:applocation
    visible: true

    property string softwareName: qsTr("Locus configure ")
    property string companiName: qsTr("«Locus wireless solutions»")
    property string softwareVersion: qsTr("Version 0.9.0")

    title: softwareName

    width: 900
    height: 700

    Material.theme: Material.Light
    Material.accent: Material.LightBlue

    property int propertyCentralSwipeLastIndex: 0

    Component.onCompleted: {
        translate.qmlReady();
        applocation.showMaximized()
    }

    StackLayout {
        id:rootSwipeView
        anchors.fill: parent
        currentIndex: 0

        StendOtherItems.SelectMode {}
        StendRoles.Qch1 {}
        StendRoles.Qch2 {}
        StendRoles.Firmware {}
    }

    BusyIndicator {
        id:busyIndicator
        width: 150
        height: 150
        visible: false
        clip: true
        anchors.centerIn: parent
    }

    Toast.Toast {
        id: toast
    }
}
