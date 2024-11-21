import QtQuick 2.9
import QtQuick.Layouts 6.6

Item {
    id: root
    anchors.fill: parent

    required property variant values

    onValuesChanged: canvas.requestPaint()

    RowLayout {
        anchors.fill: root
        Canvas {
            id: canvas
            Layout.fillHeight: true
            Layout.fillWidth: true

            onPaint: {
                const ctx = getContext("2d");
                ctx.moveTo(0, 0);
                ctx.reset();

                ctx.beginPath();
                ctx.lineTo(50, 50);

                ctx.beginPath();

                const maxItemSize = width / 10;
                const slicedValues = maxItemSize < values.length
                    ? values.slice(0).slice(-maxItemSize)
                    : values
                for (let i = 0; i < slicedValues.length; i++) {
                    ctx.lineTo(i * 10, (1 - slicedValues[i]) * canvas.height);
                }
                ctx.strokeStyle = "black"
                ctx.stroke();
            }
        }
    }

}
