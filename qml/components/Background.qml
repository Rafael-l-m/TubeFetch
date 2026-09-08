import QtQuick
import QtQuick.Effects

Item {
    id: backgroundContent

    anchors.fill: parent

    // =================================
    // Animated Gradient Background
    // =================================

    Rectangle {
        anchors.fill: parent

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#1a1a2e" }
            GradientStop { position: 0.5; color: "#16213e" }
            GradientStop { position: 1.0; color: "#0f3460" }
        }
    }

    // ===============================
    // Animated Decorative Shapes
    // ===============================

    Repeater {
        model: 8

        Rectangle {
            id: shape

            property real baseX: Math.random() * parent.width
            property real baseY: Math.random() * parent.height
            property real animPhase: Math.random() * Math.PI * 2
            property real animSpeed: 0.5 + Math.random() * 1.0

            x: baseX + Math.sin(animTime * animSpeed + animPhase) * 50
            y: baseY + Math.cos(animTime * animSpeed * 0.7 + animPhase) * 40

            width: 100 + Math.random() * 200
            height: width
            radius: width / 2

            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: Qt.hsla(0.55 + index * 0.05, 0.7, 0.5, 0.4)
                }

                GradientStop {
                    position: 1.0
                    color: Qt.hsla(0.65 + index * 0.05, 0.6, 0.4, 0.2)
                }
            }

            layer.enabled: true

            layer.effect: MultiEffect {
                blurEnabled: true
                blur: 0.6
                blurMax: 32
            }
        }
    }

    Rectangle {
        x: parent.width * 0.7 + Math.sin(animTime * 0.6) * 60
        y: parent.height * 0.2 + Math.cos(animTime * 0.5) * 50

        width: 300
        height: 300
        radius: 150

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Qt.rgba(0.9, 0.3, 0.5, 0.5)
            }

            GradientStop {
                position: 1.0
                color: Qt.rgba(0.9, 0.5, 0.3, 0.2)
            }
        }

        layer.enabled: true

        layer.effect: MultiEffect {
            blurEnabled: true
            blur: 0.5
            blurMax: 48
        }
    }

    Rectangle {
        x: parent.width * 0.1 + Math.sin(animTime * 0.8 + 2) * 50
        y: parent.height * 0.6 + Math.cos(animTime * 0.6 + 2) * 40

        width: 250
        height: 250
        radius: 125

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Qt.rgba(0.3, 0.7, 0.9, 0.5)
            }

            GradientStop {
                position: 1.0
                color: Qt.rgba(0.3, 0.5, 0.9, 0.2)
            }
        }

        layer.enabled: true

        layer.effect: MultiEffect {
            blurEnabled: true
            blur: 0.5
            blurMax: 48
        }
    }

    // =============================
    // Additional Accent Shapes
    // =============================

    Rectangle {
        x: parent.width * 0.7
        y: parent.height * 0.2

        width: 300
        height: 300
        radius: 150

        gradient: Gradient {
            GradientStop { position: 0.0; color: Qt.rgba(0.9, 0.3, 0.5, 0.5) }
            GradientStop { position: 1.0; color: Qt.rgba(0.9, 0.5, 0.3, 0.2) }
        }

        layer.enabled: true
        layer.effect: MultiEffect { blurEnabled: true; blur: 0.5; blurMax: 48 }
    }

    Rectangle {
        x: parent.width * 0.1
        y: parent.height * 0.6

        width: 250
        height: 250
        radius: 125

        gradient: Gradient {
            GradientStop { position: 0.0; color: Qt.rgba(0.3, 0.7, 0.9, 0.5) }
            GradientStop { position: 1.0; color: Qt.rgba(0.3, 0.5, 0.9, 0.2) }
        }

        layer.enabled: true
        layer.effect: MultiEffect { blurEnabled: true; blur: 0.5; blurMax: 48 }
    }

    // ==========================================
    // Animation Timer for Background Shapes
    // ==========================================

    property real animTime: 0

    Timer {
        running: true
        repeat: true
        interval: 16
        onTriggered: animTime += 0.012
    }
}
