import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.TabButton {
    id: control

    property color bgColor: "#4b5563"
    property color hoverBgColor: "#374151"

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
        implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
        implicitContentHeight + topPadding + bottomPadding)

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text

        color: "white"
        font: control.font
    }

    background: Rectangle {
        implicitHeight: 30
        color: control.hovered ? control.bgColor : control.hoverBgColor
    }
}