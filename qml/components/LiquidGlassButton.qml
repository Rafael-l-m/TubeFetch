import QtQuick

Item {
    property string text: ""
    property bool checked: false
    property bool hovered: mouseArea.containsMouse
    property bool pressed: mouseArea.pressed

    signal clicked()

    id: liquidGlassButton

    implicitWidth: 180
    implicitHeight: 58

    scale: pressed ? 0.96 : (hovered ? 1.02 : 1.0)

    Behavior on scale {
        NumberAnimation {
            duration: 160
            easing.type: Easing.OutCubic
        }
    }

    // ====================
    // Outer Soft Glow
    // ====================

    Rectangle {
        id: outerGlow

        anchors.fill: glass
        anchors.margins: -3

        radius: glass.radius + 3
        color: "transparent"

        border.width: 4
        border.color: mouseArea.containsMouse
                        ? Qt.rgba(1, 1, 1, 0.10)
                        : Qt.rgba(1, 1, 1, 0.04)

        opacity: mouseArea.pressed ? 0.4 : 1.0

        Behavior on border.color { ColorAnimation { duration: 180 } }
    }

    // ===============
    // Main Glass
    // ===============

    Rectangle {
        id: glass

        anchors.fill: parent

        radius: height / 2

        color: mouseArea.pressed
                ? Qt.rgba(1, 1, 1, 0.11)
                : Qt.rgba(1, 1, 1, 0.08)

        border.width: 1
        border.color: Qt.rgba(1, 1, 1, mouseArea.containsMouse ? 0.38 : 0.22)

        Behavior on color { ColorAnimation { duration: 120 } }

        Behavior on border.color { ColorAnimation { duration: 180 } }

        // ===================================
        // Central Subtle Liquid Gradient
        // ===================================

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: mouseArea.pressed
                        ? Qt.rgba(1, 1, 1, 0.10)
                        : Qt.rgba(1, 1, 1, 0.20)
            }

            GradientStop {
                position: 0.42
                color: Qt.rgba(1, 1, 1, 0.07)
            }

            GradientStop {
                position: 1.0
                color: Qt.rgba(0.7, 0.8, 1.0, 0.035)
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

                leftMargin: 12
                rightMargin: 12
                topMargin: 4
            }

            height: parent.height * 0.38
            radius: height / 2

            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: Qt.rgba(1, 1, 1, 0.24)
                }

                GradientStop {
                    position: 1.0
                    color: Qt.rgba(1, 1, 1, 0.01)
                }
            }

            opacity: mouseArea.pressed ? 0.45 : 0.8

            Behavior on opacity { NumberAnimation { duration: 120 } }
        }

        // =============================
        // Bottom Refraction Effect
        // =============================

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom

                leftMargin: 15
                rightMargin: 15
                bottomMargin: 3
            }

            height: 2
            radius: 1

            color: Qt.rgba(0.65, 0.8, 1.0, 0.18)
        }

        // =================================================
        // Ripple Click Effect
        // =================================================

        Rectangle {
            id: ripple

            width: 20
            height: 20

            radius: width / 2

            x: liquidGlassButton.mouseXPos - width / 2
            y: liquidGlassButton.mouseYPos - height / 2

            color: Qt.rgba(1, 1, 1, 0.22)

            opacity: 0

            scale: 0.1

            ParallelAnimation {
                id: rippleAnimation

                NumberAnimation {
                    target: ripple

                    property: "scale"

                    from: 0.1
                    to: 8

                    duration: 550

                    easing.type: Easing.OutCubic
                }

                NumberAnimation {
                    target: ripple

                    property: "opacity"

                    from: 0.30
                    to: 0

                    duration: 550
                }
            }
        }

        Text {
            anchors.centerIn: parent

            text: liquidGlassButton.text

            color: liquidGlassButton.pressed ? Qt.rgba(1, 1, 1, 0.62) : Qt.rgba(1, 1, 1, 0.96)

            font.pixelSize: liquidGlassButton.pressed ? 15 : 16
            font.weight: liquidGlassButton.pressed
                            ? Font.Bold
                            : (liquidGlassButton.hovered
                                ? Font.DemiBold
                                : Font.Medium)

            style: Text.Raised
            styleColor: Qt.rgba(0, 0, 0, 0.22)

            Behavior on scale {
                NumberAnimation {
                    duration: 120
                    easing.type: Easing.OutCubic
                }
            }

            Behavior on color { ColorAnimation { duration: 120 } }
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        hoverEnabled: true
        cursorShape: Qt.UpArrowCursor

        onClicked: liquidGlassButton.clicked()
    }
}
