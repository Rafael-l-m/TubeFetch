import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

TextField {
    property bool lightMode

    property color c_cursorColor: textField.lightMode ? "#000000" : "#ffffff"
    property real r_cursorWidth: 1

    property alias p_background_Rectangle: textFieldBackground

    id: textField

    implicitWidth: 500
    implicitHeight: 40

    color: textField.lightMode
                ? (!textField.readOnly ? "#000000" : "#f8f8f8")
                : "#f8f8f8"

    font.pixelSize: 12
    font.bold: true

    selectByMouse: true

    background: Rectangle {
        id: textFieldBackground

        border.color: "#dedede"
        border.width: 1

        radius: 6

        color: textField.lightMode
                    ? (!textField.readOnly ? "#c9eaf3" : "#686868")
                    : (!textField.readOnly ? "#aaaaaa" : "#1f1f1f")
    }

    cursorDelegate: Rectangle {
        width: r_cursorWidth

        color: c_cursorColor

        visible: textField.activeFocus
    }
}
