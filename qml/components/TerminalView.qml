import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Rectangle {
    property bool lightMode

    property alias p_ColumnLayout: columnLayout
    property alias p_ColumnLayout_ListView: outputView
    property alias p_ColumnLayout_ListView_ScrollBar: scrollBar
    property alias p_ColumnLayout_ListView_ScrollBar_background_Rectangle: scrollBar_backgroud_Rectangle
    property alias p_ColumnLayout_ListView_ScrollBar_contentItem_Rectangle: scrollBar_contentItem_Rectangle
    property alias p_ColumnLayout_Item: commandArea
    property alias p_ColumnLayout_Item_RowLayout: commandArea_RowLayout
    property alias p_ColumnLayout_Item_RowLayout_Text: commandArea_RowLayout_Text
    property alias p_ColumnLayout_Item_RowLayout_TextArea: commandInput
    property alias p_ListModel: terminalModel

    property bool b_autoScroll: true
    property int i_maxLines: 100000

    signal commandEntered(string command)

    id: terminalView

    radius: 8

    color: terminalView.lightMode ? "#c9eaf3" : Qt.rgba(0, 0, 0, 0.15)

    border.width: 1
    border.color: terminalView.lightMode ? "#d0d0d0" : "#686868"

    Component.onCompleted: { commandInput.forceActiveFocus() }

    ColumnLayout {
        id: columnLayout

        anchors.fill: parent
        anchors.margins: 8

        spacing: 0

        ListView {
            id: outputView

            Layout.fillWidth: true
            Layout.fillHeight: true

            model: terminalModel

            clip: true

            spacing: 0

            boundsBehavior: Flickable.StopAtBounds

            cacheBuffer: 2000

            function isAtBottom() { return contentHeight <= height || contentY >= contentHeight - height - 60 }

            function scrollToBottom() {
                if (!terminalView.b_autoScroll) { return }

                Qt.callLater(function() {
                    if (!terminalView.b_autoScroll) { return }
                    outputView.positionViewAtEnd()
                })
            }

            onDragStarted: { terminalView.b_autoScroll = false }

            onMovementEnded: { if (isAtBottom()) { terminalView.b_autoScroll = true } }

            onContentHeightChanged: { if (terminalView.b_autoScroll) { scrollToBottom() } }

            onHeightChanged: { if (terminalView.b_autoScroll) { scrollToBottom() } }

            delegate: Item {
                width: outputView.width
                height: type === "input"
                        ? commandText.implicitHeight + 2
                        : terminalText.implicitHeight + 2

                // ========================================
                // Terminal
                // ========================================
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
                            case "error": return "#f44336"

                            case "warning": return "#ff9800"

                            case "info": return "#2196f3"

                            case "output": return "#fb00ff"

                            default: return terminalView.lightMode ? "#000000" : "#ffffff"
                        }
                    }
                }

                // ========================================
                // Instructions from command
                // ========================================
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
                        font.bold: false

                        color: "#4caf50"
                    }

                    Text {
                        id: commandText

                        width: Math.max(0, commandRow.width - commandPrefix.width)

                        text: content

                        textFormat: Text.PlainText

                        wrapMode: Text.Wrap

                        font.family: "Menlo"
                        font.pixelSize: 10

                        color: "#4caf50"
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                id: scrollBar

                policy: ScrollBar.AsNeeded

                width: 6

                background: Rectangle {
                    id: scrollBar_backgroud_Rectangle

                    radius: 12

                    color: "transparent"
                }

                contentItem: Rectangle {
                    id: scrollBar_contentItem_Rectangle

                    radius: 12

                    color: terminalView.lightMode ? "#b5d2fc" : "#373737"

                    visible: scrollBar.size < 1.0
                }
            }
        }

        // ========================================
        // Command Input Area
        // ========================================
        Item {
            id: commandArea

            Layout.fillWidth: true

            Layout.preferredHeight: Math.min(Math.max(20, commandInput.contentHeight + 2), 120)
            Layout.minimumHeight: 20
            Layout.maximumHeight: 120

            RowLayout {
                id: commandArea_RowLayout

                anchors.fill: parent

                spacing: 6

                Text {
                    id: commandArea_RowLayout_Text

                    text: ">>"

                    Layout.alignment: Qt.AlignTop

                    font.family: "Menlo"
                    font.pixelSize: 10
                    font.bold: true

                    color: "#4caf50"
                }

                TextArea {
                    id: commandInput

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    background: null

                    padding: 0
                    leftPadding: 0
                    rightPadding: 0
                    topPadding: 0
                    bottomPadding: 0

                    wrapMode: TextEdit.Wrap

                    font.bold: true
                    font.family: "Menlo"
                    font.pixelSize: 10

                    color: "#4caf50"
                    selectionColor: "#607d8b"
                    selectByMouse: true

                    Keys.onReturnPressed: { executeCommand() }

                    Keys.onEnterPressed: { executeCommand() }
                }
            }
        }
    }

    // ========================================
    // Terminal Model
    // ========================================
    ListModel { id: terminalModel }

    // ========================================
    // Limit Max Lines
    // ========================================
    function trimTerminalModel() {
        if (i_maxLines <= 0) { return }

        const excess = terminalModel.count - i_maxLines

        if (excess > 0) { terminalModel.remove(0, excess) }
    }

    // ========================================
    // AddLine
    // ========================================
    function addLine(text, type) {
        terminalModel.append({
            "content": text,
            "type": type || "debug"
        })

        trimTerminalModel()

        if (terminalView.b_autoScroll) { outputView.scrollToBottom() }
    }

    // ========================================
    // Add Multi-line
    // ========================================
    function addText(text, type) {
        const lines = text.split("\n")

        for (let i = 0; i < lines.length; ++i) {
            terminalModel.append({
                "content": lines[i],
                "type": type || "debug"
            })
        }

        trimTerminalModel()

        if (terminalView.b_autoScroll) { outputView.scrollToBottom() }
    }

    function addDebug(text) { addText(text, "debug") }

    function addInfo(text) { addText(text, "info") }

    function addWarning(text) { addText(text, "warning") }

    function addError(text) { addText(text, "error") }

    function addOutput(text) { addText(text, "output") }

    function addNewCommandLine(command) { addLine(command, "input") }

    // ========================================
    // Execute Command
    // ========================================
    function executeCommand() {
        const command = commandInput.text.trim()

        if (command.length === 0) { return }

        addNewCommandLine(command)

        commandInput.clear()

        commandEntered(command)
    }

    // ========================================
    // Clear Terminal
    // ========================================
    function clearCommandLines() { terminalModel.clear() }
}
