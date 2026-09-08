import QtQuick

Item {
    property bool checked: false
    property bool hovered: mouseArea.containsMouse
    property bool pressed: mouseArea.pressed

    signal toggled(bool checked)

    id: liquidGlassSwitch

    scale: pressed ? 0.97 : 1.0

    implicitWidth: 72
    implicitHeight: 40

    Behavior on scale {
        NumberAnimation {
            duration: 160
            easing.type: Easing.OutCubic
        }
    }


    // ===============
    // Outer Glow
    // ===============

    Rectangle {
        id: outerGlow

        anchors.fill: track
        anchors.margins: -3

        radius: track.radius + 3

        color: "transparent"

        border.width: 3

        border.color: liquidGlassSwitch.checked
                        ? Qt.rgba(0.55, 0.80, 1.0, 0.18)
                        : Qt.rgba(1, 1, 1, liquidGlassSwitch.hovered ? 0.10 : 0.04)

        opacity: liquidGlassSwitch.pressed ? 0.45 : 1.0

        Behavior on border.color { ColorAnimation { duration: 220 } }

        Behavior on opacity { NumberAnimation { duration: 120 } }
    }


    // ================
    // Glass Track
    // ================

    Rectangle {
        id: track

        anchors.fill: parent

        radius: height / 2

        color: liquidGlassSwitch.checked
                    ? Qt.rgba(0.30, 0.60, 1.0, 0.18)
                    : Qt.rgba(1, 1, 1, 0.08)

        border.width: 1

        border.color: liquidGlassSwitch.checked
                        ? Qt.rgba(0.60, 0.82, 1.0, 0.40)
                        : Qt.rgba(1, 1, 1, liquidGlassSwitch.hovered ? 0.38 : 0.22)

        Behavior on color { ColorAnimation { duration: 240 } }

        Behavior on border.color { ColorAnimation { duration: 220 } }


        // ===============================
        // Liquid Background Gradient
        // ===============================

        gradient: Gradient {
            GradientStop {
                position: 0
                color: liquidGlassSwitch.checked
                            ? Qt.rgba(0.45, 0.72, 1.0, 0.25)
                            : Qt.rgba(1, 1, 1, 0.18)
            }

            GradientStop {
                position: 0.5
                color: liquidGlassSwitch.checked
                            ? Qt.rgba(0.20, 0.50, 1.0, 0.16)
                            : Qt.rgba(1, 1, 1, 0.06)
            }

            GradientStop {
                position: 1
                color: liquidGlassSwitch.checked
                            ? Qt.rgba(0.60, 0.82, 1.0, 0.14)
                            : Qt.rgba(0.70, 0.80, 1.0, 0.035)
            }
        }


        // ===================
        // Top Reflection
        // ===================

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top

                leftMargin: 8
                rightMargin: 8
                topMargin: 3
            }

            height: parent.height * 0.38

            radius: height / 2

            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: Qt.rgba(1, 1, 1, liquidGlassSwitch.checked ? 0.22 : 0.18)
                }

                GradientStop {
                    position: 1
                    color: Qt.rgba(1, 1, 1, 0.01)
                }
            }
        }


        // ======================
        // Bottom Refraction
        // ======================

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom

                leftMargin: 10
                rightMargin: 10
                bottomMargin: 3
            }

            height: 2

            radius: 1

            color: liquidGlassSwitch.checked
                        ? Qt.rgba(0.65, 0.85, 1.0, 0.28)
                        : Qt.rgba(0.65, 0.80, 1.0, 0.14)
        }
    }


    // =======================
    // Active Liquid Glow
    // =======================

    Rectangle {
        id: activeGlow

        anchors {
            fill: track
            margins: 5
        }

        radius: height / 2

        color: Qt.rgba(0.35, 0.65, 1.0, 0.18)

        opacity: liquidGlassSwitch.checked ? 1 : 0

        Behavior on opacity { NumberAnimation { duration: 250 } }
    }


    // ==========
    // Thumb
    // ==========

    Rectangle {
        id: thumb

        width: liquidGlassSwitch.height - 8

        height: width

        y: (liquidGlassSwitch.height - height) / 2

        property real leftX: 4
        property real rightX: liquidGlassSwitch.width - width - 4

        x: liquidGlassSwitch.checked ? rightX : leftX

        radius: height / 2

        color: liquidGlassSwitch.checked
                    ? Qt.rgba(0.85, 0.94, 1.0, 0.32)
                    : Qt.rgba(1, 1, 1, 0.22 )

        border.width: 1

        border.color: Qt.rgba(1, 1, 1, liquidGlassSwitch.checked ? 0.55 : 0.38)

        Behavior on x {
            NumberAnimation {
                duration: 320
                easing.type: Easing.OutBack
            }
        }

        Behavior on color { ColorAnimation { duration: 220 } }


        // ====================
        // Thumb Highlight
        // ====================

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top

                leftMargin: 5
                rightMargin: 5
                topMargin: 3
            }

            height: parent.height * 0.40

            radius: height / 2

            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: Qt.rgba(1, 1, 1, 0.42)
                }

                GradientStop {
                    position: 1
                    color: Qt.rgba(1, 1, 1, 0.02)
                }
            }
        }


        // =============================
        // Bottom Liquid Refraction
        // =============================

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom

                leftMargin: 6
                rightMargin: 6
                bottomMargin: 3
            }

            height: 2

            radius: 1

            color: Qt.rgba(0.70, 0.85, 1.0, 0.22)
        }
    }


    // =================
    // Click Ripple
    // =================

    Rectangle {
        id: ripple

        width: 18
        height: 18

        radius: width / 2

        color: Qt.rgba(1, 1, 1, 0.25)

        opacity: 0

        scale: 0.1

        x: mouseArea.mouseX - width / 2
        y: mouseArea.mouseY - height / 2

        ParallelAnimation {
            id: rippleAnimation

            NumberAnimation {
                target: ripple

                property: "scale"

                from: 0.1
                to: 5

                duration: 450

                easing.type: Easing.OutCubic
            }

            NumberAnimation {
                target: ripple

                property: "opacity"

                from: 0.25
                to: 0

                duration: 450
            }
        }
    }


    // ================
    // Interaction
    // ================

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        hoverEnabled: true

        cursorShape: Qt.PointingHandCursor

        onClicked: {
            liquidGlassSwitch.checked = !liquidGlassSwitch.checked

            liquidGlassSwitch.toggled(liquidGlassSwitch.checked)

            rippleAnimation.restart()
        }
    }
}
