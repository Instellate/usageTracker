import QtQuick 2.6
import QtQuick.Controls
import QtQuick.Layouts 6.6
import UsageTracker 1.0
import "./Charts"

RowLayout {
    id: root
    required property int refreshRate
    property variant timeSeries: []
    property variant coresTimeSeries: []

    Layout.fillWidth: true
    Layout.fillHeight: true

    CpuTimer {
        id: timer
        refreshRate: root.refreshRate
        // onUsageChanged: root.timeSeries = [...root.timeSeries, usage]
        onPerCoreUsageChanged: {
            const newArr = [];
            for (let i = 0; i < perCoreUsage.length; i++) {
                const newSeries = [...(coresTimeSeries[i] ?? []), perCoreUsage[i]];
                newArr.push(newSeries);
            }
            coresTimeSeries = newArr;
        }
    }

    TabBar {
        id: performanceBar
        Layout.preferredWidth: 75
        Layout.fillHeight: true

        contentItem: ListView {
            model: performanceBar.contentModel
            currentIndex: performanceBar.currentIndex

            spacing: performanceBar.spacing
            orientation: ListView.Vertical
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.AutoFlickIfNeeded
            snapMode: ListView.SnapToItem
            verticalLayoutDirection: ListView.TopToBottom

            highlightMoveDuration: 0
            highlightRangeMode: ListView.ApplyRange
            preferredHighlightBegin: 40
            preferredHighlightEnd: width - 40

        }

        UsageTabButton {
            text: "CPU"
            width: 75
        }

        UsageTabButton {
            text: "Memory"
            width: 75
        }
    }

    StackLayout {
        currentIndex: performanceBar.currentIndex
        Layout.fillWidth: true
        Layout.fillHeight: true

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            GridLayout {
                id: gridLayout

                width: parent.width < Layout.maximumWidth ? parent.width : Layout.maximumWidth
                Layout.maximumWidth: itemMaxHeight * 2 * columns
                Layout.maximumHeight: parent.height

                property real itemMaxHeight: parent.height / rows - ((columnSpacing / 2) * columns)

                property int items: 12
                columns: Math.ceil(Math.sqrt(items))
                rows: Math.ceil(items / columns)
                anchors.centerIn: parent

                uniformCellHeights: true
                uniformCellWidths: true

                Repeater {
                    model: parent.items

                    TimeChart {
                        property real aspectRatio: 2 / 1
                        required property int index

                        Layout.maximumWidth: Layout.maximumHeight * aspectRatio
                        Layout.maximumHeight:
                            gridLayout.parent.height / gridLayout.rows
                            - ((gridLayout.columnSpacing / 2) * gridLayout.columns)

                        Layout.fillWidth: true
                        implicitWidth: height * aspectRatio
                        implicitHeight: width / aspectRatio
                        width: implicitHeight * aspectRatio


                        values: root.coresTimeSeries[index] ?? []
                    }
                }
            }
        }
    }
}