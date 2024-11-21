import QtQuick 2.9

Canvas {
    id: canvas
    antialiasing: true

    required property variant values
    onValuesChanged: canvas.requestPaint()

    onPaint: {
        const ctx = getContext("2d");
        ctx.moveTo(0, 0);
        ctx.reset();

        ctx.fillStyle = "#1e293b"
        ctx.fillRect(0, 0, width, height)

        ctx.beginPath();

        const maxItemSize = width / 10;
        const slicedValues = maxItemSize < values.length
            ? values.slice(0).slice(-maxItemSize)
            : values
        for (let i = 0; i < slicedValues.length; i++) {
            ctx.lineTo(i * 10, (1 - slicedValues[i]) * canvas.height);
        }
        ctx.strokeStyle = palette.text
        ctx.stroke();
    }
}
