import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts
import "../../components" as Comp

ApplicationWindow {
    property bool lightMode: Application.styleHints.colorScheme === Qt.Light

    id: toolsWindow

    visible: true

    width: 1000
    height: 450

    minimumWidth: toolsWindow.width
    minimumHeight: toolsWindow.height
    maximumWidth: toolsWindow.width
    maximumHeight: toolsWindow.height

    title: qsTr("Tools Config")

    property bool b_beforeMainPage: false
    property bool b_closingByManager: false

    onClosing: (event) => {
        if (b_closingByManager) { event.accepted = true; return }

        if (!b_beforeMainPage) {
            event.accepted = true
            b_closingByManager = true
            windowManager.backToMainWindow()
        }

        else { event.accepted = false; quitApp.open() }
    }

    Component.onCompleted: { backend.findToolsPath() }

    GridLayout {
        id: gridLayout

        anchors.centerIn: parent
        anchors.margins: 20

        columns: 3

        columnSpacing: 16
        rowSpacing: 50

        Comp.P_Label { id: label_yt_dlp; text: qsTr("yt-dlp:"); lightMode: toolsWindow.lightMode }

        Comp.P_TextField { id: textField_yt_dlp; implicitWidth: 600; readOnly: true; lightMode: toolsWindow.lightMode }

        Comp.P_Button { id: searchYtDlp; text: qsTr("Search"); lightMode: toolsWindow.lightMode; onClicked: { search_yt_dlp.open() } }

        Comp.P_Label { id: label_ffmpeg; text: qsTr("ffmpeg:"); lightMode: toolsWindow.lightMode }

        Comp.P_TextField { id: textField_ffmpeg; implicitWidth: 600; readOnly: true; lightMode: toolsWindow.lightMode }

        Comp.P_Button { id: searchFFmpeg; text: qsTr("Search"); lightMode: toolsWindow.lightMode; onClicked: { search_ffmpeg.open() } }

        Comp.P_Label { id: label_node; text: qsTr("node:"); lightMode: toolsWindow.lightMode }

        Comp.P_TextField { id: textField_node; implicitWidth: 600; readOnly: true; lightMode: toolsWindow.lightMode }

        Comp.P_Button { id: searchNode; text: qsTr("Search"); lightMode: toolsWindow.lightMode; onClicked: { search_node.open() } }

        Comp.P_Label { text: " "; enabled: false }

        Comp.P_Button {
            id: confirmBtn

            text: qsTr("OK")

            implicitWidth: 600

            lightMode: toolsWindow.lightMode

            enabled: textField_yt_dlp.text.trim() !== "" && textField_ffmpeg.text.trim() !== "" && textField_node.text.trim() !== ""

            onClicked: {
                loading.running = true
                loading.visible = true
                backend.saveToolsPath(textField_yt_dlp.text.trim(), textField_ffmpeg.text.trim(), textField_node.text.trim())
            }
        }
    }

    FileDialog {
        id: search_yt_dlp

        fileMode: FileDialog.OpenFile

        title: qsTr("Choose yt-dlp")

        onAccepted: {
            loading.running = true
            loading.visible = true
            backend.checkYtDlp(search_yt_dlp.selectedFile)
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

    Comp.P_MessageDialog { id: messageDialog; standardButtons: Dialog.Ok }

    Comp.P_MessageDialog {
        id: quitApp;

        messageText: qsTr("You must provide the paths to all three tools, otherwise the application will exit")

        onAccepted: {
            b_closingByManager = true
            backend.requestQuitApp();
        }
    }

    Comp.P_LoadingOverlay { id: loading; lightMode: toolsWindow.lightMode; overlayWidth: toolsWindow.width; overlayHeight: toolsWindow.height }

    background: Rectangle { id: background_Rectangle; color: lightMode ? "#d7f3ff" : "#20232a" }

    Connections {
        target: backend
        enabled: visible

        function onToolsPathFound(obj) {
            textField_yt_dlp.text = obj.yt_dlp
            textField_ffmpeg.text = obj.ffmpeg
            textField_node.text = obj.node
        }

        function onYtDlpChecked(isExecutable, isYtDlp, path) {
            loading.visible = false
            loading.running = false

            if (isExecutable) {
                if (isYtDlp) { textField_yt_dlp.text = path }

                else {
                    messageDialog.messageText = qsTr("The selected file is not yt-dlp")
                    messageDialog.open()
                }
            }

            else {
                messageDialog.messageText = qsTr("The selected file is not an executable")
                messageDialog.open()
            }
        }

        function onFfmpegChecked(isExecutable, isFFmpeg, path) {
            loading.visible = false
            loading.running = false

            if (isExecutable) {
                if (isFFmpeg) { textField_ffmpeg.text = path }

                else {
                    messageDialog.messageText = qsTr("The selected file is not ffmpeg")
                    messageDialog.open()
                }
            }

            else {
                messageDialog.messageText = qsTr("The selected file is not an executable")
                messageDialog.open()
            }
        }

        function onNodeChecked(isExecutable, isNode, path) {
            loading.visible = false
            loading.running = false

            if (isExecutable) {
                if (isNode) { textField_node.text = path }

                else {
                    messageDialog.messageText = qsTr("The selected file is not nodejs")
                    messageDialog.open()
                }
            }

            else {
                messageDialog.messageText = qsTr("The selected file is not an executable")
                messageDialog.open()
            }
        }

        function onToolsPathSaved(ok, error) {
            loading.visible = false
            loading.running = false

            if (ok) { b_closingByManager = true; windowManager.backToMainWindow() }

            else {
                messageDialog.messageText = error
                messageDialog.open()
            }
        }
    }
}
