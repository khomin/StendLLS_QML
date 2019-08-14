import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtCharts 2.3

SwipeView {
    id:firmwarePannelScroll
    Layout.fillWidth: true
    Layout.fillHeight: true
    clip: true
    interactive: false

    Connections {
        target: viewControl
        onSignalUpdateRealTimeData: {
            var jsonData = JSON.parse(json)
            llsPowerVoltageLabel.text = jsonData.power_input.toFixed(2)
            llsPowerCurrentLabel.text = jsonData.power_current.toFixed(2)
            llsFreqLabel.text = jsonData.freq

            if(jsonData.mcu_serial_number !== "303030303030303030303030") {
                llsMcuSnLabel.text = jsonData.mcu_serial_number
            } else {
                llsMcuSnLabel.text = "NA";
            }
            llsMcuSnDeviceLabel.text = llsMcuSnLabel.text

            llsSnDeviceLabel.text = jsonData.serial_number

            drawChartLine(chart, chartVoltageLine, jsonData.powerCollect);
            drawChartLine(chart, chartCurrentLine, jsonData.currentCollect);
            drawChartLine(chart, chartCntLine, jsonData.cntCollect);

            if(busyIndicator.visible == true) {
                busyIndicator.visible = false;
            }
        }
    }


    function drawChartLine(chart, chartLine, dataArray) {
        chartLine.clear();

        chart.graphMinTime = new Date(dataArray[0].x);
        chart.graphMaxTime = new Date(dataArray[dataArray.length-1].x);

        var maxValue = findMaxValue(dataArray);

        dataArray.forEach(function (value) {
            chartLine.append(new Date(value.x), parseInt(value.y));
        });
        if (chartLine.axisY !== null) {
            chartLine.axisY.max = maxValue;
        }

        if(chartLine.axisYRight !== null) {
            chartLine.axisYRight.max = maxValue;
        }
    }

    function findMaxValue(dataArray) {
        var maxValue = 0;
        dataArray.forEach(function (value) {
            if(maxValue < parseInt(value.y)) {
                maxValue = parseInt(value.y);
            }
        });
        return maxValue*1.05+1;
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

    //-- data acquition send
    ColumnLayout {
        spacing: 5
        RowLayout {
            //-- log panel
            Pane {
                id: logPanel
                Material.elevation: 2
                implicitWidth: 200
                implicitHeight: firmwarePannelScroll.height - mcuSnPanel.height - 20
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
                Layout.alignment: Qt.AlignTop
                Column {
                    width: parent.width
                    Label{ text: qsTr("Tests"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                    spacing: 30
                    GridLayout {
                        rows: 7
                        columns: 2
                        columnSpacing: 10
                        rowSpacing: 10

                        RowLayout{
                            Rectangle {
                                id:addToDatabaseRectangle
                                width: 32; height: 32
                                color: "red"
                            }
                            ProgressBar {
                                id:addToDatabaseProgressBar
                                implicitWidth: 70; from: 0; to: 100;
                            }
                        }
                        Label {
                            text: qsTr("Add device to database")
                        }
                        RowLayout {
                            Rectangle {
                                id:programmingRectangle
                                width: 32; height: 32
                                color: "red"
                            }
                            ProgressBar {
                                id:programmingProgressBar
                                implicitWidth: 70; from: 0; to: 100;
                            }
                        }
                        Label {
                            text: qsTr("Programming")
                        }
                        RowLayout {
                            Rectangle {
                                id:test232Rectangle
                                width: 32; height: 32
                                color: "red"
                            }
                            ProgressBar {
                                id:test232ProgressBar
                                implicitWidth: 70; from: 0; to: 100;
                            }
                        }
                        Label {
                            text: qsTr("Test 232")
                        }
                        RowLayout{
                            Rectangle {
                                id:test485Rectangle
                                width: 32; height: 32
                                color: "red"
                            }
                            ProgressBar {
                                id:test485ProgressBar
                                implicitWidth: 70; from: 0; to: 100;
                            }
                        }
                        Label {
                            text: qsTr("Test 485")
                        }
                        RowLayout {
                            Rectangle {
                                id:testFreqRectangle
                                width: 32; height: 32
                                color: "red"
                            }
                            ProgressBar {
                                id:testFreqProgressBar
                                implicitWidth: 70; from: 0; to: 100;
                            }
                        }
                        Label {
                            text: qsTr("Test frequency")
                        }
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
                            text: qsTr("Frequency:")
                        }
                        RowLayout {
                            Label { id:llsFreqLabel
                                text: "0"
                            }
                            Label { text: "Hz"}
                        }
                        Label {
                            text: qsTr("MCU SN:")
                        }
                        RowLayout {
                            Label { id:llsMcuSnLabel
                                text: "---"
                            }
                        }

                        Connections {
                            target: viewControl
                            onSignalTestFinished: {
                                if(viewControl.stendRole == "firmare") {
                                    var jsonData = JSON.parse(json)
                                    if(jsonData.result === "finished") {
                                        addToDatabaseRectangle.color = "green"; addToDatabaseProgressBar.value = 100;
                                        programmingRectangle.color = "green"; programmingProgressBar.value = 100;
                                        test232Rectangle.color = "green"; test232ProgressBar.value = 100;
                                        test485Rectangle.color = "green"; test485ProgressBar.value = 100;
                                        testFreqRectangle.color = "green"; testFreqProgressBar.value = 100;
                                        toast.displayMessage(qsTr("Test completed successfully") + "\r\n" +
                                                             "rs232: tx " + jsonData.test232.sendPackets +
                                                             ", rx " + jsonData.test232.receivePackets + "\r\n" +
                                                             "rs485: tx " + jsonData.test485.sendPackets +
                                                             ", rx " + jsonData.test485.receivePackets + "\r\n" +
                                                             "cnt test: " +
                                                             "step1: " + jsonData.testFreq.capStep1 +
                                                             ", step2: " + jsonData.testFreq.capStep2 +
                                                             ", step3: " + jsonData.testFreq.capStep3, "good");
                                    } else {
                                        //addToDatabaseRectangle.color = "red"; addToDatabaseProgressBar.value = 0;
                                        //programmingRectangle.color = "red"; programmingProgressBar.value = 0;
                                        test232Rectangle.color = jsonData.test232.testResult === "finished" ? "green" : "red"; test232ProgressBar.value = 0;
                                        test485Rectangle.color = jsonData.test485.testResult === "finished" ? "green" : "red"; test485ProgressBar.value = 0;
                                        testFreqRectangle.color = jsonData.testFreq.testResult === "finished" ? "green" : "red"; testFreqProgressBar.value = 0;
                                        toast.displayMessage(qsTr("Test completed failed") + "\r\n" +
                                                             "rs232: tx " + jsonData.test232.sendPackets +
                                                             ", rx " + jsonData.test232.receivePackets + "\r\n" +
                                                             "rs485: tx " + jsonData.test485.sendPackets +
                                                             ", rx " + jsonData.test485.receivePackets + "\r\n" +
                                                             "cnt test: " +
                                                             "step1: " + jsonData.testFreq.capStep1 +
                                                             ", step2: " + jsonData.testFreq.capStep2 +
                                                             ", step3: " + jsonData.testFreq.capStep3, "bad");
                                    }
                                }
                            }

                            onSignalTestError: {
                                var jsonData = JSON.parse(json)
                                addToDatabaseRectangle.color = "red"; addToDatabaseProgressBar.value = 0;
                                programmingRectangle.color = "red"; programmingProgressBar.value = 0;
                                test232Rectangle.color = "red"; test232ProgressBar.value = 0;
                                test485Rectangle.color = "red"; test485ProgressBar.value = 0;
                                testFreqRectangle.color = "red"; testFreqProgressBar.value = 0;
                                toast.displayMessage(jsonData.message, "bad");
                            }

                            onSignalTestUpdateStatus: {
                                if(viewControl.stendRole == "firmare") {
                                    var jsonData = JSON.parse(json)
                                    if(jsonData.testStep === "programming") {
                                        addToDatabaseRectangle.color = "green"
                                        addToDatabaseProgressBar.value = parseInt(jsonData.percent)
                                        if(parseInt(jsonData.percent) < 100) {
                                            programmingRectangle.color = "yellow"
                                        } else {
                                            programmingRectangle.color = "green"
                                        }
                                        programmingProgressBar.value = parseInt(jsonData.percent)
                                    }
                                    if(jsonData.testStep === "waitTestNotEnd") {
                                        setTestIndicationg(jsonData.test232.testResult, test232ProgressBar, test232Rectangle)
                                        setTestIndicationg(jsonData.test485.testResult, test485ProgressBar, test485Rectangle)
                                        setTestIndicationg(jsonData.testFreq.testResult, testFreqProgressBar, testFreqRectangle)
                                    }
                                }
                            }
                        }
                    }
                    Button { id:llsStartTestButton;
                        Material.background: Material.Green;
                        Material.foreground: "white";
                        text: qsTr("Start test");
                        font.pointSize: 8
                        implicitHeight: 50;
                        implicitWidth: 150
                        focus: true
                        onClicked: {
                            if(stendProp.getStendIsConnected()) {
                                stendProp.startTest()
                                toast.flush()
                            } else {
                                toast.displayMessage(qsTr("You need to establish a connection"), "neutral");
                            }
                        }

                        Shortcut {
                            sequence: "Space"
                            onActivated: {
                                if(stendProp.getStendIsConnected()) {
                                    stendProp.startTest()
                                    toast.flush()
                                } else {
                                    toast.displayMessage(qsTr("You need to establish a connection"), "neutral");
                                }
                            }
                        }
                    }
                }
            }

            //-- test panel
            Pane {
                id:chartPanel
                Material.elevation: 6
                Layout.leftMargin: 10
                Layout.fillWidth: true
                Layout.fillHeight: true
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
        //-- mcu property
        Pane {
            id:mcuSnPanel
            Material.elevation: 6
            implicitWidth: firmwarePannelScroll.width
            Column {
                width: parent.width
                spacing: 30
                RowLayout {
                    spacing: 20
                    RowLayout {
                        Label { text: qsTr("MCU SN: ") }
                        Label {
                            id:llsMcuSnDeviceLabel; color: llsMcuSnDeviceLabel.text === "NA" ? "red" : "black" }
                    }
                    RowLayout {
                        Label { text: qsTr("DEVICE SN: ") }
                        Label { id:llsSnDeviceLabel; color: text == "------------" ? "red" : (llsSnDeviceLabel.text === "NA" || llsSnDeviceLabel.text == "000000000000" ? "red" : "black") }
                    }
                }
            }
        }
    }
}
