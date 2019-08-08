import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtCharts 2.3

SwipeView {
    id:qch1PannelScroll
    Layout.fillWidth: true
    Layout.fillHeight: true
    clip: true
    interactive: false

    function drawChart(dataArray, chartLine, chart) {
        chartLine.clear();
        chart.graphLength = dataArray.length
        chart.graphAmplitudeMax = 0
        for(var i=0; i<dataArray.length; i++) {
            if(chart.graphAmplitudeMax < parseInt(dataArray[i])) {
                chart.graphAmplitudeMax = parseInt(dataArray[i]);
            }
        }
        for(i=0; i<dataArray.length; i++) {
            chartLine.append(i, parseInt(dataArray[i]));
        }
    }

    function setTestIndicationg(testStatus, testProgressBar, testRectangle) {
        switch(testStatus) {
        case "idle":
            testProgressBar.value = 0;
            testRectangle.color = "red"
            break;
        case "process":
            testProgressBar.value = 50;
            testRectangle.color = "yellow"
            break;
        case "fail":
            testProgressBar.value = 100;
            testRectangle.color = "red"
            break;
        case "finished":
            testProgressBar.value = 100;
            testRectangle.color = "green"
            break;
        }
    }

    Connections {
        target: viewControl
        //        onSignalUpdateRealTimeData: {
        //            var jsonData = JSON.parse(json)
        //            llsPowerVoltageLabel.text = jsonData.power_input.toFixed(2)
        //            llsPowerCurrentLabel.text = jsonData.power_current.toFixed(2)
        //            llsCntLabel.text = jsonData.cnt
        //            llsMcuSnDeviceLabel.text = jsonData.mcu_serial_number
        //            llsMcuSnLabel.text = jsonData.mcu_serial_number
        //            llsSnDeviceLabel.text = jsonData.serial_number

        //            drawChart(jsonData.powerCollect, chartVoltageLine, chartVoltage)
        //            drawChart(jsonData.currentCollect, chartCurrentLine, chartCurrent)
        //            drawChart(jsonData.cntCollect, chartCapacityLine, chartCapacity)

        //            if(busyIndicator.visible == true) {
        //                busyIndicator.visible = false;
        //            }
        //        }
    }

    //    Connections {
    //        target: viewControl

    //        onSignalTestFinished: {
    //            var jsonData = JSON.parse(json)
    //            if(jsonData.result === "finished") {
    //                addToDatabaseRectangle.color = "green"; addToDatabaseProgressBar.value = 100;
    //                programmingRectangle.color = "green"; programmingProgressBar.value = 100;
    //                test232Rectangle.color = "green"; test232ProgressBar.value = 100;
    //                test485Rectangle.color = "green"; test485ProgressBar.value = 100;
    //                testFreqRectangle.color = "green"; testFreqProgressBar.value = 100;
    //                toast.displayMessage(qsTr("Test completed successfully") + "\r\n" +
    //                                     "rs232: tx " + jsonData.test232.sendPackets +
    //                                     ", rx " + jsonData.test232.receivePackets + "\r\n" +
    //                                     "rs485: tx " + jsonData.test485.sendPackets +
    //                                     ", rx " + jsonData.test485.receivePackets + "\r\n" +
    //                                     "cnt test: " +
    //                                     "step1: " + jsonData.testFreq.capStep1 +
    //                                     ", step2: " + jsonData.testFreq.capStep2 +
    //                                     ", step3: " + jsonData.testFreq.capStep3, "good");
    //            } else {
    //                //addToDatabaseRectangle.color = "red"; addToDatabaseProgressBar.value = 0;
    //                //programmingRectangle.color = "red"; programmingProgressBar.value = 0;
    //                test232Rectangle.color = jsonData.test232.testResult === "finished" ? "green" : "red"; test232ProgressBar.value = 0;
    //                test485Rectangle.color = jsonData.test485.testResult === "finished" ? "green" : "red"; test485ProgressBar.value = 0;
    //                testFreqRectangle.color = jsonData.testFreq.testResult === "finished" ? "green" : "red"; testFreqProgressBar.value = 0;
    //                toast.displayMessage(qsTr("Test completed failed") + "\r\n" +
    //                                     "rs232: tx " + jsonData.test232.sendPackets +
    //                                     ", rx " + jsonData.test232.receivePackets + "\r\n" +
    //                                     "rs485: tx " + jsonData.test485.sendPackets +
    //                                     ", rx " + jsonData.test485.receivePackets + "\r\n" +
    //                                     "cnt test: " +
    //                                     "step1: " + jsonData.testFreq.capStep1 +
    //                                     ", step2: " + jsonData.testFreq.capStep2 +
    //                                     ", step3: " + jsonData.testFreq.capStep3, "bad");
    //            }
    //        }

    //        onSignalTestError: {
    //            var jsonData = JSON.parse(json)
    //            addToDatabaseRectangle.color = "red"; addToDatabaseProgressBar.value = 0;
    //            programmingRectangle.color = "red"; programmingProgressBar.value = 0;
    //            test232Rectangle.color = "red"; test232ProgressBar.value = 0;
    //            test485Rectangle.color = "red"; test485ProgressBar.value = 0;
    //            testFreqRectangle.color = "red"; testFreqProgressBar.value = 0;
    //            toast.displayMessage(jsonData.message, "bad");
    //        }

    //        onSignalTestUpdateStatus: {
    //            var jsonData = JSON.parse(json)
    //            if(jsonData.testStep === "programming") {
    //                addToDatabaseRectangle.color = "green"
    //                addToDatabaseProgressBar.value = parseInt(jsonData.percent)
    //                if(parseInt(jsonData.percent) < 100) {
    //                    programmingRectangle.color = "yellow"
    //                } else {
    //                    programmingRectangle.color = "green"
    //                }
    //                programmingProgressBar.value = parseInt(jsonData.percent)
    //            }
    //            if(jsonData.testStep === "waitTestNotEnd") {
    //                setTestIndicationg(jsonData.test232.testResult, test232ProgressBar, test232Rectangle)
    //                setTestIndicationg(jsonData.test485.testResult, test485ProgressBar, test485Rectangle)
    //                setTestIndicationg(jsonData.testFreq.testResult, testFreqProgressBar, testFreqRectangle)
    //            }
    //        }
    //    }

    //        Shortcut {
    //          sequence: "Space"
    //          onActivated: {
    //              if(viewControl.isConnected()) {
    //                  viewControl.startTestStend()
    //                  toast.flush()
    //              } else {
    //                  toast.displayMessage(qsTr("You need to establish a connection"), "neutral");
    //              }
    //          }
    //        }
    //    }

    ColumnLayout {
        spacing: 5
        RowLayout {
            //-- panel
            Pane {
                id: logPanel
                Material.elevation: 2
                implicitWidth: 200
                implicitHeight: qch1PannelScroll.height - 20
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                ColumnLayout {
                    width: parent.width
                    spacing: 30
                    Label{ text: qsTr("Log"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                    ListView { id:logListView
                        Layout.preferredWidth: logPanel.width - 30
                        height: 500
                        clip: true
                        model: logStendModel
                        delegate: Rectangle {
                            width: parent.width
                            height: 50
                            color: "transparent"
                            TextEdit {
                                text: model.text
                                color: model.color
                                font.bold: false
                                font.pointSize: 9
                                readOnly: true
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                wrapMode: TextEdit.WrapAnywhere
                            }
                        }
                        ScrollIndicator.horizontal: ScrollIndicator {}
                        ScrollIndicator.vertical: ScrollIndicator {}
                    }
                }
            }
            //-- test panel
            Pane {
                Material.elevation: 6
                Layout.leftMargin: 10
                implicitWidth: 330
                implicitHeight: qch1PannelScroll.height - 20
                Layout.alignment: Qt.AlignTop

                ScrollView {
                    id: frame
                    anchors.fill: parent
                    clip: true
                    Column {
                        width: parent.width
                        Label{ text: qsTr("Tests"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                        spacing: 15
                        GridLayout {
                            rows: 7
                            columns: 2
                            columnSpacing: 10
                            rowSpacing: 10

                            Label {
                                text: qsTr("Power voltage:")
                            }
                            RowLayout {
                                Label { id:llsPowerVoltageLabel
                                    text: "0.0"
                                }
                                Label { text: "v"}
                            }

                            Label {
                                text: qsTr("Power current:")
                            }
                            RowLayout {
                                Label { id:llsPowerCurrentLabel
                                    text: "0.0"
                                }
                                Label { text: "mA"}
                            }

                            Label {
                                text: qsTr("CNT:")
                            }
                            RowLayout {
                                Label { id:llsCntLabel
                                    text: "0"
                                }
                                Label { text: "Hz"}
                            }

                            Label {
                                text: qsTr("MCU SN:")
                            }
                            Label { id:llsMcuSnLabel
                                text: "NA"
                            }

                            Label {
                                text: qsTr("SN:")
                            }
                            Label { id:llsSnLabel
                                text: "NA"
                            }

                            Label {
                                text: qsTr("Scan.num:")
                            }
                            Label { id:llsScannedNumLabel; text: "NA" }

                            Label { text: qsTr("Temperature:") }
                            RowLayout {
                                Label { id:llsTempLabel; text: "NA" }
                                Label { text: "°C" }
                            }

                            Label { text: qsTr("RS485:") }
                            Rectangle {
                                id:llsRs485StatusRectangle
                                width: 32; height: 32
                                color: "red"
                            }

                            Label { text: qsTr("RS232:") }
                            Rectangle {
                                id:llsRs232StatusRectangle
                                width: 32; height: 32
                                color: "red"
                            }

                            Label { text: "Empty:" }
                            Label { id:llsEmptyValueLabel; text: "NA" }

                            Label { text: "Full:" }
                            Label { id:llsfullValueLabel; text: "NA" }
                        }

                        ColumnLayout {
                            spacing: 10
                            RowLayout {
                                spacing: 10
                                Button {
                                    id:llsWriteButton;
                                    Material.background: Material.Green;
                                    Material.foreground: "white";
                                    text: qsTr("Write to LLS");
                                    icon.source:"qrc:/svg/resources/fonts/svgs/solid/pen.svg"
                                    icon.width: 16; icon.height: 16
                                    font.pointSize: 8
                                    implicitHeight: 50
                                    focus: true
                                    onClicked: {
                                    }
                                }
                                Button {
                                    id:llsMarkAsDefectiveButton;
                                    Material.background: Material.Green;
                                    Material.foreground: "white";
                                    text: qsTr("Mark as defective");
                                    icon.source:"qrc:/svg/resources/fonts/svgs/solid/trash.svg"
                                    icon.width: 16; icon.height: 16
                                    font.pointSize: 8
                                    implicitHeight: 50
                                    focus: true
                                    onClicked: {
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //-- test panel
            Pane {
                Material.elevation: 6
                Layout.leftMargin: 10
                Layout.fillWidth: true
                implicitHeight: qch1PannelScroll.height - 20
                Layout.alignment: Qt.AlignTop

                ChartView {
                    id: chart
                    theme: ChartView.ChartThemeLight
                    antialiasing: true
                    height: parent.height
                    width: parent.width
                    legend.alignment: Qt.AlignTop
                    backgroundColor: "transparent"
                    property date graphMinTime: new Date()
                    property date graphMaxTime: new Date()
                    DateTimeAxis {
                        id: chartAxisX
                        format: "hh:mm:ss"
                        min: chart.graphMinTime
                        max: chart.graphMaxTime
                        tickCount: 5
                    }
                    ValueAxis {
                        id: chartVoltageAxisY
                        min: -0.5
                        max: (chart.series(chartVoltageLine) !== null) ?  chart.series(chartVoltageLine).max : 1
                        tickCount: 5
                        color: "blue"
                    }
                    ValueAxis {
                        id: chartCurrentAxisY
                        min: -0.5
                        max: (chart.series(chartCurrentLine) !== null) ?  chart.series(chartCurrentLine).max : 1
                        tickCount: 5
                        color: "red"
                    }
                    ValueAxis {
                        id: chartCntAxisY
                        min: -0.5
                        max: (chart.series(chartCntLine) !== null) ?  chart.series(chartCntLine).max : 1
                        tickCount: 5
                        color: "orange"
                    }
                    LineSeries {
                        id: chartVoltageLine
                        axisX: chartAxisX
                        axisY: chartVoltageAxisY
                        color: "blue"
                        name: "Voltage"
                        property int max: 1
                    }
                    LineSeries {
                        id: chartCurrentLine
                        axisX: chartAxisX
                        axisY: chartCurrentAxisY
                        color: "red"
                        name: "Current"
                        property int max: 1
                    }
                    LineSeries {
                        id: chartCntLine
                        axisX: chartAxisX
                        axisYRight: chartCntAxisY
                        color: "orange"
                        name: "Cnt"
                        property int max: 1
                    }
                }
            }
        }
    }
}
