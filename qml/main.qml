import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "qrc:/qml" as QmlCustomize
import "qrc:/qml/delegates/" as Delegates
import "qrc:/qml/toast/" as Toast
import "qrc:/qml/StendRoles/" as StendRoles

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
        translate.qmlReady(); // applocation.showMaximized()
    }

    FontLoader { id: fontAwesome; source: "qrc:/font/resources/fonts/awesome_900.ttf" }

    SwipeView {
        id:rootSwipeView
        anchors.fill: parent
        currentIndex: 0//3
        interactive: false
        Item{
            RowLayout {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                Button {
                    implicitWidth: 200; implicitHeight: 200
                    text: qsTr("QCH1"); font.pointSize: 22
                    icon.source: "qrc:/svg/resources/fonts/svgs/solid/tasks.svg"
                    onClicked: {
                        rootSwipeView.currentIndex = 1
                    }
                }
                Button {
                    implicitWidth: 200; implicitHeight: 200
                    text: qsTr("QCH2"); font.pointSize: 22
                    icon.source: "qrc:/svg/resources/fonts/svgs/solid/shopping-cart.svg"
                    onClicked: {
                        rootSwipeView.currentIndex = 2
                    }
                }
                Button {
                    implicitWidth: 200; implicitHeight: 200
                    text: qsTr("Firmware"); font.pointSize: 22
                    icon.source: "qrc:/svg/resources/fonts/svgs/solid/microchip.svg"
                    onClicked: {
                        rootSwipeView.currentIndex = 3
                    }
                }
            }
        }

        StendRoles.Qch1 {

        }
        StendRoles.Qch2 {

        }
        StendRoles.Firmware {

        }
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
