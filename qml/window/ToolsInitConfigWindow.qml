import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts
import "../components" as Comp

ApplicationWindow {
    id: toolsInitConfigWindow

    color: "transparent"

    visible: true
    flags: Qt.Window | Qt.WindowTitleHint | Qt.CustomizeWindowHint

    width: 950
    height: 450

    minimumWidth: 950
    maximumWidth: 950

    minimumHeight: 450
    maximumHeight: 450

    title: qsTr("Init Configuration Tools")

    onClosing: (event) => { event.accepted = true; windowManager.backToMainWindow() }

    Component.onCompleted: { backend.findToolsPath() }

    Comp.Background { }

    ColumnLayout {
        id: columnLayout

        anchors.fill: parent
        anchors.margins: 50

        spacing: 50

        Item { Layout.fillHeight: true }

        GroupBox {
            Layout.fillWidth: true

            title: qsTr("Tools")

            GridLayout {
                anchors.centerIn: parent
                anchors.margins: 20

                columns: 3
                columnSpacing: 16
                rowSpacing: 20

                Label {
                    text: qsTr("ffmpeg:")

                    font.pixelSize: 14
                    font.bold: true

                    opacity: 0.7

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                Comp.LiquidGlassTextField {
                    id: textFieldFFmpeg

                    implicitWidth: 500

                    font.pixelSize: 14

                    extraTimer.onTriggered: {
                        loading.running = true
                        loading.visible = true

                        backend.checkFFmpeg(textFieldFFmpeg.text.trim())
                    }
                }

                Comp.LiquidGlassButton {
                    id: searchFFmpegBtn

                    text: qsTr("Search")

                    implicitWidth: 160

                    onClicked: { searchFFmpeg.open() }
                }

                Label {
                    text: qsTr("node:")

                    font.pixelSize: 14
                    font.bold: true

                    opacity: 0.7

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                Comp.LiquidGlassTextField {
                    id: textFieldNode

                    implicitWidth: 500

                    font.pixelSize: 14

                    extraTimer.onTriggered: {
                        loading.running = true
                        loading.visible = true

                        backend.checkNode(textFieldNode.text.trim())
                    }
                }

                Comp.LiquidGlassButton {
                    id: searchNodeBtn

                    text: qsTr("Search")

                    implicitWidth: 160

                    onClicked: { searchNode.open() }
                }
            }

            label: Label {
                text: parent.title

                anchors.left: parent.left
                anchors.leftMargin: 12

                font.pixelSize: 12
                font.bold: true

                opacity: 0.3

                color: "#ffffff"
            }

            background: Rectangle {
                border.color: "#dedede"
                border.width: 1

                opacity: 0.3

                radius: 8

                color: "transparent"
            }
        }

        RowLayout {
            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton {
                id: confirmBtn

                text: qsTr("Confirm")

                enabled: textFieldFFmpeg.text.trim().length > 0 && textFieldNode.text.trim().length > 0

                onClicked: {
                    loading.running = true
                    loading.visible = true

                    backend.saveFFmpegPath(textFieldFFmpeg.text.trim())
                    backend.saveNodePath(textFieldNode.text.trim())

                    toolsInitConfigWindow.close()
                }
            }

            Item { Layout.fillWidth: true }
        }

        Item { Layout.fillHeight: true }
    }

    FileDialog {
        id: searchFFmpeg

        fileMode: FileDialog.OpenFile

        title: qsTr("Choose ffmpeg")

        onAccepted: {
            loading.running = true
            loading.visible = true

            backend.checkFFmpeg(searchFFmpeg.selectedFile)
        }
    }

    FileDialog {
        id: searchNode

        fileMode: FileDialog.OpenFile

        title: qsTr("Choose node")

        onAccepted: {
            loading.running = true
            loading.visible = true

            backend.checkNode(searchNode.selectedFile)
        }
    }

    Comp.MessageDialog { id: showInfo; b_askType: false }

    Comp.LoadingOverlay { id: loading; r_overlayWidth: toolsInitConfigWindow.width; r_overlayHeight: toolsInitConfigWindow.height }

    Connections {
        target: backend
        enabled: toolsInitConfigWindow.visible

        function onToolsPathFound(ffp, ndp) {
            loading.visible = false
            loading.running = false

            textFieldFFmpeg.text = ffp
            textFieldNode.text = ndp
        }

        function onFfmpegChecked(isExecutable, isFFmpeg, filePath) {
            loading.visible = false
            loading.running = false

            if (isExecutable) {
                if (isFFmpeg) { textFieldFFmpeg.text = filePath }

                else {
                    showInfo.width = 300
                    showInfo.messageText = qsTr("The selected file is not ffmpeg")
                    showInfo.open()
                }
            }

            else {
                showInfo.width = 350
                showInfo.messageText = qsTr("The selected file is not an executable file")
                showInfo.open()
            }
        }

        function onNodeChecked(isExecutable, isNode, filePath) {
            loading.visible = false
            loading.running = false

            if (isExecutable) {
                if (isNode) { textFieldNode.text = filePath }

                else {
                    showInfo.width = 300
                    showInfo.messageText = qsTr("The selected file is not nodejs")
                    showInfo.open()
                }
            }

            else {
                showInfo.width = 350
                showInfo.messageText = qsTr("The selected file is not an executable file")
                showInfo.open()
            }
        }
    }
}
