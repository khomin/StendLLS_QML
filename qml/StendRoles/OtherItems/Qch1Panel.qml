import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtCharts 2.3
import Settings 1.0

SwipeView {
    id:qch1PannelScroll
    Layout.fillWidth: true
    Layout.fillHeight: true
    clip: true
    interactive: false

    Connections {
        target: viewControl
        onSignalUpdateRealTimeData: {
            if(viewControl.stendRole == "qch1") {
                var jsonData = JSON.parse(json)
                llsPowerVoltageLabel.text = jsonData.power_input.toFixed(2)
                llsPowerCurrentLabel.text = jsonData.power_current.toFixed(1)
                llsFreqLabel.text = jsonData.freq
                llsTempLabel.text = jsonData.temp.toFixed(2)

                if(jsonData.mcu_serial_number !== "303030303030303030303030") {
                    llsMcuSnLabel.text = jsonData.mcu_serial_number
                } else {
                    llsMcuSnLabel.text = "NA";
                }

                llsSnLabel.text = jsonData.serial_number

                drawChartLine(chart, chartVoltageLine, jsonData.powerCollect);
                drawChartLine(chart, chartCurrentLine, jsonData.currentCollect);
                drawChartLine(chart, chartCntLine, jsonData.cntCollect);

                if(busyIndicator.visible == true) {
                    busyIndicator.visible = false;
                }
            }
        }
        onSignalDataBaseError: {
            if(viewControl.stendRole == "qch1") {
                toast.displayMessage(err, "bad")
            }
        }
        onGoodMessage: {
            if(viewControl.stendRole == "qch1") {
                toast.displayMessage(text, "good")
            }
        }
        onBadMessage: {
            if(viewControl.stendRole == "qch1") {
                toast.displayMessage(text, "bad")
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

    Connections {
        target: qrScaner
        onQrCodeError: {
            if(viewControl.stendRole == "qch1") {
                toast.displayMessage(message, "bad");
            }
        }
        onQrCodeUpdateSerialNum: {}
    }

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
                        Label{ text: qsTr("Tests") + " " + selectLlsTestType.activeProfile;
                            font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
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
                                    color: "black"
                                }
                                Label { text: "v"}
                            }

                            Label {
                                text: qsTr("Power current:")
                            }
                            RowLayout {
                                Label { id:llsPowerCurrentLabel
                                    text: "0.0"
                                    color: stendQchDecision.powerCurrValid ? "black" : "red"
                                }
                                Label { text: "mA"}
                            }

                            Label {
                                text: qsTr("Frequency:")
                            }
                            RowLayout {
                                Label { id:llsFreqLabel
                                    text: "0"
                                    color: stendQchDecision.levelValid ? "black" : "red"
                                }
                                Label { text: "Hz"}
                            }

                            Label {
                                text: qsTr("MCU SN:")
                            }
                            Label { id:llsMcuSnLabel
                                text: "NA"
                                color: stendQchDecision.mcuSnValid ? "black" : "red"
                            }

                            Label {
                                text: qsTr("SN:")
                            }
                            Label { id:llsSnLabel
                                text: "NA"
                                color: stendQchDecision.snValid ? "black" : "red"
                            }

                            Label { text: qsTr("Temperature:") }
                            RowLayout {
                                Label { id:llsTempLabel; text: "NA"
                                    color: stendQchDecision.tempValid ? "black" : "red" }
                                Label { text: "°C" }
                            }

                            Label { text: qsTr("RS485:") }
                            Rectangle {
                                id:llsRs485StatusRectangle
                                width: 32; height: 32
                                color: stendQchDecision.rs485IsNormal ? "green" : "red"
                            }

                            Label { text: qsTr("RS232:") }
                            Rectangle {
                                id:llsRs232StatusRectangle
                                width: 32; height: 32
                                color: stendQchDecision.rs232IsNormal ? "green" : "red"
                            }

                            Label { text: "Empty:" }
                            Label { id:llsEmptyValueLabel;
                                text:stendQchDecision.levelEmptyValue
                                color: stendQchDecision.levelEmptyTriggered ? "black" : "red" }

                            Label { text: "Full:" }
                            Label { id:llsfullValueLabel;
                                text:stendQchDecision.levelFullValue
                                color: stendQchDecision.levelFullTriggered ? "black" : "red" }
                        }

                        ColumnLayout {
                            spacing: 10
                            RowLayout {
                                spacing: 10
                                Button {
                                    id:llsWriteTestButton;
                                    Material.background: Material.Green;
                                    Material.foreground: "white";
                                    text: qsTr("Save test");
                                    enabled: qrScaner.isValid
                                             && stendProp.isConnected
                                             && stendQchDecision.levelEmptyTriggered
                                             && stendQchDecision.levelFullTriggered
                                             && llsSnLabel.text == "------------"
                                             && llsMcuSnLabel.text != "NA"
                                    icon.source:"qrc:/svg/resources/fonts/svgs/solid/pen.svg"
                                    icon.width: 16; icon.height: 16
                                    font.pointSize: 8
                                    implicitHeight: 50
                                    onClicked: {
                                        stendProp.saveTestLlsToDb(llsMcuSnLabel.text)
                                    }
                                }
                                Button {
                                    id:llsMarkAsDefectiveButton;
                                    Material.background: Material.Green;
                                    Material.foreground: "white";
                                    text: qsTr("Mark as defective");
                                    enabled: stendQchDecision.mcuSnValid && stendProp.isConnected
                                    icon.source:"qrc:/svg/resources/fonts/svgs/solid/trash.svg"
                                    icon.width: 16; icon.height: 16
                                    font.pointSize: 8
                                    implicitHeight: 50
                                    onClicked: {
                                        var mcuSn = llsMcuSnLabel.text
                                        var qrCode = qrScaner.getQrCode();
                                        var result = []
                                        if(mcuSn !== "NA" && qrCode !== "NA") {
                                            stendProp.markLlsAsDefective(mcuSn, qrCode, JSON.stringify(result))
                                        }
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
