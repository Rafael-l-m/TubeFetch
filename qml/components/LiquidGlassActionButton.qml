import QtQuick
import QtQuick.Controls

/*
 * Usage:
 *      width: 30
 *      height: 30
 *      symbol: " "
 *      toolTipText: qsTr(" ")
 *      onClicked: { }
 */

Item {
    property string symbol: ""
    property string toolTipText: ""
    property bool destructive: false

    signal clicked()

    id: liquidGlassActionButton

    scale: mouseArea.pressed ? 0.88 : mouseArea.containsMouse ? 1.08 : 1.0

    Behavior on scale {
        NumberAnimation {
            duration: 120
            easing.type: Easing.OutCubic
        }
    }


    // =====================
    // Glass Background
    // =====================

    Rectangle {
        id: background

        anchors.fill: parent

        radius: width / 2

        color: destructive
                  ? (mouseArea.containsMouse
                        ? Qt.rgba(1, 0.20, 0.25, 0.16)
                        : Qt.rgba(1, 0.20, 0.25, 0.07))
                  : (mouseArea.containsMouse
                        ? Qt.rgba(1, 1, 1, 0.15)
                        : Qt.rgba(1, 1, 1, 0.075))

        border.width: 1
        border.color: destructive
                        ? Qt.rgba(1, 0.35, 0.40, mouseArea.containsMouse ? 0.40 : 0.18)
                        : Qt.rgba(1, 1, 1, mouseArea.containsMouse ? 0.34 : 0.16)

        Behavior on color { ColorAnimation { duration: 120 } }
        Behavior on border.color { ColorAnimation { duration: 140 } }


        // ===================
        // Top reflection
        // ===================

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top

                leftMargin: 3
                rightMargin: 3
                topMargin: 2
            }

            height: parent.height * 0.40

            radius: height / 2

            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: Qt.rgba(1, 1, 1, 0.18)
                }

                GradientStop {
                    position: 1
                    color: Qt.rgba(1, 1, 1, 0)
                }
            }

            enabled: false
        }
    }


    // ===========
    // Symbol
    // ===========

    Text {
        anchors.centerIn: parent

        text: liquidGlassActionButton.symbol

        color: liquidGlassActionButton.destructive ? Qt.rgba(1, 0.65, 0.68, 0.95) : Qt.rgba(1, 1, 1, 0.88)

        font.pixelSize: liquidGlassActionButton.symbol === "×" ? 19 : 13
        font.weight: Font.Medium

        style: Text.Raised
        styleColor: Qt.rgba(0, 0, 0, 0.20)

        enabled: false
    }


    // ==========
    // Mouse
    // ==========

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        hoverEnabled: true

        cursorShape: Qt.PointingHandCursor

        onClicked: { liquidGlassActionButton.clicked() }
    }


    // ============
    // Tooltip
    // ============

    ToolTip {
        visible: mouseArea.containsMouse && liquidGlassActionButton.toolTipText !== ""

        text: liquidGlassActionButton.toolTipText

        delay: 500

        timeout: 2500
    }
}
