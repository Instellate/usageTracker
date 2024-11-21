import QtQuick 2.6
import QtQuick.Controls
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: root
    width: 480
    height: 360
    visible: true
    color: "#111827"
    palette.text: "white"

    property color primaryColor: "#29b6f6"
    required property int refreshRate

    RowLayout {
        anchors.fill: parent

        TabBar {
            id: statsTypeBar
            Layout.preferredWidth: 100

            contentItem: ListView {
                model: statsTypeBar.contentModel
                currentIndex: statsTypeBar.currentIndex

                spacing: statsTypeBar.spacing
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

            TabButton {
                text: "Processes"
                width: 100
            }

            TabButton {
                text: "Performance"
                width: 100
            }
        }

        StackLayout {
            currentIndex: statsTypeBar.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true

            Item {
                Text {
                    color: palette.text
                    text: "Not yet implemented"
                }
            }

            PerformanceTab {
                refreshRate: root.refreshRate
            }
        }
    }

}
