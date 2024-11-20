import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import tracker

const primaryColor = '#29b6f6';

ApplicationWindow {
    id: root
    width: 480
    height: 360
    visible: true
    color: "white"

    CpuTimer {
        id: timer
    }

    StatsGauge {
        id: gauge
        value: timer.usage
        anchors.centerIn: parent
        primaryColor: primaryColor
    }

    ColumnLayout {
        anchors.centerIn: gauge

        Text {
            color: primaryColor
            text: "CPU usage"
            Layout.alignment: Qt.AlignCenter
        }
        Text {
            color: primaryColor
            text: `${Math.round(timer.usage * 1000) / 10}%`
            Layout.alignment: Qt.AlignCenter
        }
    }
}