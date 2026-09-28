import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Rectangle {
    readonly property real defaultPageWidth: 400
    readonly property real defaultPageHeight: 500

    property real toastWidth: toast.defaultPageWidth
    property real toastHeight: toast.defaultPageHeight
    property real toastWidthScale: toast.toastWidth / toast.defaultPageWidth
    property real toastHeightScale: toast.toastHeight / toast.defaultPageHeight
    property real maxWidth: parent ? parent.width * 0.8 : toast.defaultPageWidth - 100

    id: toast

    color: "#f0f0f0"

    radius: 8

    opacity: 0

    z: 999

    width: maxWidth

    anchors.horizontalCenter: parent ? parent.horizontalCenter : undefined
    anchors.bottom: parent ? parent.bottom : undefined
    anchors.bottomMargin: 20 * toast.toastHeightScale

    property real hiddenY: anchors.bottomMargin + 20 * toast.toastHeightScale

    Text {
        id: toastText

        anchors {
            left: parent.left
            right: parent.right

            leftMargin: 20
            rightMargin: 20

            verticalCenter: parent.verticalCenter
        }

        color: "#000000"

        font.bold: true
        font.pixelSize: 14

        wrapMode: Text.Wrap

        horizontalAlignment: Text.AlignHCenter

        maximumLineCount: 10

        elide: Text.ElideRight
    }

    height: toastText.paintedHeight + 40

    SequentialAnimation {
        id: toastAnimation

        ParallelAnimation {
            PropertyAnimation { target: toast; property: "opacity"; to: 1; duration: 400 }
            PropertyAnimation { target: toast; property: "y"; to: parent.height - toast.height - anchors.bottomMargin; duration: 200; easing.type: Easing.OutCubic }
        }

        PauseAnimation { duration: 2500 }

        ParallelAnimation {
            PropertyAnimation { target: toast; property: "opacity"; to: 0; duration: 1000 }
            PropertyAnimation { target: toast; property: "y"; to: parent.height + hiddenY; duration: 300; easing.type: Easing.InCubic }
        }
    }

    function show(message) {
        toastText.text = message
        toast.y = parent.height + hiddenY
        toastAnimation.start()
    }
}

/*
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Rectangle {
    property bool lightMode: true

    readonly property real defaultPageWidth: 400
    readonly property real defaultPageHeight: 500

    property real toastWidth: toast.defaultPageWidth
    property real toastHeight: toast.defaultPageHeight
    property real toastWidthScale: toast.toastWidth / toast.defaultPageWidth
    property real toastHeightScale: toast.toastHeight / toast.defaultPageHeight

    property real maxWidth: parent ? parent.width * 0.8 : toast.defaultPageWidth - 100

    id: toast

    width: maxWidth
    height: toastText.paintedHeight + 40

    radius: height / 2

    z: 999

    opacity: 0

    anchors.horizontalCenter: parent ? parent.horizontalCenter : undefined
    anchors.bottom: parent ? parent.bottom : undefined
    anchors.bottomMargin: 20 * toast.toastHeightScale

    property real hiddenY: anchors.bottomMargin + 20 * toast.toastHeightScale

    color: lightMode ? Qt.rgba(1.0, 1.0, 1.0, 0.13) : Qt.rgba(0.12, 0.12, 0.15, 0.38)

    border.width: 1
    border.color: lightMode ? Qt.rgba(1, 1, 1, 0.42) : Qt.rgba(1, 1, 1, 0.20)


    // ====================
    // Soft Outer Glow
    // ====================

    Rectangle {
        id: outerGlow

        z: -1

        anchors.fill: parent
        anchors.margins: -3

        radius: parent.radius + 3

        color: "transparent"

        border.width: 4
        border.color: lightMode ? Qt.rgba(1, 1, 1, 0.07) : Qt.rgba(0.7, 0.8, 1.0, 0.06)

        opacity: toast.opacity
    }


    // ===============================
    // Main Liquid-Glass Gradient
    // ================================

    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: lightMode ? Qt.rgba(1, 1, 1, 0.20) : Qt.rgba(1, 1, 1, 0.13)
        }

        GradientStop {
            position: 0.38
            color: lightMode ? Qt.rgba(1, 1, 1, 0.09) : Qt.rgba(1, 1, 1, 0.07)
        }

        GradientStop {
            position: 0.72
            color: lightMode ? Qt.rgba(0.85, 0.90, 1.0, 0.07) : Qt.rgba(0.45, 0.55, 0.75, 0.08)
        }

        GradientStop {
            position: 1.0
            color: lightMode ? Qt.rgba(0.70, 0.80, 1.0, 0.045) : Qt.rgba(0.30, 0.40, 0.65, 0.10)
        }
    }


    // =========================
    // Top Glass Reflection
    // =========================

    Rectangle {
        id: topReflection

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
                color: lightMode ? Qt.rgba(1, 1, 1, 0.28) : Qt.rgba(1, 1, 1, 0.14)
            }

            GradientStop {
                position: 0.45
                color: lightMode ? Qt.rgba(1, 1, 1, 0.08) : Qt.rgba(1, 1, 1, 0.05)
            }

            GradientStop {
                position: 1.0
                color: Qt.rgba(1, 1, 1, 0.0)
            }
        }

        opacity: 0.9
    }


    // ==========================
    // Inner Glass Highlight
    // ==========================

    Rectangle {
        anchors {
            fill: parent

            leftMargin: 1
            rightMargin: 1
            topMargin: 1
            bottomMargin: 1
        }

        radius: parent.radius - 1

        color: "transparent"

        border.width: 1
        border.color: lightMode ? Qt.rgba(1, 1, 1, 0.12) : Qt.rgba(1, 1, 1, 0.07)
    }


    // ===========================
    // Bottom Refraction Line
    // ===========================

    Rectangle {
        id: bottomRefraction

        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom

            leftMargin: 18
            rightMargin: 18
            bottomMargin: 3
        }

        height: 2

        radius: 1

        color: lightMode ? Qt.rgba(0.65, 0.80, 1.0, 0.20) : Qt.rgba(0.55, 0.70, 1.0, 0.16)

        opacity: 0.8
    }


    // ====================
    // Small Top Shine
    // ====================

    Rectangle {
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top

            topMargin: 2
        }

        width: parent.width * 0.45
        height: 1

        radius: 1

        color: Qt.rgba(1, 1, 1, 0.22)

        opacity: 0.8
    }


    // =========
    // Text
    // =========

    Text {
        id: toastText

        anchors {
            left: parent.left
            right: parent.right

            leftMargin: 20
            rightMargin: 20

            verticalCenter: parent.verticalCenter
        }

        color: lightMode ? Qt.rgba(0.05, 0.05, 0.08, 0.90) : Qt.rgba(1, 1, 1, 0.94)

        font.bold: true
        font.pixelSize: 14

        wrapMode: Text.Wrap

        horizontalAlignment: Text.AlignHCenter

        maximumLineCount: 10

        elide: Text.ElideRight

        style: Text.Raised

        styleColor: lightMode ? Qt.rgba(1, 1, 1, 0.35) : Qt.rgba(0, 0, 0, 0.30)
    }


    // ==========================
    // Show / Hide animation
    // ==========================

    SequentialAnimation {
        id: toastAnimation

        ParallelAnimation {
            id: parallelAnimation1

            PropertyAnimation {
                id: propertyAnimation11

                target: toast

                property: "opacity"

                from: 0
                to: 1

                duration: 400

                easing.type: Easing.OutCubic
            }

            PropertyAnimation {
                id: propertyAnimation12

                target: toast

                property: "y"

                to: parent.height
                    - toast.height
                    - anchors.bottomMargin

                duration: 350

                easing.type: Easing.OutCubic
            }

            PropertyAnimation {
                target: toast

                property: "scale"

                from: 0.96
                to: 1.0

                duration: 350

                easing.type: Easing.OutCubic
            }
        }

        PauseAnimation { id: pauseAnimation; duration: 2500 }

        ParallelAnimation {
            id: parallelAnimation2

            PropertyAnimation {
                id: propertyAnimation21

                target: toast

                property: "opacity"

                to: 0

                duration: 350

                easing.type: Easing.InCubic
            }

            PropertyAnimation {
                id: propertyAnimation22

                target: toast

                property: "y"

                to: parent.height + hiddenY

                duration: 300

                easing.type: Easing.InCubic
            }

            PropertyAnimation {
                target: toast

                property: "scale"

                to: 0.96

                duration: 300

                easing.type: Easing.InCubic
            }
        }
    }

    function show(message) {
        toastText.text = message

        toast.opacity = 0
        toast.scale = 0.96
        toast.y = parent.height + hiddenY

        toastAnimation.restart()
    }
}
*/
