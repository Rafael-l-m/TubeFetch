import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Label {
    property bool lightMode

    text: parent.title

    anchors.left: parent.left
    anchors.leftMargin: 12

    color: lightMode ? "#000000" : "#ffffff"

    font.pixelSize: 12
    font.bold: true
    font.family: "Avenir Next"
}
