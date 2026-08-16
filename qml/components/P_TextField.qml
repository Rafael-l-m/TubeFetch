import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

TextField {
    property bool lightMode

    property alias p_root: textField
    property alias p_rectangle_Rectangle: rectangle_Rectangle

    id: textField

    implicitWidth: 500
    implicitHeight: 40

    color: lightMode ? "#000000" : "#f8f8f8"

    font.pixelSize: 12
    font.bold: true
    font.family: "Avenir Next"

    background: Rectangle {
        id: rectangle_Rectangle

        border.color: "#dedede"
        border.width: 1

        radius: 6

        color: lightMode ? (enabled ? "#c9eaf3" : "#686868") : (enabled ? "#aaaaaa" : "#1f1f1f")
    }
}
