import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs

import "../components" as Comp


ApplicationWindow {
    property bool lightMode: Application.styleHints.colorScheme === Qt.Light

    id: toolsWindow

    visible: true
    flags: Qt.Window | Qt.WindowTitleHint | Qt.CustomizeWindowHint

    width: 900
    height: 350

    minimumWidth: 900
    minimumHeight: 350

    maximumWidth: 900
    maximumHeight: 350

    title: qsTr("Tools Config")

    onClosing: (event) => { event.accepted = true; windowManager.backToMainWindow() }

    Component.onCompleted: { backend.findToolsPath() }

    GridLayout {
        id: gridLayout

        anchors.centerIn: parent
        anchors.margins: 20

        columns: 3

        columnSpacing: 16
        rowSpacing: 50

        Label {
            id: labelFFmpeg

            text: qsTr("ffmpeg:")

            font.pixelSize: 12
            font.bold: true

            color: toolsWindow.lightMode ? "#000000" : "#ffffff"
        }

        Comp.InputField {
            id: textField_ffmpeg

            implicitWidth: 600

            readOnly: true

            lightMode: toolsWindow.lightMode
        }

        Comp.PushButton {
            id: searchFFmpeg

            text: qsTr("Search")

            lightMode: toolsWindow.lightMode

            onClicked: { search_ffmpeg.open() }
        }

        Label {
            id: labelNode

            text: qsTr("node:")

            font.pixelSize: 12
            font.bold: true

            color: toolsWindow.lightMode ? "#000000" : "#ffffff"
        }

        Comp.InputField {
            id: textField_node

            implicitWidth: 600

            readOnly: true

            lightMode: toolsWindow.lightMode
        }

        Comp.PushButton {
            id: searchNode

            text: qsTr("Search")

            lightMode: toolsWindow.lightMode;

            onClicked: { search_node.open() }
        }

        Label {
            text: ""
            enabled: false
        }

        Comp.PushButton {
            id: confirmBtn

            text: qsTr("OK")

            implicitWidth: 600

            lightMode: toolsWindow.lightMode

            enabled: textField_ffmpeg.text.trim() !== "" && textField_node.text.trim() !== ""

            onClicked: {
                loading.running = true
                loading.visible = true

                backend.saveFFmpegPath(textField_ffmpeg.text.trim())
                backend.saveNodePath(textField_node.text.trim())

                toolsWindow.close()
            }
        }
    }

    FileDialog {
        id: search_ffmpeg

        fileMode: FileDialog.OpenFile

        title: qsTr("Choose ffmpeg")

        onAccepted: {
            loading.running = true
            loading.visible = true

            backend.checkFFmpeg(search_ffmpeg.selectedFile)
        }
    }

    FileDialog {
        id: search_node

        fileMode: FileDialog.OpenFile

        title: qsTr("Choose node")

        onAccepted: {
            loading.running = true
            loading.visible = true

            backend.checkNode(search_node.selectedFile)
        }
    }

    Comp.MessageDialog { id: messageDialog; b_askType: false }

    Comp.LoadingOverlay { id: loading; lightMode: toolsWindow.lightMode; r_overlayWidth: toolsWindow.width; r_overlayHeight: toolsWindow.height }

    background: Rectangle { id: background_Rectangle; color: lightMode ? "#d7f3ff" : "#20232a" }

    Connections {
        target: backend
        enabled: toolsWindow.visible

        function onToolsPathFound(ffp, ndp) {
            textField_ffmpeg.text = ffp
            textField_node.text = ndp
        }

        function onFfmpegChecked(isExecutable, isFFmpeg, filePath) {
            loading.visible = false
            loading.running = false

            if (isExecutable) {
                if (isFFmpeg) { textField_ffmpeg.text = filePath }

                else {
                    messageDialog.width = 300
                    messageDialog.messageText = qsTr("The selected file is not ffmpeg")
                    messageDialog.open()
                }
            }

            else {
                messageDialog.width = 350
                messageDialog.messageText = qsTr("The selected file is not an executable file")
                messageDialog.open()
            }
        }

        function onNodeChecked(isExecutable, isNode, filePath) {
            loading.visible = false
            loading.running = false

            if (isExecutable) {
                if (isNode) { textField_node.text = filePath }

                else {
                    messageDialog.width = 300
                    messageDialog.messageText = qsTr("The selected file is not nodejs")
                    messageDialog.open()
                }
            }

            else {
                messageDialog.width = 350
                messageDialog.messageText = qsTr("The selected file is not an executable file")
                messageDialog.open()
            }
        }

        function onSettingsLoaded(obj) {
            const lma = obj.lightModeAutomatic
            const lm = obj.lightMode

            if (!lma) { toolsWindow.lightMode = lm }

            else { toolsWindow.lightMode = Qt.binding(function() {
                return Application.styleHints.colorScheme === Qt.Light
            })}
        }
    }
}
