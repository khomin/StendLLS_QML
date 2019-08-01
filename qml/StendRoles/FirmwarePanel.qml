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

    Connections {
        target: viewControl
        onSignalUpdateRealTimeData: {
            var jsonData = JSON.parse(json)
            llsPowerVoltageLabel.text = jsonData.power_input.toFixed(2)
            llsPowerCurrentLabel.text = jsonData.power_current.toFixed(2)
            llsCntLabel.text = jsonData.cnt
            llsMcuSnDeviceLabel.text = jsonData.mcu_serial_number
            llsMcuSnLabel.text = jsonData.mcu_serial_number
            llsSnDeviceLabel.text = jsonData.serial_number

            drawChart(jsonData.powerCollect, chartVoltageLine, chartVoltage)
            drawChart(jsonData.currentCollect, chartCurrentLine, chartCurrent)
            drawChart(jsonData.cntCollect, chartCapacityLine, chartCapacity)

            if(busyIndicator.visible == true) {
                busyIndicator.visible = false;
            }
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
                implicitWidth: 290
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

                        Rectangle {
                            width: 32; height: 32
                            color: "red"
                        }
                        Label {
                            text: qsTr("Add device to database")
                        }
                        Rectangle {
                            width: 32; height: 32
                            color: "red"
                        }
                        Label {
                            text: qsTr("Test 232")
                        }
                        Rectangle {
                            width: 32; height: 32
                            color: "red"
                        }
                        Label {
                            text: qsTr("Test 485")
                        }
                        Rectangle {
                            width: 32; height: 32
                            color: "red"
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
                        RowLayout {
                            Label { id:llsMcuSnLabel
                                text: "---"
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
                        onClicked: {

                        }
                    }
                }
            }

            //-- test panel
            Pane {
                Material.elevation: 6
                Layout.leftMargin: 10
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop

                Column {
                    id:chartTestColumn
                    width: parent.width
                    Label{ text: qsTr("Diagrams"); font.pointSize: 8; color: Material.color(Material.Green, Material.Shade800)}
                    spacing: 10
                    ChartView {
                        id: chartVoltage
                        theme: ChartView.ChartThemeLight
                        title: "Voltage"
                        antialiasing: true
                        width: chartTestColumn.width;
                        height: (firmwarePannelScroll.height / 3 - 50)
                        backgroundColor: "transparent"
                        property int graphLength: 1
                        property int graphAmplitudeMax: 1
                        ValueAxis {
                            id: chartVoltageAxisX
                            min: -0.5
                            max: chartVoltage.graphLength
                            tickCount: 5
                        }
                        ValueAxis {
                            id: chartVoltageAxisY
                            min: -0.5
                            max: chartVoltage.graphAmplitudeMax
                            tickCount: 5
                        }
                        LineSeries {
                            id: chartVoltageLine
                            axisX: chartVoltageAxisX
                            axisY: chartVoltageAxisY
                            color: "blue"
                        }
                    }
                    ChartView {
                        id: chartCurrent
                        theme: ChartView.ChartThemeLight
                        title: "Current"
                        antialiasing: true
                        width: chartTestColumn.width;
                        height: (firmwarePannelScroll.height / 3 - 50)
                        property int graphLength: 1
                        property int graphAmplitudeMax: 1
                        backgroundColor: "transparent"
                        property var chartCurrentLine
                        ValueAxis {
                            id: chartCurrentAxisX
                            min: -0.5
                            max: chartCurrent.graphLength
                            tickCount: 5
                        }
                        ValueAxis {
                            id: chartCurrentChartAxisY
                            min: -0.5
                            max: chartCurrent.graphAmplitudeMax
                            tickCount: 5
                        }
                        LineSeries {
                            id: chartCurrentLine
                            axisX: chartCurrentAxisX
                            axisY: chartCurrentChartAxisY
                            color: "red"
                        }
                    }
                    ChartView {
                        id: chartCapacity
                        theme: ChartView.ChartThemeLight
                        title: "Frequency"
                        antialiasing: true
                        width: chartTestColumn.width; height: (firmwarePannelScroll.height / 3 - 50)
                        property int graphLength: 1
                        property int graphAmplitudeMax: 1
                        backgroundColor: "transparent"
                        property var chartCapacityLine
                        ValueAxis {
                            id: chartCapacityAxisX
                            min: -0.5
                            max: chartCapacity.graphLength
                            tickCount: 5
                        }
                        ValueAxis {
                            id: chartCapacityAxisY
                            min: -0.5
                            max: chartCapacity.graphAmplitudeMax
                            tickCount: 5
                        }
                        LineSeries {
                            id: chartCapacityLine
                            axisX: chartCapacityAxisX
                            axisY: chartCapacityAxisY
                            color: "orange"
                        }
                    }
                }
            }
        }
        //-- mcu property
        Pane {
            id:mcuSnPanel
            Material.elevation: 6
            implicitWidth: firmwarePannelScroll.width - 20
            Column {
                width: parent.width
                spacing: 30
                RowLayout {
                    spacing: 20
                    RowLayout {
                        Label { text: qsTr("MCU SN: "); color: llsMcuSnDeviceLabel.text.length === 0 ? "red" : "black" }
                        Label {
                            id:llsMcuSnDeviceLabel
                            text: ""
                        }
                    }
                    RowLayout {
                        Label { text: qsTr("DEVICE SN: "); color: llsSnDeviceLabel.text.length === 0 ? "red" : "black" }
                        Label {
                            id:llsSnDeviceLabel
                            text: ""
                            color: text == "ТЕСТОВ" ? "red" : "black"
                        }
                    }
                }
            }
        }
    }
}
