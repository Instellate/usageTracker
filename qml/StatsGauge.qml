import QtQuick 2.9

Item {
    id: root

    property int size: 150
    property int lineWidth: 5
    property real value: 0

    property color primaryColor: "#29b6f6"
    property color secondaryColor: "#e0e0e0"

    width: size
    height: size

    onValueChanged: canvas.degree = value * 270;

    Canvas {
        id: canvas

        property real degree: 0

        anchors.fill: parent
        antialiasing: true

        onDegreeChanged: requestPaint();

        onPaint: {
            const ctx = getContext("2d");

            const x = root.width / 2;
            const y = root.height / 2;

            const radius = root.size / 2 - root.lineWidth;
            const startAngle = (Math.PI / 180) * 135;
            const fullAngle = (Math.PI / 180) * (360 + 45);
            const progressAngle = (Math.PI / 180) * (135 + degree);

            ctx.reset()

            ctx.lineCap = "round";
            ctx.lineWidth = root.lineWidth;

            ctx.beginPath();
            ctx.arc(x, y, radius, startAngle, fullAngle);
            ctx.strokeStyle = root.secondaryColor;
            ctx.stroke();

            ctx.beginPath();
            ctx.arc(x, y, radius, startAngle, progressAngle);
            ctx.strokeStyle = root.primaryColor;
            ctx.stroke();
        }

        // Behavior on degree {
        //     NumberAnimation {
        //         duration: root.animationDuration
        //     }
        // }
    }
}
