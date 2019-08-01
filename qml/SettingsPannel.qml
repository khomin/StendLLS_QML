import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.2 as QtQuickOld
import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

import "qrc:/qml/delegates/" as Delegates

SwipeView {
    id:settingsPannelSwipeView
    spacing: 10
    Layout.fillWidth: true
    Layout.fillHeight: true
    interactive: false

    signal requestRebootTracker();

    signal error();
    signal settingIsRead()
    signal settingIsWrited()
    signal trackerIsRebooted();

    Connections {
        target: viewControl
        onSignalSettingsIsReaded: {
            var json = JSON.parse(settingsJson)
            // communication
            communicationApnTextFild.text = json.Apn;
            communicationApnUserNameTextField.text = json.ApnUserName;
            communicationApnPasswordTextField.text = json.ApnPassword;
            communicationTargetPortTexField.text = json.Port;
            communicationTargetAddressTextField.text = json.Domain;
            communicationGsmBandComBox.currentIndex = json.Band;
            communicationServerResponseTimeoutSlider.value = parseInt(json.ServerResponseTime)
            communicationActiveDataLinkSlider.value = parseInt(json.sendTime)
            communicationProtocolTypeComBox.currentIndex = parseInt(json.SocketType);
            communicationSimPinTextField.text = json.PIN;

            // system parameters
            acquistionIgnitionCheckBox.checked = parseInt(json.Ignition);
            whenMovingMinimumTimeSlider.value = parseInt(json.MinTime);
            whenMovingDistanceSlider.value = parseInt(json.MinDistance);
            whenMovingMinimumVeerAngleSlider.value = parseInt(json.MinVeerAngle);
            whenStopedMinimumTimeSlider.value  = 30;
            sleepStopDefinationComBox.currentIndex = parseInt(json.Sleeptype)
            sleepDelaySlider.value = parseInt(json.EnterSleepDelaytime)
            speedAlarmsComBox.currentIndex = parseInt(json.speedAlarmEn)
            speedAlarmSlider.value = parseInt(json.speedAlarmValue)
            temp1AlarmsComBox.currentIndex = parseInt(json.Temperatrue0_En)
            tempAlarm1MinValSlider.value = parseInt(json.Temperatrue0_TL)
            tempAlarm1MaxValSlider.value = parseInt(json.Temperatrue0_TH)
            temp2AlarmsComBox.currentIndex = parseInt(json.Temperatrue1_En)
            tempAlarm2MinValSlider.value = parseInt(json.Temperatrue1_TL)
            tempAlarm2MaxValSlider.value = parseInt(json.Temperatrue1_HL)

            // authorized
            authorizedAutoAnswerComBox.currentIndex = parseInt(json.phoneStatus)    // TODO: check it
            authorizedPhone1.text = json.sosNum0.lenght !== 0 ? json.sosNum0 : ""
            authorizedPhone2.text = json.sosNum1.lenght !== 0 ? json.sosNum1 : ""
            authorizedPhone3.text = json.sosNum2.lenght !== 0 ? json.sosNum2 : ""
            // TODO: table onewire id
            //--
            //

            // digital input
            digitalIn1ComBox.currentIndex = parseInt(json.Intput1_En)
            digitalIn2ComBox.currentIndex = parseInt(json.Intput2_En)
            digitalIn3ComBox.currentIndex = parseInt(json.Intput3_En)
            digitalIn4ComBox.currentIndex = parseInt(json.Intput4_En)
            digitalIn5ComBox.currentIndex = parseInt(json.Intput5_En)
            digitalOut1ComBox.currentIndex = parseInt(json.OUT1_En)
            digitalOut2ComBox.currentIndex = parseInt(json.OUT2_En)
            digitalOut3ComBox.currentIndex = parseInt(json.OUT3_En)
            digitalOut4ComBox.currentIndex = parseInt(json.OUT4_En)

            // analog input
            //---
            //
            // OBD
            obdDataTimeSlider.value = parseFloat(json.OBDTime)
            obdMaxAccelerationSlider.value = 30;//parseFloat(json.MaxAcceleration)
            obdMaxBreakingSlider.value = 30;//parseFloat(json.MaxBreaking)

            // geofence
            longitudeTextField.text = json.GeoLong.lenght !== 0 ? parseFloat(json.GeoLong) : ""
            latitudeTextField.text = json.GeoLat.lenght !== 0 ? parseFloat(json.GeoLat) : ""
            geofenceRadiusSlider.value = parseInt(json.geoScope);

            // sms alarm
            smsAlarmGpsAntennaCutComBox.currentIndex = parseInt(json.OUT4_En)
            smsAlarmExternalPowerCutComBox.currentIndex = parseInt(json.OUT4_En)
            smsAlarmTowComBox.currentIndex = parseInt(json.OUT4_En)
            smsAlarmIn5SosComBox.currentIndex = parseInt(json.OUT4_En)
            smsAlarmGeoInComBox.currentIndex = parseInt(json.OUT4_En)
            smsAlarmGeoOutComBox.currentIndex = parseInt(json.OUT4_En)
            smsAlarmIn1OnComBox.currentIndex = parseInt(json.OUT4_En)
            smsAlarmIn1OffComBox.currentIndex = parseInt(json.OUT4_En)
            smsAlarmIn2OnComBox.currentIndex = parseInt(json.OUT4_En)
            smsAlarmIn2OffComBox.currentIndex = parseInt(json.OUT4_En)
            smsAlarmIn3OnComBox.currentIndex = parseInt(json.OUT4_En)
            smsAlarmIn3OffComBox.currentIndex = parseInt(json.OUT4_En)
            smsAlarmIn4ONComBox.currentIndex = parseInt(json.OUT4_En)
            smsAlarmIn4OffComBox.currentIndex = parseInt(json.OUT4_En)
            smsAlarmLowBattVoltageComBox.currentIndex = parseInt(json.OUT4_En)

            settingIsRead()
        }
        onSignalSettingsIsWrited: {
            settingIsWrited();
        }
        onSignalSettingsTrackerIsRebooted: {
            trackerIsRebooted();
        }
        onSignalUpdateRealTimeDataForm: {
            var json = JSON.parse(jsonData)
            if(json.type === "longitude" || json.type === "latitude") {
                if(json.latitude !== undefined && json.longitude !== undefined) {
                    marker.coordinate = QtPositioning.coordinate(json.latitude, json.longitude)
                }
            }
        }
        onSignalAppendLog: {
            if(unitTestDisplayLogList.model.count > 256) {
                unitTestDisplayLogList.model.clear()
            }
            if(unitTestDisplayLogShow.checked) {
                unitTestDisplayLogList.model.append({"text": text})
            }
        }
    }

    function getJsonSettings() {
        var json = {}
        // communication
        json.Apn = communicationApnTextFild.text;
        json.ApnUserName = communicationApnUserNameTextField.text;
        json.ApnPassword = communicationApnPasswordTextField.text;
        json.Port = communicationTargetPortTexField.text;
        json.Domain = communicationTargetAddressTextField.text;
        json.Band = communicationGsmBandComBox.currentIndex;
        json.ServerResponseTime = communicationServerResponseTimeoutSlider.value;
        json.sendTime = communicationActiveDataLinkSlider.value;
        json.SocketType = communicationProtocolTypeComBox.currentIndex;
        json.PIN = communicationSimPinTextField.text;

        // system parameters
        json.Ignition = acquistionIgnitionCheckBox.checked;
        json.MinTime = whenMovingMinimumTimeSlider.value;
        json.MinDistance = whenMovingDistanceSlider.value;
        json.MinVeerAngle = whenMovingMinimumVeerAngleSlider.value;
        // todo: json.ApnUserName = whenStopedMinimumTimeSlider.value  = 30;
        json.Sleeptype = sleepStopDefinationComBox.currentIndex;
        json.EnterSleepDelaytime = sleepDelaySlider.value;
        json.speedAlarmEn = speedAlarmsComBox.currentIndex;
        json.speedAlarmValue = speedAlarmSlider.value;
        json.Temperatrue0_En = temp1AlarmsComBox.currentIndex;
        json.Temperatrue0_TL = tempAlarm1MinValSlider.value;
        json.Temperatrue0_TH = tempAlarm1MaxValSlider.value;
        json.Temperatrue1_En = temp2AlarmsComBox.currentIndex;
        json.Temperatrue1_TL = tempAlarm2MinValSlider.value;
        json.Temperatrue1_HL = tempAlarm2MaxValSlider.value;

        // authorized
        json.phoneStatus = authorizedAutoAnswerComBox.currentIndex;
        json.sosNum0 = authorizedPhone1.text
        json.sosNum1 = authorizedPhone2.text
        json.sosNum2 = authorizedPhone3.text

        // TODO: table onewire id
        //--
        //

        // digital input
        json.Intput1_En = digitalIn1ComBox.currentIndex
        json.Intput2_En = digitalIn2ComBox.currentIndex
        json.Intput3_En = digitalIn3ComBox.currentIndex
        json.Intput4_En = digitalIn4ComBox.currentIndex
        json.Intput5_En = digitalIn5ComBox.currentIndex
        json.OUT1_En = digitalOut1ComBox.currentIndex
        json.OUT2_En = digitalOut2ComBox.currentIndex
        json.OUT3_En = digitalOut3ComBox.currentIndex
        json.OUT4_En = digitalOut4ComBox.currentIndex

        // analog input
        //---
        //
        // OBD
        json.OBDTime = obdDataTimeSlider.value
        // todo: obdMaxAccelerationSlider.value = 30;//parseFloat(json.MaxAcceleration)
        // todo: obdMaxBreakingSlider.value = 30;//parseFloat(json.MaxBreaking)

        // geofence
        json.GeoLong = longitudeTextField.text
        json.GeoLat = latitudeTextField.text
        json.geoScope = geofenceRadiusSlider.value

        // sms alarm
        json.OUT4_En = smsAlarmGpsAntennaCutComBox.currentIndex
        json.OUT4_En = smsAlarmExternalPowerCutComBox.currentIndex
        json.OUT4_En = smsAlarmTowComBox.currentIndex
        json.OUT4_En = smsAlarmIn5SosComBox.currentIndex
        json.OUT4_En = smsAlarmGeoInComBox.currentIndex
        json.OUT4_En = smsAlarmGeoOutComBox.currentIndex
        json.OUT4_En = smsAlarmIn1OnComBox.currentIndex
        json.OUT4_En = smsAlarmIn1OffComBox.currentIndex
        json.OUT4_En = smsAlarmIn2OnComBox.currentIndex
        json.OUT4_En = smsAlarmIn2OffComBox.currentIndex
        json.OUT4_En = smsAlarmIn3OnComBox.currentIndex
        json.OUT4_En = smsAlarmIn3OffComBox.currentIndex
        json.OUT4_En = smsAlarmIn4ONComBox.currentIndex
        json.OUT4_En = smsAlarmIn4OffComBox.currentIndex
        json.OUT4_En = smsAlarmLowBattVoltageComBox.currentIndex

        return JSON.stringify(json);
    }

    function setCurrentWidget(widgetName) {
        settingsPannel.currentIndex = 0
        if(widgetName == "settings") {
            while(settingsPannel.currentItem != settingsWidget) {
                settingsPannel.setCurrentIndex(settingsPannel.currentIndex+1)
            }
        }
        if(widgetName == "about") {
            while(settingsPannel.currentItem != aboutWidget) {
                settingsPannel.setCurrentIndex(settingsPannel.currentIndex+1)
            }
        }
        if(widgetName == "unitTestsWidget") {
            while(settingsPannel.currentItem != unitTestsWidget) {
                settingsPannel.setCurrentIndex(settingsPannel.currentIndex+1)
            }
        }
    }

    function mapUpdatePosition(lat, lon) {
        map.center = QtPositioning.coordinate(parseFloat(lat), parseFloat(lon))
        map.circle.center = QtPositioning.coordinate(parseFloat(lat), parseFloat(lon))
    }

    Item {
        id:propertySwipeView
        SwipeView {
            id: propertyPanelSwipe
            currentIndex: tabBar.currentIndex
            spacing: 10
            Layout.fillWidth: true
            Layout.fillHeight: true
            anchors.fill: parent
            interactive: false
            clip: true

            //-- communication
            Item {
                ScrollView {
                    anchors.fill: parent
                    //-- data acquition send
                    ColumnLayout {
                        spacing: 5
                        anchors.fill: parent
                        Pane {
                            Material.elevation: 2
                            Layout.leftMargin: 10
                            implicitWidth: propertyPanelSwipe.width - 15
                            Column {
                                Label{ text: qsTr("Basic setup"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                                GridLayout {
                                    rows: 7
                                    columns: 2
                                    columnSpacing: 10
                                    rowSpacing: -5
                                    Label { text: qsTr("UserID"); font.pointSize: 8 }
                                    //-- user id
                                    TextField { id:communicationUserIdTextField
                                        placeholderText: qsTr("Cannot include the lettters (i)O(o)Q(q)");
                                        implicitWidth: 200; font.pointSize: 8;
                                        validator: RegExpValidator { regExp: /^[^o,^i,^I,^O,Q,q]{1,17}$/ }
                                        color: acceptableInput ? "black" : "red"
                                    }

                                    //-- sim pin
                                    Label { text: qsTr("SIM pin"); font.pointSize: 8 }
                                    //-- sim pin
                                    TextField { id:communicationSimPinTextField
                                        placeholderText: qsTr("4 digits max"); implicitWidth: 200; font.pointSize: 8;
                                        validator: RegExpValidator { regExp: /^[\d]{4}$/ }
                                        color: acceptableInput ? "black" : "red"
                                    }

                                    //-- gsm band
                                    Label { text: qsTr("GSM band"); font.pointSize: 8 }
                                    //-- gsm band
                                    ComboBox {
                                        id:communicationGsmBandComBox
                                        implicitHeight: 35
                                        font.pointSize: 8
                                        model: [ qsTr("900MHz"), qsTr("1800MHz"), qsTr("850MHz"), qsTr("1900MHz") ]
                                        delegate: Delegates.ComboxDelegateTypical{}
                                    }
                                }
                            }
                        }
                        //-- gprs information
                        Pane {
                            Material.elevation: 6
                            Layout.leftMargin: 10
                            implicitWidth: propertyPanelSwipe.width - 15
                            Column {
                                width: parent.width
                                Label{ text: qsTr("GPRS informantion"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                                GridLayout {
                                    rows: 7
                                    columns: 2
                                    columnSpacing: 10
                                    rowSpacing: -5
                                    //-- apn
                                    Label { text: qsTr("APN"); font.pointSize: 8 }
                                    //-- apn
                                    TextField {
                                        id:communicationApnTextFild
                                        placeholderText: qsTr("30 digits"); implicitWidth: 200; font.pointSize: 8;
                                        validator: RegExpValidator { regExp: /^[\w.]{1,30}$/ }
                                        color: acceptableInput ? "black" : "red"
                                    }
                                    //-- user name
                                    Label { text: qsTr("User name"); font.pointSize: 8}
                                    //-- user name
                                    TextField {
                                        id:communicationApnUserNameTextField
                                        placeholderText: qsTr("20 digits maximum"); implicitWidth: 200; font.pointSize: 8;
                                        validator: RegExpValidator { regExp: /^[\w]{1,20}$/ }
                                        color: acceptableInput ? "black" : "red"
                                    }
                                    //-- password
                                    Label { text: qsTr("Password"); font.pointSize: 8 }
                                    //-- password
                                    TextField {
                                        id:communicationApnPasswordTextField
                                        placeholderText: qsTr("20 digits maximum"); implicitWidth: 200; font.pointSize: 8;
                                        validator: RegExpValidator { regExp: /^[\w]{1,20}$/ }
                                        color: acceptableInput ? "black" : "red"
                                    }
                                    //-- domain ip
                                    Label { text: qsTr("Domain/IP"); font.pointSize: 8 }
                                    //-- domain ip
                                    ComboBox {
                                        id:communicationDomainIpComBox
                                        implicitHeight: 35
                                        font.pointSize: 8
                                        model: [ qsTr("IP"), qsTr("Domain") ]
                                        enabled: false
                                        delegate: Delegates.ComboxDelegateTypical{}
                                    }

                                    //-- target address
                                    Label { text: qsTr("Target address"); font.pointSize: 8 }
                                    //-- target address
                                    TextField { id: communicationTargetAddressTextField
                                        placeholderText: qsTr("E.g. 192.168.1.2"); implicitWidth: 200; font.pointSize: 8;
                                        validator: RegExpValidator { regExp: /^(http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)?[a-z0-9]+([\-\.]{1}[a-z0-9]+)*\.[a-z]{2,5}(:[0-9]{1,5})?(\/.*)?|^((http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)?([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$/ }
                                        color: acceptableInput ? "black" : "red"
                                    }
                                    //-- target port
                                    Label { text: qsTr("Target port"); font.pointSize: 8 }
                                    //-- target port
                                    TextField { id: communicationTargetPortTexField
                                        placeholderText: qsTr("5 digits maximum"); implicitWidth: 200; font.pointSize: 8;
                                        validator: RegExpValidator { regExp: /^[\d]{1,5}$/ }
                                        color: acceptableInput ? "black" : "red"
                                    }
                                    //-- protocol
                                    Label { text: qsTr("Protocol"); font.pointSize: 8 }
                                    //-- protocol
                                    ComboBox {
                                        id:communicationProtocolTypeComBox
                                        implicitHeight: 35
                                        font.pointSize: 8
                                        model: [ qsTr("TCP/IP"), qsTr("UDP/IP") ]
                                        delegate: Delegates.ComboxDelegateTypical{}
                                    }

                                    //-- server response timeout
                                    Label { text: qsTr("Server response timeout"); font.pointSize: 8 }
                                    //-- server response timeout
                                    Row {
                                        Slider { id: communicationServerResponseTimeoutSlider; from: 5; value: 60; to: 60; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                        Label {id: communicationServerResponseTimeout; anchors.verticalCenter: parent.verticalCenter;
                                            text: (communicationServerResponseTimeoutSlider.value).toFixed(0); font.pointSize: 8 }
                                        Label {text: " seconds"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                    }

                                    //-- active data link timeout
                                    Label { text: qsTr("Active data link timeout"); font.pointSize: 8 }
                                    //-- active data link timeout
                                    Row {
                                        Slider { id: communicationActiveDataLinkSlider; from: 0; value: 60; to: 999; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                        Label {id: communicationActiveDataLink; anchors.verticalCenter: parent.verticalCenter;
                                            text: ( communicationActiveDataLinkSlider.value).toFixed(0); font.pointSize: 8 }
                                        Label {text: " seconds"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //-- system paramters
            Item {
                ScrollView {
                    width: parent.width
                    height: parent.height
                    //-- data acquition send
                    ColumnLayout {
                        spacing: 5
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            Layout.leftMargin: 10
                            ColumnLayout {
                                width: parent.width
                                Label{ text: qsTr("Reboot"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                Button { id:rebootedButton
                                    text: qsTr("Reboot");
                                    font.pointSize: 8
                                    onClicked: {
                                        requestRebootTracker()
                                    }
                                }
                            }
                        }
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            Layout.leftMargin: 10
                            ColumnLayout {
                                width: parent.width
                                Label{ text: qsTr("Data acquistion/Send definition"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                RowLayout {
                                    spacing: 10
                                    ColumnLayout {
                                        spacing: 25
                                        //-- Data acquistion by movement or ignition
                                        Label { text: qsTr("On/Off"); font.pointSize: 8}
                                    }
                                    ColumnLayout {
                                        spacing: 0
                                        Layout.fillWidth: true
                                        CheckBox {  id: acquistionIgnitionCheckBox }
                                    }
                                }
                            }
                        }
                        Pane {
                            Material.elevation: 6
                            Layout.leftMargin: 10
                            implicitWidth: propertyPanelSwipe.width - 15
                            Column {
                                spacing: 5
                                width: parent.width
                                Label{ text: qsTr("When moving"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                GridLayout {
                                    rows: 3
                                    columns: 2
                                    columnSpacing: 15
                                    rowSpacing: -5
                                    Layout.fillWidth: true

                                    //-- miimum time
                                    Label { text: qsTr("Minimum time"); font.pointSize: 8}
                                    Row {
                                        Slider { id: whenMovingMinimumTimeSlider; from: 1; value: 1; to: 30; snapMode: Slider.SnapAlways; Material.accent: "#406D9F"}
                                        Label {id: dataAcquitionTimeSecond; anchors.verticalCenter: parent.verticalCenter; text: (whenMovingMinimumTimeSlider.value).toFixed(0); Material.accent: "#406D9F"; font.pointSize: 8}
                                        Label {text: " seconds"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                    }

                                    //-- miimum distance
                                    Label { text: qsTr("Minimum distance"); font.pointSize: 8}
                                    Row {
                                        Slider { id: whenMovingDistanceSlider; from: 1; value: 1; to: 1000; snapMode: Slider.SnapAlways; Material.accent: "#406D9F"}
                                        Label {id: dataAcquitionDistance; anchors.verticalCenter: parent.verticalCenter; text: (whenMovingDistanceSlider.value).toFixed(0); font.pointSize: 8; Material.accent: "#406D9F"}
                                        Label {text: " meters"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                    }

                                    //-- miimum time
                                    Label { text: qsTr("Minimum veer angle"); font.pointSize: 8}
                                    Row {
                                        Slider { id: whenMovingMinimumVeerAngleSlider; from: 1; value: 1; to: 30; snapMode: Slider.SnapAlways; Material.accent: "#406D9F"}
                                        Label {id: dataAcquitionAngle; anchors.verticalCenter: parent.verticalCenter; text: (whenMovingMinimumVeerAngleSlider.value).toFixed(0); font.pointSize: 8; Material.accent: "#406D9F"}
                                        Label {text: " degree"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                    }
                                }
                            }
                        }
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            Layout.leftMargin: 10
                            Column {
                                spacing: 10
                                width: parent.width
                                Label{ text: qsTr("When stopped time"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                RowLayout {
                                    spacing: 10
                                    ColumnLayout {
                                        spacing: 10
                                        //-- miimum time
                                        Text { text: qsTr("Minimum time"); font.pointSize: 8 }
                                    }
                                    ColumnLayout {
                                        spacing: -10
                                        Layout.fillWidth: true
                                        Row {
                                            Slider { id: whenStopedMinimumTimeSlider; enabled: false;  from: 1; value: 30; to: 30; snapMode: Slider.SnapAlways; Material.accent: "#406D9F"}
                                            Label {id: stopepedTime; anchors.verticalCenter: parent.verticalCenter; text: (whenStopedMinimumTimeSlider.value).toFixed(0); font.pointSize: 8 }
                                            Label {text: " seconds"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8; }
                                        }
                                    }
                                }
                            }
                        }
                        //-- slip/stop definition
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            Layout.leftMargin: 10
                            Column {
                                spacing: 10
                                width: parent.width
                                Label{ text: qsTr("Sleep stop definition"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                GridLayout {
                                    rows: 2
                                    columns: 2
                                    columnSpacing: 15
                                    rowSpacing: -5
                                    Layout.fillWidth: true
                                    //-- normal sleep
                                    Text { text: qsTr("Normal/Sleep/Deep Sleep"); font.pointSize: 8 }
                                    ComboBox {
                                        id:sleepStopDefinationComBox
                                        implicitHeight: 35
                                        model: [ qsTr("Normal"), qsTr("Sleep"), qsTr("Deep sleep") ]
                                        font.pointSize: 8
                                        delegate: Delegates.ComboxDelegateTypical{}
                                    }

                                    //-- sleep delay
                                    Text { text: qsTr("Sleep delay"); font.pointSize: 8 }
                                    Row {
                                        Slider { id: sleepDelaySlider; from: 1; value: 1; to: 1000; snapMode: Slider.SnapAlways; Material.accent: "#406D9F"}
                                        Label {id: sleepDelay; anchors.verticalCenter: parent.verticalCenter; text: (sleepDelaySlider.value).toFixed(0); font.pointSize: 8 }
                                        Label {text: " seconds"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                    }
                                }
                            }
                        }
                        //-- speed alarm
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            Layout.leftMargin: 10
                            Column {
                                spacing: 10
                                width: parent.width
                                Label{ text: qsTr("Speed alarms"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                GridLayout {
                                    rows: 2
                                    columns: 2
                                    columnSpacing: 15
                                    rowSpacing: -5
                                    Layout.fillWidth: true
                                    //-- over speed
                                    Text { text: qsTr("Over speed"); font.pointSize: 8 }
                                    ComboBox {
                                        id:speedAlarmsComBox
                                        implicitHeight: 35
                                        model: [ qsTr("Enable"), qsTr("Disable") ]
                                        font.pointSize: 8
                                        delegate: Delegates.ComboxDelegateTypical{}
                                    }

                                    //-- max speed
                                    Text { text: qsTr("Max speed"); font.pointSize: 8 }
                                    Row {
                                        Slider { id: speedAlarmSlider; from: 1; value: 1; to: 300; snapMode: Slider.SnapAlways; Material.accent: "#406D9F"}
                                        Label {id: maxSpeed; anchors.verticalCenter: parent.verticalCenter; text: (speedAlarmSlider.value).toFixed(0); font.pointSize: 8 }
                                        Label {text: " km/h"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                    }
                                }
                            }
                        }
                        //-- temerature alarm
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            Layout.leftMargin: 10
                            Column {
                                spacing: 10
                                width: parent.width
                                Label{ text: qsTr("Temperature alarm"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                GridLayout {
                                    columns: 4
                                    rows: 3
                                    columnSpacing: 30
                                    rowSpacing: -10
                                    //-- Temp1
                                    Text { text: qsTr("Temp1"); font.pointSize: 8 }
                                    ComboBox {
                                        id:temp1AlarmsComBox
                                        implicitHeight: 35
                                        model: [ qsTr("Enable"), qsTr("Disable") ]
                                        font.pointSize: 8
                                        delegate: Delegates.ComboxDelegateTypical{}
                                    }

                                    //-- Temp2
                                    Text { text: qsTr("Temp2"); font.pointSize: 8 }
                                    ComboBox {
                                        id:temp2AlarmsComBox
                                        implicitHeight: 35
                                        model: [ qsTr("Enable"), qsTr("Disable") ]
                                        font.pointSize: 8
                                        delegate: Delegates.ComboxDelegateTypical{}
                                    }

                                    //-- minVal1
                                    Text { text: qsTr("MinVal1"); font.pointSize: 8}
                                    Row {
                                        Slider {id: tempAlarm1MinValSlider; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                        Label {id: tempAlarm1MinVal; anchors.verticalCenter: parent.verticalCenter; text: (tempAlarm1MinValSlider.value).toFixed(0); font.pointSize: 8 }
                                        Label {text: " °C"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                    }

                                    //-- minVal1
                                    Text { text: qsTr("MinVal2"); font.pointSize: 8 }
                                    Row {
                                        Slider { id: tempAlarm2MinValSlider; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                        Label {id: tempAlarm2MinVal; anchors.verticalCenter: parent.verticalCenter; text: (tempAlarm2MinValSlider.value).toFixed(0); font.pointSize: 8 }
                                        Label {text: " °C"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                    }

                                    //-- maxVal1
                                    Text { text: qsTr("MaxVal1"); font.pointSize: 8 }
                                    //                                            }
                                    Row {
                                        Slider {id: tempAlarm1MaxValSlider; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                        Label {id: tempAlarm1MaxVal; anchors.verticalCenter: parent.verticalCenter; text: (tempAlarm1MaxValSlider.value).toFixed(0); font.pointSize: 8 }
                                        Label {text: " °C"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                    }

                                    //-- MaxVal1
                                    Text { text: qsTr("MaxVal2"); font.pointSize: 8 }
                                    Row {
                                        Slider { id: tempAlarm2MaxValSlider; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                        Label {id: tempAlarm2MaxVal; anchors.verticalCenter: parent.verticalCenter; text: (tempAlarm2MaxValSlider.value).toFixed(0); font.pointSize: 8 }
                                        Label {text: " °C"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //-- authtorized (ibutton)
            Item {
                ScrollView {
                    width: parent.width
                    height: parent.height
                    //-- Authorized monitor phone
                    ColumnLayout {
                        width: parent.width
                        spacing: 5
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            Layout.leftMargin: 10
                            ColumnLayout {
                                width: parent.width
                                Label{ text: qsTr("Authorized monitor phone"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                GridLayout {
                                    rows: 4
                                    columns: 2
                                    columnSpacing: 15
                                    rowSpacing: -5
                                    Layout.fillWidth: true
                                    //-- auto answer
                                    Label { text: qsTr("Auto answer"); font.pointSize: 8}
                                    ComboBox {
                                        id:authorizedAutoAnswerComBox
                                        implicitHeight: 35
                                        model: [ qsTr("Enable"), qsTr("Disable") ]
                                        font.pointSize: 8
                                        delegate: Delegates.ComboxDelegateTypical{}
                                    }

                                    //-- phone 1
                                    Label { text: qsTr("Phone 1"); font.pointSize: 8}
                                    TextField { id:authorizedPhone1
                                        placeholderText: qsTr("+7900000000"); implicitWidth: 100; font.pointSize: 8;
                                        validator: RegExpValidator { regExp: /^[+]*[(]{0,1}[0-9]{1,4}[)]{0,1}[-\s\./0-9]*$/ }
                                        color: acceptableInput ? "black" : "red"
                                    }

                                    //-- phone 2
                                    Label { text: qsTr("Phone 2"); font.pointSize: 8}
                                    TextField { id:authorizedPhone2
                                        placeholderText: qsTr("+7900000000"); implicitWidth: 100; font.pointSize: 8;
                                        validator: RegExpValidator { regExp: /^[+]*[(]{0,1}[0-9]{1,4}[)]{0,1}[-\s\./0-9]*$/ }
                                        color: acceptableInput ? "black" : "red"
                                    }
                                    //-- phone 3
                                    Label { text: qsTr("Phone 3"); font.pointSize: 8}
                                    TextField { id:authorizedPhone3
                                        placeholderText: qsTr("+7903000000"); implicitWidth: 100; font.pointSize: 8;
                                        validator: RegExpValidator { regExp: /^[+]*[(]{0,1}[0-9]{1,4}[)]{0,1}[-\s\./0-9]*$/ }
                                        color: acceptableInput ? "black" : "red"
                                    }
                                }
                            }
                        }
                        //-- authtorized driving
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            Layout.leftMargin: 10
                            ColumnLayout {
                                width: parent.width
                                Label{ text: qsTr("Authorized driving"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                RowLayout {
                                    spacing: 50
                                    ColumnLayout {
                                        Rectangle {
                                            width: oneWireListView.width
                                            height: 30
                                            color: "silver"
                                            Row {
                                                anchors.verticalCenter: parent.verticalCenter
                                                Label {
                                                    id:authorizedOneWireNumberLabel
                                                    padding: 10
                                                    font.pointSize: 9
                                                    font.bold: false
                                                    text: "Number"
                                                }
                                                Label {
                                                    id:authorizedOneWireValueLabel
                                                    width: 100
                                                    padding: 10
                                                    font.pointSize: 9
                                                    font.bold: false
                                                    text: "1-Wire"
                                                }
                                            }
                                        }
                                        ListView {
                                            id: oneWireListView
                                            Layout.fillHeight: true
                                            Layout.preferredWidth: 300
                                            clip: true
                                            flickableDirection: Flickable.HorizontalAndVerticalFlick
                                            model: oneWireTableModel
                                            delegate:
                                                ColumnLayout {
                                                property int indexOfThisDelegate: index
                                                RowLayout {
                                                    Rectangle {
                                                        color: "transparent"
                                                        height: 30
                                                        width: 80
                                                        Label {
                                                            text: model.columnNum
                                                            anchors.horizontalCenter: parent.horizontalCenter
                                                            anchors.verticalCenter: parent.verticalCenter
                                                            font.bold: false
                                                            font.pointSize: 9
                                                        }
                                                    }
                                                    Rectangle {
                                                        color: "transparent"
                                                        height: 30
                                                        width: 200
                                                        id:oneWireNumRect
                                                        TextField {
                                                            id:oneWireNumText
                                                            text: model.columnOneWire
                                                            font.bold: false
                                                            font.pointSize: 9
                                                            placeholderText: qsTr("Please enter a number")
                                                            anchors.topMargin: 10
                                                            background: Item{}
                                                            validator: RegExpValidator { regExp: /^([a-fA-Z]|[0-9]){16}$/ }
                                                            onEditingFinished: {
                                                                model.columnOneWire = text
                                                            }
                                                        }
                                                        Rectangle {
                                                            width: oneWireNumText.width
                                                            height: 3
                                                            visible: oneWireNumText.cursorVisible
                                                            color: oneWireNumText.acceptableInput ? Material.color(Material.Blue) : Material.color(Material.Red)
                                                            anchors.bottom: oneWireNumRect.bottom
                                                        }
                                                    }
                                                    MouseArea {
                                                        width: parent.width
                                                        height: parent.height
                                                        acceptedButtons: Qt.RightButton
                                                        Menu {
                                                            id: toolMenuOneWireKey
                                                            MenuItem { text: qsTr("Remove")
                                                                onClicked: {
                                                                    viewControl.removeOneWireKey(indexOfThisDelegate)
                                                                }
                                                            }
                                                        }
                                                        onClicked: {
                                                            toolMenuOneWireKey.open()
                                                        }

                                                    }
                                                }
                                                Rectangle {
                                                    color: "black"
                                                    height: 1
                                                    width: oneWireListView.width
                                                }
                                            }
                                            onCountChanged: {
                                                oneWireListView.currentIndex = count - 1
                                            }
                                            ScrollIndicator.horizontal: ScrollIndicator { }
                                            ScrollIndicator.vertical: ScrollIndicator { }
                                        }
                                        Rectangle {
                                            width: oneWireListView.width
                                            height: 50
                                            color: "transparent"
                                            RowLayout{
                                                Button { id:authorizedAddButton
                                                    text: qsTr("Add key");
                                                    font.pointSize: 8
                                                    onClicked: {
                                                        var res = viewControl.addOneWireKey()
                                                        if(!res) {
                                                            error(qsTr("Add error") + "\r\n" + qsTr("Available limit is exceeded"))
                                                        }
                                                    }
                                                }
                                                Button { id:authorizedRemoveButton
                                                    text: qsTr("Remove last");
                                                    font.pointSize: 8
                                                    enabled: oneWireListView.count
                                                    onClicked: {
                                                        viewControl.removeOneWireKey(oneWireListView.count-1)
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    Image {
                                        id: ibuttonImage
                                        Layout.preferredWidth: 210
                                        Layout.preferredHeight: 256
                                        source: "qrc:/images/resources/ibutton.PNG"
                                    }
                                }
                            }
                        }
                    }
                }
            }
            //-- input/output
            Item {
                ScrollView {
                    width: parent.width
                    height: parent.height
                    Column {
                        width: parent.width
                        spacing: 5
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            ColumnLayout {
                                width: parent.width
                                Label{ text: qsTr("Digital input"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                RowLayout {
                                    spacing: 10
                                    GridLayout {
                                        rows: 5
                                        columns: 4
                                        columnSpacing: 15
                                        rowSpacing: -5
                                        //-- INT+
                                        Label { text: qsTr("IN1+"); font.pointSize: 8}
                                        Row {
                                            spacing: 10
                                            ComboBox {
                                                id:digitalIn1ComBox
                                                anchors.verticalCenter: parent.verticalCenter
                                                implicitHeight: 35
                                                model: [ qsTr("Enable"), qsTr("Disable") ]
                                                font.pointSize: 8
                                                delegate: Delegates.ComboxDelegateTypical{}
                                            }
                                        }
                                        //-- IN2
                                        Label { text: qsTr("IN2"); font.pointSize: 8}
                                        ComboBox {
                                            id:digitalIn2ComBox
                                            implicitHeight: 35
                                            model: [ qsTr("Enable"), qsTr("Disable") ]
                                            font.pointSize: 8
                                            delegate: Delegates.ComboxDelegateTypical{}
                                        }
                                        //-- IN3
                                        Label { text: qsTr("IN3"); font.pointSize: 8}
                                        ComboBox {
                                            id:digitalIn3ComBox
                                            implicitHeight: 35
                                            model: [ qsTr("Enable"), qsTr("Disable") ]
                                            font.pointSize: 8
                                            delegate: Delegates.ComboxDelegateTypical{}
                                        }
                                        //-- IN4
                                        Label { text: qsTr("IN4"); font.pointSize: 8}
                                        ComboBox {
                                            id:digitalIn4ComBox
                                            implicitHeight: 35
                                            model: [ qsTr("Enable"), qsTr("Disable") ]
                                            font.pointSize: 8
                                            delegate: Delegates.ComboxDelegateTypical{}
                                        }
                                        //-- IN5
                                        Label { text: qsTr("IN5"); font.pointSize: 8}
                                        ComboBox {
                                            id:digitalIn5ComBox
                                            implicitHeight: 35
                                            model: [ qsTr("Enable"), qsTr("Disable") ]
                                            font.pointSize: 8
                                            delegate: Delegates.ComboxDelegateTypical{}
                                        }
                                    }
                                }
                            }
                        }
                        //-- digital output
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            ColumnLayout {
                                width: parent.width
                                Label{ text: qsTr("Digital output"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                GridLayout {
                                    rows: 4
                                    columns: 2
                                    columnSpacing: 15
                                    rowSpacing: -5
                                    //-- Digital output 1
                                    Label { text: qsTr("Digital output 1"); font.pointSize: 8}
                                    ComboBox {  id:digitalOut1ComBox
                                        font.pointSize: 8; implicitHeight: 35; implicitWidth: 150;
                                        model: [ qsTr("Authorized driving"), qsTr("User defined"), qsTr("Disable") ]
                                        delegate: Delegates.ComboxDelegateTypical{} }

                                    Label { text: qsTr("Digital output 2"); font.pointSize: 8}
                                    ComboBox { id:digitalOut2ComBox
                                        font.pointSize: 8; implicitHeight: 35;
                                        model: [ qsTr("Buser"), qsTr("User defined"), qsTr("Disable") ]
                                        delegate: Delegates.ComboxDelegateTypical{} }

                                    Label { text: qsTr("Digital output 3"); font.pointSize: 8}
                                    ComboBox { id:digitalOut3ComBox
                                        font.pointSize: 8; implicitHeight: 35;
                                        model: [ qsTr("LED") ]
                                        enabled: false
                                        delegate: Delegates.ComboxDelegateTypical{} }

                                    Label { text: qsTr("Digital output 4"); font.pointSize: 8}
                                    ComboBox { id:digitalOut4ComBox
                                        font.pointSize: 8; implicitHeight: 35;
                                        model: [ qsTr("LED") ];
                                        enabled: false
                                        delegate: Delegates.ComboxDelegateTypical{} }
                                }
                            }
                        }
                    }
                }
            }

            //-- Analog OBD
            Item {
                ScrollView {
                    width: parent.width
                    height: parent.height
                    Column {
                        width: parent.width
                        spacing: 5

                        //-- analog input 1
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            Column {
                                //-- analog input enable/disable
                                enabled: false
                                RowLayout {
                                    spacing: 20
                                    RowLayout {
                                        Layout.alignment: Qt.AlignTop
                                        spacing: 10
                                        Label{ text: qsTr("Analog Input 1"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                        ComboBox {implicitWidth: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                    }
                                    GridLayout {
                                        columns: 2
                                        rows: 3
                                        rowSpacing: -20
                                        //-- Low level
                                        Label { text: qsTr("Low level"); font.pointSize: 8}
                                        Row {
                                            Slider { id: analogIn1LowLevelSlider; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                            Label {id: analogIn1LowLevel; anchors.verticalCenter: parent.verticalCenter; text: (analogIn1LowLevelSlider.value).toFixed(0); font.pointSize: 8 }
                                            Label {text: " mV"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                        }

                                        //-- high level
                                        Label { text: qsTr("High level"); font.pointSize: 8}
                                        Row {
                                            Slider { id: analogIn1HighLevelSlider; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                            Label {id: analogIn1HighLevel; anchors.verticalCenter: parent.verticalCenter; text: (analogIn1HighLevelSlider.value).toFixed(0); font.pointSize: 8 }
                                            Label {text: " mV"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                        }

                                        //-- percent
                                        Label { text: qsTr("Percent"); font.pointSize: 8}
                                        CheckBox {}
                                    }
                                }
                            }
                        }
                        //-- analog input 2
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            Column {
                                //-- analog input enable/disable
                                enabled: false
                                RowLayout {
                                    spacing: 20
                                    RowLayout {
                                        Layout.alignment: Qt.AlignTop
                                        spacing: 10
                                        Label{ text: qsTr("Analog Input 2"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                        ComboBox {implicitWidth: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                    }
                                    GridLayout {
                                        columns: 2
                                        rows: 3
                                        rowSpacing: -20
                                        //-- Low level
                                        Label { text: qsTr("Low level"); font.pointSize: 8}
                                        Row {
                                            Slider { id: analogIn2LowLevelSlider; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                            Label {id: analogIn2LowLevel; anchors.verticalCenter: parent.verticalCenter; text: (analogIn2LowLevelSlider.value).toFixed(0); font.pointSize: 8 }
                                            Label {text: " mV"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                        }
                                        //-- high level
                                        Label { text: qsTr("High level"); font.pointSize: 8}
                                        Row {
                                            Slider { id: analogIn2HighLevelSlider; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                            Label {id: analogIn2HighLevel; anchors.verticalCenter: parent.verticalCenter; text: (analogIn2HighLevelSlider.value).toFixed(0); font.pointSize: 8 }
                                            Label {text: " mV"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                        }
                                        //-- percent
                                        Label { text: qsTr("Percent"); font.pointSize: 8}
                                        CheckBox {}
                                    }
                                }
                            }
                        }
                        //-- analog input 3
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            Column {
                                //-- analog input enable/disable
                                enabled: false
                                RowLayout {
                                    spacing: 20
                                    RowLayout {
                                        Layout.alignment: Qt.AlignTop
                                        spacing: 10
                                        Label{ text: qsTr("Analog Input 3"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                        ComboBox {implicitWidth: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                    }
                                    GridLayout {
                                        columns: 2
                                        rows: 3
                                        rowSpacing: -20
                                        //-- Low level
                                        Label { text: qsTr("Low level"); font.pointSize: 8}
                                        Row {
                                            Slider { id: analogIn3LowLevelSlider; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                            Label {id: analogIn3LowLevel; anchors.verticalCenter: parent.verticalCenter; text: (analogIn3LowLevelSlider.value).toFixed(0); font.pointSize: 8 }
                                            Label {text: " mV"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                        }
                                        //-- high level
                                        Label { text: qsTr("High level"); font.pointSize: 8}
                                        Row {
                                            Slider { id: analogIn3HighLevelSlider; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                            Label {id: analogIn3HighLevel; anchors.verticalCenter: parent.verticalCenter; text: (analogIn3HighLevelSlider.value).toFixed(0); font.pointSize: 8 }
                                            Label {text: " mV"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                        }
                                        //-- percent
                                        Label { text: qsTr("Percent"); font.pointSize: 8}
                                        CheckBox {}
                                    }
                                }
                            }
                        }
                        //-- analog input 4
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            Column {
                                //-- analog input enable/disable
                                enabled: false
                                RowLayout {
                                    spacing: 20
                                    RowLayout {
                                        Layout.alignment: Qt.AlignTop
                                        spacing: 10
                                        Label{ text: qsTr("Analog Input 4"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                        ComboBox {implicitWidth: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                    }
                                    GridLayout {
                                        columns: 2
                                        rows: 3
                                        rowSpacing: -20
                                        //-- Low level
                                        Label { text: qsTr("Low level"); font.pointSize: 8}
                                        Row {
                                            Slider { id: analogIn4LowLevelSlider; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                            Label {id: analogIn4LowLevel; anchors.verticalCenter: parent.verticalCenter; text: (analogIn4LowLevelSlider.value).toFixed(0); font.pointSize: 8 }
                                            Label {text: " mV"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                        }
                                        //-- high level
                                        Label { text: qsTr("High level"); font.pointSize: 8}
                                        Row {
                                            Slider { id: analogIn4HighLevelSlider; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                            Label {id: analogIn4HighLevel; anchors.verticalCenter: parent.verticalCenter; text: (analogIn4HighLevelSlider.value).toFixed(0); font.pointSize: 8 }
                                            Label {text: " mV"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                        }
                                        //-- percent
                                        Label { text: qsTr("Percent"); font.pointSize: 8}
                                        CheckBox {}
                                    }
                                }
                            }
                        }
                        //-- OBD
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            ColumnLayout {
                                spacing: -10
                                //-- header OBD
                                Label{ text: qsTr("OBD"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                GridLayout {
                                    columns: 2
                                    rows: 3
                                    rowSpacing: -20
                                    //-- OBD data time
                                    Label { text: qsTr("OBD data time"); font.pointSize: 8 }
                                    RowLayout {
                                        Row {
                                            Slider { id: obdDataTimeSlider; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                            Label {id: obdDataTimeLevel; anchors.verticalCenter: parent.verticalCenter; text: (obdDataTimeSlider.value).toFixed(0); font.pointSize: 8 }
                                            Label {text: " seconds"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                        }
                                    }
                                    //-- Max acceleration
                                    Label { text: qsTr("Max acceleration"); enabled: false; font.pointSize: 8}
                                    RowLayout {
                                        Row {
                                            enabled: false;
                                            Slider { id: obdMaxAccelerationSlider; enabled: false; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                            Label {id: obdMaxAcceleration; enabled: false; anchors.verticalCenter: parent.verticalCenter; text: (obdMaxAccelerationSlider.value).toFixed(0); font.pointSize: 8 }
                                            Label {text: " m/s^2*10"; enabled: false; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                        }
                                    }
                                    //-- Max breaking
                                    Label { text: qsTr("Max breaking"); enabled: false; font.pointSize: 8}
                                    RowLayout {
                                        Row {
                                            enabled: false;
                                            Slider { id: obdMaxBreakingSlider; from: 1; value: 1; to: 150; snapMode: Slider.SnapAlways; implicitWidth: 150; Material.accent: "#406D9F"}
                                            Label {id: obdMaxBreaking; anchors.verticalCenter: parent.verticalCenter; text: (obdMaxBreakingSlider.value).toFixed(0); font.pointSize: 8 }
                                            Label {text: " m/s^2*10"; anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //-- geofence
            Item {
                ScrollView {
                    width: parent.width
                    height: parent.height
                    Column {
                        width: parent.width
                        spacing: 5
                        //-- geofence
                        Pane {
                            Material.elevation: 6
                            width: parent.width
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            Column {
                                width: parent.width
                                //-- geofence label
                                Label{ text: qsTr("Geofence"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                GridLayout {
                                    rows: 5
                                    columns: 2
                                    columnSpacing: 10
                                    rowSpacing: -5
                                    //-- enter event
                                    Label { text: qsTr("Enter event"); font.pointSize: 8}
                                    ComboBox {font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                        implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }

                                    //-- exit event
                                    Label { text: qsTr("Exit event"); font.pointSize: 8}
                                    ComboBox {font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                        implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }

                                    //-- radius
                                    Label { text: qsTr("Radius"); font.pointSize: 8}
                                    Row{ spacing: 10
                                        Slider { id: geofenceRadiusSlider;
                                            from: 0; value: 2000; to: 99999; snapMode: Slider.SnapAlways; implicitWidth: 150
                                            Material.accent: "#406D9F"
                                            onValueChanged: {
                                                map.circle.radius = geofenceRadiusSlider.value
                                            }
                                        }
                                        Label {id: geofenceRadius; anchors.verticalCenter: parent.verticalCenter; text: (geofenceRadiusSlider.value).toFixed(0); font.pointSize: 8 }
                                        Label {text: qsTr("Distance in meters"); anchors.verticalCenter: parent.verticalCenter; font.pointSize: 8}
                                    }

                                    //-- latitude
                                    Label { text: qsTr("Longitude"); font.pointSize: 8}
                                    TextField {
                                        id:longitudeTextField
                                        placeholderText: qsTr("Enter latitude"); implicitWidth: 200; font.pointSize: 8;
                                        validator: RegExpValidator { regExp: /^\d{0,4}(\.\d{0,10}){0,5}$/ }
                                        color: acceptableInput ? "black" : "red"
                                        onTextChanged: {
                                            if(acceptableInput) {
                                                mapUpdatePosition(latitudeTextField.text, parseFloat(longitudeTextField.text))
                                            }
                                        }
                                    }

                                    //-- longitude
                                    Label { text: qsTr("Latitude"); font.pointSize: 8}
                                    TextField {
                                        id:latitudeTextField
                                        placeholderText: qsTr("Enter longitude"); implicitWidth: 200; font.pointSize: 8;
                                        validator: RegExpValidator { regExp: /^\d{0,4}(\.\d{0,10}){0,5}$/ }
                                        color: acceptableInput ? "black" : "red"
                                        onTextChanged: {
                                            if(acceptableInput) {
                                                mapUpdatePosition(latitudeTextField.text, parseFloat(longitudeTextField.text))
                                            }
                                        }
                                    }
                                }

                                Plugin {
                                    id: mapPlugin
                                    name: "esri" // "osm", "mapboxgl", "esri"
                                }
                                Map {
                                    id:map
                                    width: parent.width
                                    height: 300
                                    plugin: mapPlugin
                                    center: QtPositioning.coordinate(parseFloat(latitudeTextField.text), parseFloat(longitudeTextField.text))
                                    zoomLevel: 13
                                    property MapCircle circle
                                    Component.onCompleted: {
                                        map.circle = Qt.createQmlObject('import QtLocation 5.3; MapCircle {}', map)
                                        map.circle.center = QtPositioning.coordinate(parseFloat(latitudeTextField.text), parseFloat(longitudeTextField.text))
                                        map.circle.radius = geofenceRadiusSlider.value
                                        map.circle.color = 'transparent'
                                        map.circle.border.color = Material.color(Material.Red)
                                        map.circle.border.width = 3
                                        map.addMapItem(circle)
                                    }
                                    MapQuickItem {
                                        id: marker
                                        sourceItem: Rectangle { width: 14; height: 14; color: "#e41e25"; border.width: 2; border.color: "white"; smooth: true; radius: 7 }
                                        coordinate {
                                            latitude: 59.9485
                                            longitude: 10.7686
                                        }
                                        opacity: 1.0
                                        anchorPoint: Qt.point(sourceItem.width/2, sourceItem.height/2)
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //-- sms alarm
            Item {
                ScrollView {
                    width: parent.width
                    height: parent.height
                    Column {
                        width: parent.width
                        spacing: 5
                        Pane {
                            Material.elevation: 6
                            width: parent.width
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            ColumnLayout {
                                width: parent.width
                                Row {
                                    spacing: 10
                                    Label{ text: qsTr("Target phone number"); font.pointSize: 8; anchors.verticalCenter: parent.verticalCenter;
                                        color: Material.color(Material.Green, Material.Shade800) }
                                    TextField {placeholderText: qsTr("+7900000000");
                                        anchors.verticalCenter: parent.verticalCenter; implicitWidth: 100; font.pointSize: 8;
                                        validator: RegExpValidator { regExp: /^[+]*[(]{0,1}[0-9]{1,4}[)]{0,1}[-\s\./0-9]*$/ }
                                        color: acceptableInput ? "black" : "red"
                                    }
                                }
                            }
                        }
                        Pane {
                            Material.elevation: 6
                            implicitWidth: propertyPanelSwipe.width - 15
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            ColumnLayout {
                                width: parent.width
                                Label{ text: qsTr("Digital input"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800) }
                                RowLayout {
                                    spacing: 10
                                    GridLayout {
                                        rows: 5
                                        columns: 4
                                        columnSpacing: 10
                                        //-- INT+
                                        // 1 line
                                        Label{ text: qsTr("GPS antenna cut"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmGpsAntennaCutComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{}
                                        }
                                        Label{ text: qsTr("External power cut"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmExternalPowerCutComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                        // 2 line
                                        Label{ text: qsTr("Tow"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmTowComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                        Label{ text: qsTr("IN5(SOS)"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmIn5SosComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{}}
                                        // 3 line
                                        Label{ text: qsTr("GEO IN"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmGeoInComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{}}
                                        Label{ text: qsTr("GEO OUT"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmGeoOutComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{}}
                                        // 4 line
                                        Label{ text: qsTr("IN1 ON"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmIn1OnComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                        Label{ text: qsTr("IN1 OFF"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmIn1OffComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                        // 5 line
                                        Label{ text: qsTr("IN2 ON"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmIn2OnComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                        Label{ text: qsTr("IN2 OFF"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmIn2OffComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                        // 6 line
                                        Label{ text: qsTr("IN3 ON"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmIn3OnComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                        Label{ text: qsTr("IN3 OFF"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmIn3OffComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                        // 7 line
                                        Label{ text: qsTr("IN4 ON"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmIn4ONComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                        Label{ text: qsTr("IN4 OFF"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmIn4OffComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                        // 8 line
                                        Label{ text: qsTr("Low battery voltage"); font.pointSize: 8 }
                                        ComboBox { id: smsAlarmLowBattVoltageComBox
                                            width: 150; font.pointSize: 8; model: [ qsTr("Enable"), qsTr("Disable") ];
                                            implicitHeight: 35; delegate: Delegates.ComboxDelegateTypical{} }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Item {
        //-- settings
        id:settingsWidget
        ScrollView {
            anchors.fill: parent
            //-- settings
            ColumnLayout {
                spacing: 5
                anchors.fill: parent
                Pane {
                    Material.elevation: 2
                    Layout.leftMargin: 10
                    implicitWidth: propertyPanelSwipe.width - 15
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

    Item {
        //-- about
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
                    implicitWidth: propertyPanelSwipe.width - 15
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

    //-- unit test
    Item {
        id:unitTestsWidget
        clip: true
        ScrollView {
            anchors.fill: parent
            //-- unit test
            ColumnLayout {
                spacing: 5
                anchors.fill: parent
                //-- network setup
                Pane {
                    Material.elevation: 2
                    Layout.leftMargin: 10
                    implicitWidth: propertyPanelSwipe.width - 15
                    Column {
                        enabled: false
                        Label{ text: qsTr("Network setup"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                        RowLayout { spacing: 10
                            //-- ip
                            Row { spacing: 10
                                Label { text: qsTr("IP"); font.pointSize: 8; anchors.verticalCenter: parent.verticalCenter }
                                //-- ip
                                TextField {placeholderText: qsTr("Enter ip address"); implicitWidth: 200; font.pointSize: 8;
                                    validator: RegExpValidator { regExp: /^(?=.*[^\.]$)((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.?){4}$/ }
                                    color: acceptableInput ? "black" : "red"
                                }
                            }
                            //-- port
                            Row { spacing: 10
                                Label { text: qsTr("Port"); font.pointSize: 8; anchors.verticalCenter: parent.verticalCenter }
                                //-- port
                                TextField {placeholderText: qsTr("Enter port"); implicitWidth: 200; font.pointSize: 8;
                                    validator: RegExpValidator { regExp: /^{1-1024}$/ }
                                    color: acceptableInput ? "black" : "red"
                                }
                            }
                            Button { Material.background: Material.BlueGrey; font.pointSize: 8; Material.foreground: "white"; text: qsTr("Monitor"); }
                        }
                    }
                }
                //-- input command
                Pane {
                    Material.elevation: 2
                    Layout.leftMargin: 10
                    implicitWidth: propertyPanelSwipe.width - 15
                    Column {
                        enabled: false
                        Label{ text: qsTr("Input commmand"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                        RowLayout {
                            spacing: 10
                            //-- command
                            TextField {placeholderText: qsTr("Enter command"); implicitWidth: 200; font.pointSize: 8}
                            //-- com send
                            Button { Material.background: Material.BlueGrey; font.pointSize: 8; Material.foreground: "white"; text: qsTr("COM send"); }
                            //-- gprs send
                            Button { Material.background: Material.BlueGrey; font.pointSize: 8; Material.foreground: "white"; text: qsTr("GPRS send"); }
                        }
                    }
                }
                //-- display log
                Pane {
                    Material.elevation: 2
                    Layout.leftMargin: 10
                    implicitWidth: propertyPanelSwipe.width - 15
                    Column {
                        RowLayout  {
                            spacing: 10
                            RowLayout {
                                spacing: 5
                                Label{ text: qsTr("Display log"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                                CheckBox { id:unitTestDisplayLogShow; checked: true }
                            }
                            RowLayout {
                                spacing: 5
                                enabled: false
                                Label{ text: qsTr("Write log"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                                CheckBox { id:unitTestDisplayLogWriteLog }
                            }
                        }
                    }
                }
                Pane { id:logPanel
                    Material.elevation: 2
                    Layout.leftMargin: 10
                    implicitWidth: propertyPanelSwipe.width - 15
                    implicitHeight: 300
                    ListView {
                        clip: true
                        id: unitTestDisplayLogList
                        implicitHeight: logPanel.height - 20
                        implicitWidth: logPanel.width
                        Layout.alignment: Qt.AlignCenter
                        ScrollBar.vertical: ScrollBar { width: 20; enabled: true }
                        delegate: Item {
                            id: item
                            height: 25
                            width: parent.width
                            Label {
                                text: model.text
                                font.pointSize: 9
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        model: ListModel {
                            id: listModel
                        }
                    }
                }
            }
        }
    }
}
