import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion

Button {
    property bool lightMode

    property bool b_useCheckedEffect: true
    property bool b_useDownEffect: true
    property bool b_useHoverEffect: true

    property alias p_contentItem_Text: buttonLabel
    property alias p_background_Rectangle: buttonBackground
    property alias p_background_Rectangle_gradient_Gradient: buttonBackgroundGradient
    property alias p_background_Rectangle_gradient_Gradient_GradientStop0: buttonBackgroundGradientStop0
    property alias p_background_Rectangle_gradient_Gradient_GradientStop1: buttonBackgroundGradientStop1

    id: pushButton

    hoverEnabled: true

    implicitWidth: 100
    implicitHeight: 36

    contentItem: Text {
        id: buttonLabel

        text: pushButton.text

        font.pixelSize: 10
        font.bold: true
        font.family: "Avenir Next"

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        color: {
            if (pushButton.lightMode) {
                if (!pushButton.enabled) return "#5c5a5a"
                if (pushButton.down && pushButton.b_useDownEffect) return "#2c3e50"
                if (pushButton.checked && pushButton.b_useCheckedEffect) return "#000000"
                if (pushButton.hovered && pushButton.b_useHoverEffect) return "#2c3e50"
                return "#2c3e50"
            }

            else {
                if (!pushButton.enabled) return "#cbcbcb"
                if (pushButton.down && pushButton.b_useDownEffect) return "#fffefe"
                if (pushButton.checked && pushButton.b_useCheckedEffect) return "#000000"
                if (pushButton.hovered && pushButton.b_useHoverEffect) return "#fffefe"
                return "#fffefe"
            }
        }
    }

    background: Rectangle {
        id: buttonBackground

        radius: 8

        border.width: 1
        border.color: {
            if (pushButton.lightMode) {
                if (!pushButton.enabled) return "#cce5f5"
                if (pushButton.down && pushButton.b_useDownEffect) return "#5ca6c8"
                if (pushButton.checked && pushButton.b_useCheckedEffect) return "#000000"
                if (pushButton.hovered && pushButton.b_useHoverEffect) return "#89c2e0"
                return "#a7d3f0"
            }

            else {
                if (!pushButton.enabled) return "#0d0d0d"
                if (pushButton.down && pushButton.b_useDownEffect) return "#555555"
                if (pushButton.checked && pushButton.b_useCheckedEffect) return "#000000"
                if (pushButton.hovered && pushButton.b_useHoverEffect) return "#333333"
                return "#1a1a1a"
            }
        }

        gradient: Gradient {
            id: buttonBackgroundGradient

            GradientStop {
                id: buttonBackgroundGradientStop0

                position: 0

                color: {
                    if (pushButton.lightMode) {
                        if (!pushButton.enabled) return "#e8f4fb"
                        if (pushButton.down && pushButton.b_useDownEffect) return "#89c2e0"
                        if (pushButton.checked && pushButton.b_useCheckedEffect) return "#000000"
                        if (pushButton.hovered && pushButton.b_useHoverEffect) return "#c0e0f0"
                        return "#c3e7fa"
                    }

                    else {
                        if (!pushButton.enabled) return "#0d0d0d"
                        if (pushButton.down && pushButton.b_useDownEffect) return "#555555"
                        if (pushButton.checked && pushButton.b_useCheckedEffect) return "#000000"
                        if (pushButton.hovered && pushButton.b_useHoverEffect) return "#333333"
                        return "#1a1a1a"
                    }
                }
            }

            GradientStop {
                id: buttonBackgroundGradientStop1

                position: 1

                color: {
                    if (pushButton.lightMode) {
                        if (!pushButton.enabled) return "#cce5f5"
                        if (pushButton.down && pushButton.b_useDownEffect) return "#a3d5f7"
                        if (pushButton.checked && pushButton.b_useCheckedEffect) return "#000000"
                        if (pushButton.hovered && pushButton.b_useHoverEffect) return "#89c2e0"
                        return "#a7d3f0"
                    }

                    else {
                        if (!pushButton.enabled) return "#cce5f5"
                        if (pushButton.down && pushButton.b_useDownEffect) return "#f0f0f0"
                        if (pushButton.checked && pushButton.b_useCheckedEffect) return "#000000"
                        if (pushButton.hovered && pushButton.b_useHoverEffect) return "#89c2e0"
                        return "#bedff5"
                    }
                }
            }
        }
    }
}
