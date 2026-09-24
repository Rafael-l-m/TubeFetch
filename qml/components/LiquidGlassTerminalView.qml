import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/*
 * Usage:
 *      enabled: true
 *
 *      Layout.fillWidth: true
 *      Layout.fillHeight: true
 */

Rectangle {
    property bool autoScroll: true
    property int maxLines: 10000

    property alias p_ListModel: terminalModel

    signal commandEntered(string command)

    id: terminalView

    radius: 16

    color: Qt.rgba(0.035, 0.045, 0.065, 0.78)

    border.width: 1
    border.color: Qt.rgba(1, 1, 1, 0.20)

    clip: true


    // ===============
    // Glass Base
    // ===============

    Rectangle {
        id: glassBase

        anchors.fill: parent

        radius: parent.radius

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Qt.rgba(1, 1, 1, 0.085)
            }

            GradientStop {
                position: 0.38
                color: Qt.rgba(1, 1, 1, 0.025)
            }

            GradientStop {
                position: 1.0
                color: Qt.rgba(0.15, 0.30, 0.55, 0.10)
            }
        }
    }


    // =================
    // Main Content
    // =================

    ColumnLayout {
        id: columnLayout

        anchors.fill: parent
        anchors.margins: 10

        spacing: 8


        // ===========
        // Output
        // ===========

        ListView {
            id: outputView

            Layout.fillWidth: true
            Layout.fillHeight: true

            model: terminalModel

            clip: true

            spacing: 1

            boundsBehavior: Flickable.StopAtBounds

            cacheBuffer: 2000

            function isAtBottom() { return contentHeight <= height || contentY >= contentHeight - height - 200 }

            function scrollToBottom() {
                if (!terminalView.autoScroll) { return }

                Qt.callLater(function() {
                    if (!terminalView.autoScroll) { return }
                    outputView.positionViewAtEnd()
                })
            }

            onDragStarted: { terminalView.autoScroll = false }

            onMovementEnded: { if (isAtBottom()) { terminalView.autoScroll = true } }

            onContentHeightChanged: { if (terminalView.autoScroll) { scrollToBottom() } }

            onHeightChanged: { if (terminalView.autoScroll) { scrollToBottom() } }

            delegate: Item {
                width: outputView.width
                height: type === "input" ? commandText.implicitHeight + 2 : terminalText.implicitHeight + 2


                // ==================
                // Terminal text
                // ==================

                Text {
                    id: terminalText

                    visible: type !== "input"

                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top

                    text: content
                    textFormat: Text.PlainText
                    wrapMode: Text.Wrap

                    font.family: "Menlo"
                    font.pixelSize: 10

                    color: {
                        switch (type) {
                            case "error":   return "#ff5f67"
                            case "warning": return "#ffb454"
                            case "info":    return "#5aa9ff"
                            case "output":  return "#ff4dff"
                            default:        return "#e7edf5"
                        }
                    }
                }


                // ====================
                // Command History
                // ====================

                Row {
                    id: commandRow

                    visible: type === "input"

                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top

                    spacing: 0

                    Text {
                        id: commandPrefix

                        text: ">> "

                        font.family: "Menlo"
                        font.pixelSize: 10

                        color: "#55d979"
                    }

                    Text {
                        id: commandText

                        width: Math.max(0, commandRow.width - commandPrefix.width)

                        text: content
                        textFormat: Text.PlainText
                        wrapMode: Text.Wrap

                        font.family: "Menlo"
                        font.pixelSize: 10

                        color: "#55d979"
                    }
                }
            }


            // ==============
            // ScrollBar
            // ==============

            ScrollBar.vertical: ScrollBar {
                id: scrollBar

                policy: ScrollBar.AsNeeded

                width: 7

                background: Rectangle {
                    id: scrollBar_backgroud_Rectangle

                    radius: 10

                    color: "transparent"
                }

                contentItem: Rectangle {
                    id: scrollBar_contentItem_Rectangle

                    radius: 10

                    implicitWidth: 6

                    color: scrollBar.pressed ? Qt.rgba(1, 1, 1, 0.38) : Qt.rgba(1, 1, 1, 0.18)

                    border.width: 1
                    border.color: Qt.rgba(1, 1, 1, 0.18)

                    visible: scrollBar.size < 1.0

                    opacity: 0.8

                    Behavior on opacity { NumberAnimation { duration: 150 } }
                }
            }
        }


        // ==================
        // Command Input
        // ==================

        Item {
            id: commandArea

            Layout.fillWidth: true
            Layout.preferredHeight: Math.min(Math.max(30, commandInput.contentHeight + 10), 120)

            Layout.minimumHeight: 30
            Layout.maximumHeight: 120


            // ===========================
            // Input Glass Background
            // ===========================

            Rectangle {
                id: inputGlass

                anchors.fill: parent

                radius: 12

                color: commandInput.activeFocus ? Qt.rgba(1, 1, 1, 0.075) : Qt.rgba(1, 1, 1, 0.045)

                border.width: 1
                border.color: commandInput.activeFocus ? Qt.rgba(0.35, 0.70, 1.0, 0.50) : Qt.rgba(1, 1, 1, 0.13)

                Behavior on color { ColorAnimation { duration: 160 } }
                Behavior on border.color { ColorAnimation { duration: 160 } }

                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: Qt.rgba(1, 1, 1, 0.09)
                    }

                    GradientStop {
                        position: 0.5
                        color: Qt.rgba(1, 1, 1, 0.025)
                    }

                    GradientStop {
                        position: 1.0
                        color: Qt.rgba(0.15, 0.35, 0.65, 0.07)
                    }
                }
            }


            // =========================
            // Input Top Reflection
            // =========================

            Rectangle {
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    margins: 1
                }

                height: parent.height * 0.45

                radius: 12

                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: Qt.rgba(1, 1, 1, 0.11)
                    }

                    GradientStop {
                        position: 1.0
                        color: Qt.rgba(1, 1, 1, 0.0)
                    }
                }
            }


            // ==================
            // Input Content
            // ==================

            RowLayout {
                id: commandArea_RowLayout

                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10

                spacing: 6

                Text {
                    id: commandArea_RowLayout_Text

                    text: ">>"

                    Layout.alignment: Qt.AlignVCenter

                    font.family: "Menlo"
                    font.pixelSize: 10
                    font.bold: true

                    color: "#55d979"
                }

                TextArea {
                    id: commandInput

                    readonly property bool isMultiline: commandInput.lineCount > 1

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    background: null

                    padding: 0

                    leftPadding: 0
                    rightPadding: 0
                    topPadding: !isMultiline ? 9 : 4

                    wrapMode: TextEdit.Wrap

                    font.bold: true
                    font.family: "Menlo"
                    font.pixelSize: 10

                    color: "#55d979"

                    selectionColor: "#536f89"
                    selectByMouse: true

                    Keys.onReturnPressed: { executeCommand() }

                    Keys.onEnterPressed: { executeCommand() }
                }
            }


            // ===============
            // Focus Glow
            // ===============

            Rectangle {
                anchors.fill: parent

                radius: 12

                color: "transparent"

                border.width: 1
                border.color: Qt.rgba(
                    0.30,
                    0.70,
                    1.0,
                    commandInput.activeFocus ? 0.35 : 0.0
                )

                opacity: commandInput.activeFocus ? 1.0 : 0.0

                Behavior on opacity { NumberAnimation { duration: 180 } }
            }
        }
    }


    // =================
    // Liquid Light
    // =================

    Rectangle {
        id: topLiquidGlow

        x: -width * 0.30
        y: -height * 0.55

        width: terminalView.width * 0.85
        height: terminalView.height * 0.85

        radius: width / 2

        color: Qt.rgba(0.35, 0.65, 1.0, 0.035)

        opacity: 0.8

        z: 10

        visible: terminalView.width > 0
    }

    Rectangle {
        id: bottomLiquidGlow

        x: terminalView.width - width * 0.65
        y: terminalView.height - height * 0.55

        width: terminalView.width * 0.70
        height: terminalView.height * 0.70

        radius: width / 2

        color: Qt.rgba(0.45, 0.25, 1.0, 0.025)

        opacity: 0.8

        z: 10
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
            margins: 1
        }

        height: Math.min(parent.height * 0.25, 90)

        radius: parent.radius

        z: 20

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Qt.rgba(1, 1, 1, 0.095)
            }

            GradientStop {
                position: 0.35
                color: Qt.rgba(1, 1, 1, 0.045)
            }

            GradientStop {
                position: 1.0
                color: Qt.rgba(1, 1, 1, 0.0)
            }
        }

        MouseArea {
            anchors.fill: parent
            enabled: false
        }
    }


    // ============================
    // Bottom Glass Reflection
    // ============================

    Rectangle {
        id: bottomReflection

        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 1
        }

        height: Math.min(parent.height * 0.15, 55)

        radius: parent.radius

        z: 20

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Qt.rgba(0.25, 0.45, 0.75, 0.0)
            }

            GradientStop {
                position: 1.0
                color: Qt.rgba(0.25, 0.45, 0.75, 0.055)
            }
        }

        MouseArea {
            anchors.fill: parent
            enabled: false
        }
    }


    // =======================
    // Glass Border - Top
    // =======================

    Rectangle {
        anchors.fill: parent

        radius: parent.radius

        color: "transparent"

        border.width: 1
        border.color: Qt.rgba(1, 1, 1, 0.20)

        z: 30

        MouseArea {
            anchors.fill: parent
            enabled: false
        }
    }


    // ===================
    // Terminal Model
    // ===================

    ListModel { id: terminalModel }


    // ====================
    // Limit Max Lines
    // ====================

    function trimTerminalModel() {
        if (maxLines <= 0) { return }

        const excess = terminalModel.count - maxLines

        if (excess > 0) { terminalModel.remove(0, excess) }
    }


    // ==============
    // Functions
    // ==============

    function addLine(text, type) {
        terminalModel.append({
            "content": text,
            "type": type || "debug"
        })

        trimTerminalModel()

        if (terminalView.autoScroll) { outputView.scrollToBottom() }
    }

    function addText(text, type) {
        const lines = text.split("\n")

        for (let i = 0; i < lines.length; ++i) {
            terminalModel.append({
                "content": lines[i],
                "type": type || "debug"
            })
        }

        trimTerminalModel()

        if (terminalView.autoScroll) { outputView.scrollToBottom() }
    }

    function addDebug(text) { addText(text, "debug") }
    function addInfo(text) { addText(text, "info") }
    function addWarning(text) { addText(text, "warning") }
    function addError(text) { addText(text, "error") }
    function addOutput(text) { addText(text, "output") }
    function addNewCommandLine(command) { addLine(command, "input") }

    function executeCommand() {
        let command = commandInput.text.trim()

        if (command.length === 0) { return }
        if (command.endsWith("\\")) { commandInput.insert(commandInput.length, "\n"); return }

        command = command.replace(/\\\r?\n/g, " ")
        command = command.replace(/\s+/g, " ")

        addNewCommandLine(command)

        commandInput.clear()

        commandEntered(command)
    }

    function clearCommandLines() { terminalModel.clear() }

    Component.onCompleted: { commandInput.forceActiveFocus() }
}
