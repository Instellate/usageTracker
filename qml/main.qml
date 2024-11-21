import QtQuick 2.6
import QtQuick.Controls
import QtQuick.Layouts 1.1
import UsageTracker 1.0

ApplicationWindow {
    id: root
    width: 480
    height: 360
    visible: true
    color: "white"

    property color primaryColor: "#29b6f6"
    property variant timeSeries: []
    required property int refreshRate

    CpuTimer {
        id: timer
        refreshRate: root.refreshRate
        onUsageChanged: root.timeSeries = [...root.timeSeries, usage]
    }

    TimeChart {
        id: chart
        values: root.timeSeries
    }

    ColumnLayout {
        anchors.centerIn: chart

        Text {
            text: "CPU usage"
            Layout.alignment: Qt.AlignCenter
        }
        Text {
            text: `${Math.round(timer.usage * 1000) / 10}%`
            Layout.alignment: Qt.AlignCenter
        }
    }
}