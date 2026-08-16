import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Label {
    property bool lightMode

    font.pixelSize: 12
    font.bold: true
    font.family: "Avenir Next"

    color: lightMode ? "#000000" : "#ffffff"
}
