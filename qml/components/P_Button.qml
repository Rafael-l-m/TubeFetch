import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion

Button {
    property bool lightMode

    property alias p_root: button
    property alias p_contentItem_Text: label_Btn
    property alias p_background_Rectangle: background_Rectangle
    property alias p_background_Rectangle_gradient_Gradient: gradient_Gradient
    property alias p_background_Rectangle_gradient_Gradient_gradientStop0: gradientStop0
    property alias p_background_Rectangle_gradient_Gradient_gradientStop1: gradientStop1

    property bool b_useCheckedEffect: true
    property bool b_useDownEffect: true
    property bool b_useHoverEffect: true

    id: button

    hoverEnabled: true

    implicitWidth: 100
    implicitHeight: 36

    contentItem:
        Text {
            id: label_Btn

            text: button.text

            font.pixelSize: 11
            font.bold: true
            font.family: "Avenir Next"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            color: {
                if (button.lightMode) {
                    if (!button.enabled) return "#5c5a5a"
                    if (button.down && button.b_useDownEffect) return "#2c3e50"
                    if (button.checked && button.b_useCheckedEffect) return "#000000"
                    if (button.hovered && button.b_useHoverEffect) return "#2c3e50"
                    return "#2c3e50"
                }

                else {
                    if (!button.enabled) return "#cbcbcb"
                    if (button.down && button.b_useDownEffect) return "#fffefe"
                    if (button.checked && button.b_useCheckedEffect) return "#000000"
                    if (button.hovered && button.b_useHoverEffect) return "#fffefe"
                    return "#fffefe"
                }
            }
        }

    background: Rectangle {
        id: background_Rectangle

        radius: 8

        border.width: 1

        border.color: {
            if (lightMode) {
                if (!button.enabled) return "#cce5f5"
                if (button.down && button.b_useDownEffect) return "#5ca6c8"
                if (button.checked && button.b_useCheckedEffect) return "#000000"
                if (button.hovered && button.b_useHoverEffect) return "#89c2e0"
                return "#a7d3f0"
            }

            else {
                if (!button.enabled) return "#0d0d0d"
                if (button.down && button.b_useDownEffect) return "#555555"
                if (button.checked && button.b_useCheckedEffect) return "#000000"
                if (button.hovered && button.b_useHoverEffect) return "#333333"
                return "#1a1a1a"
            }
        }

        gradient: Gradient {
            id: gradient_Gradient

            GradientStop {
                id: gradientStop0
                position: 0

                color: {
                    if (button.lightMode) {
                        if (!button.enabled) return "#e8f4fb"
                        if (button.down && button.b_useDownEffect) return "#89c2e0"
                        if (button.checked && button.b_useCheckedEffect) return "#000000"
                        if (button.hovered && button.b_useHoverEffect) return "#c0e0f0"
                        return "#c3e7fa"
                    }

                    else {
                        if (!button.enabled) return "#0d0d0d"
                        if (button.down && button.b_useDownEffect) return "#555555"
                        if (button.checked && button.b_useCheckedEffect) return "#000000"
                        if (button.hovered && button.b_useHoverEffect) return "#333333"
                        return "#1a1a1a"
                    }
                }
            }

            GradientStop {
                id: gradientStop1
                position: 1

                color: {
                    if (button.lightMode) {
                        if (!button.enabled) return "#cce5f5"
                        if (button.down && button.b_useDownEffect) return "#a3d5f7"
                        if (button.checked && button.b_useCheckedEffect) return "#000000"
                        if (button.hovered && button.b_useHoverEffect) return "#89c2e0"
                        return "#a7d3f0"
                    }

                    else {
                        if (!button.enabled) return "#cce5f5"
                        if (button.down && button.b_useDownEffect) return "#a3d5f7"
                        if (button.checked && button.b_useCheckedEffect) return "#000000"
                        if (button.hovered && button.b_useHoverEffect) return "#89c2e0"
                        return "#a7d3f0"
                    }
                }
            }
        }
    }
}
