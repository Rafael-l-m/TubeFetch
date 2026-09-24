import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components" as Comp

Rectangle {
    property bool alwaysOnAnimation: false

    signal editRequest(var idd)
    signal removeRequest(var idd)
    signal startRequest(var idd)
    signal stopRequest(var idd)

    id: downloadDelegate

    implicitHeight: 58

    radius: 12

    color: mouseArea.pressed
              ? Qt.rgba(1, 1, 1, 0.105)
              : mouseArea.containsMouse
                  ? Qt.rgba(1, 1, 1, 0.075)
                  : Qt.rgba(1, 1, 1, 0.045)

    border.width: 1
    border.color: mouseArea.containsMouse ? Qt.rgba(1, 1, 1, 0.28) : Qt.rgba(1, 1, 1, 0.12)

    Behavior on color { ColorAnimation { duration: 140 } }
    Behavior on border.color { ColorAnimation { duration: 160 } }


    // ==================================================
    // Subtle Glass Highlight
    // ==================================================

    Rectangle {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top

            leftMargin: 4.5
            rightMargin: 4.5
            topMargin: 3
        }

        height: parent.height * 0.85

        radius: parent.radius

        gradient: Gradient {
            GradientStop {
                position: 0
                color: Qt.rgba(1, 1, 1, 0.15)
            }

            GradientStop {
                position: 1
                color: Qt.rgba(1, 1, 1, 0.01)
            }
        }

        opacity: mouseArea.pressed ? 0.35 : 0.65

        enabled: false
    }


    // ============================
    // Bottom Glass Reflection
    // ============================

    Rectangle {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom

            leftMargin: 12
            rightMargin: 12
            bottomMargin: 2
        }

        height: 1

        radius: 1

        color: Qt.rgba(0.65, 0.8, 1.0, 0.13)

        enabled: false
    }


    // ============
    // Content
    // ============

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 9

        spacing: 6

        Row {
            id: rowDelegate

            Layout.fillWidth: true

            spacing: 15

            property real buttonWidth: 28
            property real buttonsWidth: buttonWidth * 4 + spacing * 3

            property real totalSpacing: spacing * 6
            property real availableWidth: rowDelegate.width - rowDelegate.totalSpacing - rowDelegate.buttonsWidth


            // ==========
            // Title
            // ==========

            Comp.MarqueeText {
                id: titleContainer

                width: rowDelegate.availableWidth * 0.24
                height: 30

                contentText: title

                infiniteAnimationWithoutPause: downloadDelegate.alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_Text.color: Qt.rgba(1, 1, 1, 0.94)
                p_Text.font.bold: true
                p_Text.font.pixelSize: 12

                p_SequentialAnimation.running: {
                    if (downloadDelegate.alwaysOnAnimation) { return p_Text.implicitWidth > titleContainer.width }
                    return mouseArea.containsMouse && p_Text.implicitWidth > titleContainer.width
                }
            }


            // =============
            // Progress
            // =============

            Item {
                width: rowDelegate.availableWidth * 0.2
                height: 30

                clip: true

                Rectangle {
                    anchors.fill: parent

                    radius: height / 2

                    color: Qt.rgba(0, 0, 0, 0.20)

                    border.width: 1
                    border.color: Qt.rgba(1, 1, 1, 0.08)
                }

                Rectangle {
                    id: progressContainer_ProgressBar

                    width: parent.width * progress / 100
                    height: parent.height

                    radius: height / 2

                    clip: true

                    gradient: Gradient {
                        orientation: Gradient.Horizontal

                        GradientStop {
                            position: 0
                            color: "#4facfe"
                        }

                        GradientStop {
                            position: 1
                            color: "#00f2fe"
                        }
                    }

                    Behavior on width {
                        NumberAnimation {
                            id: progressAnimation

                            duration: 550
                            easing.type: Easing.OutCubic

                            onStarted: { liquidTimer.restart() }
                        }
                    }

                    Rectangle {
                        id: liquidWave

                        width: parent.width * 0.30
                        height: parent.height * 1.6

                        y: -height * 0.3

                        radius: height / 2

                        color: Qt.rgba(1, 1, 1, 0.22)

                        opacity: 0

                        x: -width

                        Behavior on opacity { NumberAnimation { duration: 100 } }

                        NumberAnimation {
                            id: waveAnimation

                            target: liquidWave
                            property: "x"

                            from: -liquidWave.width
                            to: progressContainer_ProgressBar.width

                            duration: 500

                            easing.type: Easing.OutCubic
                        }
                    }

                    Timer {
                        id: liquidTimer

                        interval: 20
                        repeat: false

                        onTriggered: {
                            liquidWave.x = -liquidWave.width
                            liquidWave.opacity = 0.0

                            waveAnimation.stop()

                            liquidWave.opacity = 0.8
                            waveAnimation.start()
                        }
                    }
                }

                Text {
                    id: progressBarText

                    anchors.centerIn: parent

                    text: progress + "%"

                    color: Qt.rgba(1, 1, 1, 0.90)

                    font.pixelSize: 10
                    font.bold: true
                }
            }

            /*Item {
                width: rowDelegate.availableWidth * 0.2
                height: 30

                Rectangle {
                    anchors.fill: parent

                    radius: height / 2

                    color: Qt.rgba(0, 0, 0, 0.20)

                    border.width: 1
                    border.color: Qt.rgba(1, 1, 1, 0.08)
                }

                Rectangle {
                    width: parent.width * progress / 100
                    height: parent.height

                    radius: height / 2

                    gradient: Gradient {
                        id: gradient

                        orientation: Gradient.Horizontal

                        GradientStop {
                            id: gradientStop1
                            position: 0
                            color: "#4facfe"
                        }

                        GradientStop {
                            id: gradientStop2
                            position: 1
                            color: "#00f2fe"
                        }
                    }

                    Behavior on width {
                        SmoothedAnimation {
                            id: smoothedAnimation
                            velocity: 800
                        }
                    }
                }

                Text {
                    anchors.centerIn: parent

                    text: progress + "%"

                    color: Qt.rgba(1, 1, 1, 0.90)

                    font.pixelSize: 10
                    font.bold: true
                }
            }*/


            // =========
            // Size
            // =========

            Comp.MarqueeText {
                id: sizeTextContainer

                width: rowDelegate.availableWidth * 0.18
                height: 30

                contentText: downloadedBytes + "/" + totalBytes + " MiB"

                infiniteAnimationWithoutPause: downloadDelegate.alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_Text.color: Qt.rgba(1, 1, 1, 0.78)

                p_SequentialAnimation_NumberAnimation1.duration: titleContainer.p_SequentialAnimation_NumberAnimation1.duration
                p_SequentialAnimation_NumberAnimation2.duration: titleContainer.p_SequentialAnimation_NumberAnimation2.duration
                p_SequentialAnimation.running: mouseArea.containsMouse && p_Text.implicitWidth > sizeTextContainer.width
            }


            // ==========
            // Speed
            // ==========

            Comp.MarqueeText {
                id: speedContainer

                width: rowDelegate.availableWidth * 0.12
                height: 30

                contentText: speed + " KiB"

                infiniteAnimationWithoutPause: downloadDelegate.alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_Text.color: Qt.rgba(1, 1, 1, 0.78)

                p_SequentialAnimation_NumberAnimation1.duration: titleContainer.p_SequentialAnimation_NumberAnimation1.duration
                p_SequentialAnimation_NumberAnimation2.duration: titleContainer.p_SequentialAnimation_NumberAnimation2.duration
                p_SequentialAnimation.running: mouseArea.containsMouse && p_Text.implicitWidth > speedContainer.width
            }


            // ==============
            // Remaining
            // ==============

            Comp.MarqueeText {
                id: remainTextContainer

                width: rowDelegate.availableWidth * 0.12
                height: 30

                contentText: remain

                infiniteAnimationWithoutPause: downloadDelegate.alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_Text.color: Qt.rgba(1, 1, 1, 0.78)

                p_SequentialAnimation_NumberAnimation1.duration: titleContainer.p_SequentialAnimation_NumberAnimation1.duration
                p_SequentialAnimation_NumberAnimation2.duration: titleContainer.p_SequentialAnimation_NumberAnimation2.duration
                p_SequentialAnimation.running: mouseArea.containsMouse && p_Text.implicitWidth > remainTextContainer.width
            }


            // ===========
            // Status
            // ===========

            Comp.MarqueeText {
                id: statusContainer

                width: rowDelegate.availableWidth * 0.14
                height: 30

                contentText: status

                infiniteAnimationWithoutPause: downloadDelegate.alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_Text.color: Qt.rgba(1, 1, 1, 0.80)

                p_SequentialAnimation_NumberAnimation1.duration: titleContainer.p_SequentialAnimation_NumberAnimation1.duration
                p_SequentialAnimation_NumberAnimation2.duration: titleContainer.p_SequentialAnimation_NumberAnimation2.duration
                p_SequentialAnimation.running: mouseArea.containsMouse && p_Text.implicitWidth > statusContainer.width
            }

            Comp.LiquidGlassActionButton {
                width: rowDelegate.buttonWidth
                height: 30

                symbol: "✎"

                toolTipText: qsTr("Edit")

                onClicked: { downloadDelegate.editRequest(internalId) }
            }

            Comp.LiquidGlassActionButton {
                width: rowDelegate.buttonWidth
                height: 30

                symbol: "×"

                toolTipText: qsTr("Remove")

                destructive: true

                onClicked: { downloadDelegate.removeRequest(internalId) }
            }

            Comp.LiquidGlassActionButton {
                width: rowDelegate.buttonWidth
                height: 30

                symbol: "▶"

                toolTipText: qsTr("Start download")

                onClicked: { downloadDelegate.startRequest(internalId) }
            }

            Comp.LiquidGlassActionButton {
                width: rowDelegate.buttonWidth
                height: 30

                symbol: "■"

                toolTipText: qsTr("Stop download")

                onClicked: { downloadDelegate.stopRequest(internalId) }
            }
        }
    }


    // ====================
    // Main Mouse Area
    // ====================

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        hoverEnabled: true

        propagateComposedEvents: true
    }
}
