import QtQuick
import QtQuick.Controls

Item {
    property alias text: input.text
    property alias placeholderText: input.placeholderText
    property alias readOnly: input.readOnly
    property alias echoMode: input.echoMode
    property alias font: input.font
    property alias cursorPosition: input.cursorPosition
    property alias inputMethodHints: input.inputMethodHints

    property bool hovered: mouseDetector.containsMouse
    property bool focused: input.activeFocus
    property bool enabled: true

    signal accepted()
    signal textEdited(string text)

    id: liquidGlassTextField

    implicitWidth: 260
    implicitHeight: 52

    property bool editableState: !readOnly && enabled


    // ===============
    // Outer Glow
    // ===============

    Rectangle {
        id: outerGlow

        anchors.fill: glass
        anchors.margins: liquidGlassTextField.focused ? -4 : -2

        radius: glass.radius + 4

        color: "transparent"

        border.width: liquidGlassTextField.focused ? 3 : 2
        border.color: liquidGlassTextField.readOnly
                        ? Qt.rgba(1, 1, 1, 0.035)
                        : liquidGlassTextField.focused
                            ? Qt.rgba(0.45, 0.72, 1.0, 0.38)
                            : liquidGlassTextField.hovered
                                ? Qt.rgba(1, 1, 1, 0.12)
                                : Qt.rgba(1, 1, 1, 0.045)

        opacity: liquidGlassTextField.focused
                    ? 1
                    : liquidGlassTextField.hovered && !liquidGlassTextField.readOnly
                        ? 0.8
                        : 0.45

        Behavior on border.color { ColorAnimation { duration: 180 } }

        Behavior on opacity { NumberAnimation { duration: 180 } }

        Behavior on anchors.margins {
            NumberAnimation {
                duration: 180
                easing.type: Easing.OutCubic
            }
        }
    }


    // =========================
    // Main Glass Container
    // =========================

    Rectangle {
        id: glass

        anchors.fill: parent

        radius: 14

        color: liquidGlassTextField.readOnly
                    ? Qt.rgba(0.85, 0.90, 1.0, 0.045)
                    : liquidGlassTextField.focused
                        ? Qt.rgba(1, 1, 1, 0.12)
                        : Qt.rgba(1, 1, 1, 0.075)

        border.width: liquidGlassTextField.focused ? 1.5 : 1
        border.color: liquidGlassTextField.readOnly
                        ? Qt.rgba(1, 1, 1, 0.14)
                        : liquidGlassTextField.focused
                            ? Qt.rgba(0.65, 0.82, 1.0, 0.55)
                            : liquidGlassTextField.hovered
                                ? Qt.rgba(1, 1, 1, 0.35)
                                : Qt.rgba(1, 1, 1, 0.20)

        Behavior on color { ColorAnimation { duration: 160 } }

        Behavior on border.color { ColorAnimation { duration: 180 } }

        gradient: Gradient {
            GradientStop {
                position: 0
                color: liquidGlassTextField.readOnly
                            ? Qt.rgba(0.85, 0.90, 1.0, 0.07)
                            : liquidGlassTextField.focused
                                ? Qt.rgba(1, 1, 1, 0.20)
                                : Qt.rgba(1, 1, 1, 0.15)
            }

            GradientStop {
                position: 0.5
                color: liquidGlassTextField.readOnly
                            ? Qt.rgba(0.6, 0.7, 0.85, 0.025)
                            : Qt.rgba(1, 1, 1, 0.055)
            }

            GradientStop {
                position: 1
                color: liquidGlassTextField.readOnly
                            ? Qt.rgba(0.55, 0.65, 0.8, 0.025)
                            : liquidGlassTextField.focused
                                ? Qt.rgba(0.45, 0.70, 1.0, 0.10)
                                : Qt.rgba(0.7, 0.8, 1.0, 0.035)
            }
        }


        // =========================
        // Top Glass Reflection
        // =========================

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top

                leftMargin: 10
                rightMargin: 10
                topMargin: 3
            }

            height: parent.height * 0.38

            radius: height / 2

            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: liquidGlassTextField.readOnly
                                ? Qt.rgba(1, 1, 1, 0.10)
                                : Qt.rgba(1, 1, 1, liquidGlassTextField.focused ? 0.28 : 0.20)
                }

                GradientStop {
                    position: 1
                    color: Qt.rgba(1, 1, 1, 0.01)
                }
            }

            opacity: liquidGlassTextField.readOnly ? 0.55 : 1
        }


        // ======================
        // Bottom Refraction
        // ======================

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom

                leftMargin: 12
                rightMargin: 12
                bottomMargin: 3
            }

            height: 2

            radius: 1

            color: liquidGlassTextField.readOnly
                        ? Qt.rgba(0.65, 0.75, 0.90, 0.08)
                        : liquidGlassTextField.focused
                            ? Qt.rgba(0.55, 0.78, 1.0, 0.30)
                            : Qt.rgba(0.65, 0.80, 1.0, 0.16)
        }


        // =====================
        // ReadOnly Overlay
        // =====================

        Rectangle {
            anchors.fill: parent

            radius: parent.radius

            color: liquidGlassTextField.readOnly
                        ? Qt.rgba(0.12, 0.16, 0.22, 0.08)
                        : "transparent"

            Behavior on color { ColorAnimation { duration: 180 } }
        }
    }


    // ==================
    // ReadOnly Icon
    // ==================

    Text {
        id: readonlyIcon

        anchors {
            left: parent.left

            leftMargin: liquidGlassTextField.readOnly ? 14 : 0

            verticalCenter: parent.verticalCenter
        }

        text: "◈"

        font.pixelSize: 13

        color: Qt.rgba(0.75, 0.84, 1.0, 0.48)

        opacity: liquidGlassTextField.readOnly ? 1 : 0

        scale: liquidGlassTextField.readOnly ? 1 : 0.7

        Behavior on opacity { NumberAnimation { duration: 180 } }

        Behavior on scale {
            NumberAnimation {
                duration: 220

                easing.type: Easing.OutBack
            }
        }
    }


    // =====================
    // Actual TextField
    // =====================

    TextField {
        id: input

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom

            leftMargin: liquidGlassTextField.readOnly ? 38 : 16

            rightMargin: 16
        }

        background: null

        verticalAlignment: TextInput.AlignVCenter

        color: liquidGlassTextField.readOnly
                    ? Qt.rgba(1, 1, 1, 0.62)
                    : Qt.rgba(1, 1, 1, 0.96)

        placeholderTextColor: liquidGlassTextField.readOnly
                                ? Qt.rgba(1, 1, 1, 0.28)
                                : Qt.rgba(1, 1, 1, 0.42)

        selectionColor: Qt.rgba(0.35, 0.65, 1.0, 0.55)

        selectedTextColor: Qt.white

        cursorDelegate: Rectangle {
            width: 2

            radius: 1

            color: Qt.rgba(0.75, 0.88, 1.0, 0.9)
        }

        cursorVisible: !liquidGlassTextField.readOnly && input.activeFocus

        font.pixelSize: 16

        font.weight: liquidGlassTextField.readOnly ? Font.Medium : Font.Normal

        enabled: liquidGlassTextField.enabled

        onAccepted: liquidGlassTextField.accepted()

        onTextEdited: function() { liquidGlassTextField.textEdited(input.text) }
    }


    // ====================
    // Hover Detection
    // ====================

    MouseArea {
        id: mouseDetector

        anchors.fill: parent

        hoverEnabled: true

        acceptedButtons: Qt.NoButton

        propagateComposedEvents: true

        z: -1
    }


    // =======================
    // Focus Liquid Pulse
    // =======================

    SequentialAnimation {
        id: focusPulse

        running: liquidGlassTextField.focused && !liquidGlassTextField.readOnly

        loops: Animation.Infinite

        NumberAnimation {
            target: focusLiquid

            property: "opacity"

            from: 0.18
            to: 0.38

            duration: 1600

            easing.type: Easing.InOutSine
        }

        NumberAnimation {
            target: focusLiquid

            property: "opacity"

            from: 0.38
            to: 0.18

            duration: 1600

            easing.type: Easing.InOutSine
        }
    }


    // =====================================================
    // Focus Liquid Layer
    // =====================================================

    Rectangle {
        id: focusLiquid

        anchors {
            fill: parent
            margins: 2
        }

        radius: glass.radius - 2

        color: Qt.rgba(0.35, 0.65, 1.0, 0.08)

        opacity: 0

        visible: liquidGlassTextField.focused && !liquidGlassTextField.readOnly

        z: -1
    }
}
