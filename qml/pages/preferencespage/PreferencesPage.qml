import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts
import "../../components" as Comp

Rectangle {
    property string savedFFmpegPath
    property string savedNodePath

    id: preferencesPage

    color: "transparent"

    // ESC -> Rollback
    focus: true
    Keys.onEscapePressed: { mainStackView.pop() }

    // Simulate: Rollback
    MultiPointTouchArea {
        anchors.fill: parent
        minimumTouchPoints: 1
        maximumTouchPoints: 1
        onReleased: (points) => { if (points[0].x - points[0].startX > 120) { mainStackView.pop() } }
    }

    Component.onCompleted: {
        loading.running = true
        loading.visible = true

        backend.loadSettingsPreferencesPage()
    }

    ColumnLayout {
        id: columnLayout

        anchors.fill: parent
        anchors.margins: 50

        spacing: 50

        Item { Layout.fillHeight: true }

        GroupBox {
            Layout.fillWidth: true

            title: qsTr("Info")

            GridLayout {
                anchors.centerIn: parent
                anchors.margins: 20

                columns: 2
                columnSpacing: 16
                rowSpacing: 20

                Label {
                    text: qsTr("App Version:")

                    font.pixelSize: 12
                    font.bold: true

                    opacity: 0.7

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }

                Label {
                    id: appVersionLab

                    font.pixelSize: 12
                    font.bold: true

                    opacity: 0.7

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
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

        GroupBox {
            Layout.fillWidth: true

            title: qsTr("Config")

            GridLayout {
                anchors.centerIn: parent
                anchors.margins: 20

                columns: 2
                columnSpacing: 16
                rowSpacing: 20

                Label {
                    text: qsTr("Self-Check:")

                    font.pixelSize: 12
                    font.bold: true

                    opacity: 0.7

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }

                Comp.LiquidGlassSwitch {
                    id: selfCheckSwitch

                    checked: true

                    onToggled: {
                        loading.running = true
                        loading.visible = true

                        backend.updateSelfCheck(selfCheckSwitch.checked)
                    }
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
                    text: qsTr("yt-dlp:")

                    font.pixelSize: 14
                    font.bold: true

                    opacity: 0.7

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                Comp.LiquidGlassTextField {
                    id: textFieldYtDlp

                    implicitWidth: 500

                    font.pixelSize: 14

                    readOnly: true
                }

                Label {
                    text: " "
                    enabled: false
                    opacity: 0
                }

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

                        backend.checkFFmpeg(textFieldFFmpeg.text.trim());
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

                        backend.checkNode(textFieldNode.text.trim());
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
            spacing: 50

            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton {
                text: qsTr("Back")

                onClicked: { mainStackView.pop() }
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

    Comp.Toast { id: toast }

    Comp.LoadingOverlay { id: loading; overlayWidth: preferencesPage.width; overlayHeight: preferencesPage.height }

    Connections {
        target: backend
        enabled: preferencesPage.visible

        function onSettingsLoadedPreferencesPage(obj) {
            loading.visible = false
            loading.running = false

            appVersionLab.text = obj.versions.trim()
            selfCheckSwitch.checked = obj.selfCheck
            textFieldYtDlp.text = obj.ytDlpPath.trim()
            textFieldFFmpeg.text = obj.ffmpegPath.trim()
            textFieldNode.text = obj.nodePath.trim()

            preferencesPage.savedFFmpegPath = textFieldFFmpeg.text.trim()
            preferencesPage.savedNodePath = textFieldNode.text.trim()
        }

        function onSelfCheckUpdated(ok) {
            loading.visible = false
            loading.running = false

            if (ok) { toast.show(qsTr("Self-Check updated correctly")) }

            else {
                selfCheckSwitch.checked = !selfCheckSwitch.checked
                toast.show(qsTr("Self-Check updated incorrectly"))
            }
        }

        function onFfmpegChecked(isExecutable, isFFmpeg, filePath) {
            loading.visible = false
            loading.running = false

            if (isExecutable) {
                if (isFFmpeg) {
                    loading.running = true
                    loading.visible = true

                    const _pathFFmpeg = filePath.trim()

                    textFieldFFmpeg.text = _pathFFmpeg
                    backend.updateFFmpegPath(_pathFFmpeg)

                    return
                }

                else { toast.show(qsTr("The selected file is not ffmpeg")) }
            }

            else { toast.show(qsTr("The selected file is not an executable file")) }

            textFieldFFmpeg.text = preferencesPage.savedFFmpegPath.trim()
        }

        function onNodeChecked(isExecutable, isNode, filePath) {
            loading.visible = false
            loading.running = false

            if (isExecutable) {
                if (isNode) {
                    loading.running = true
                    loading.visible = true

                    const _pathNode = filePath.trim()

                    textFieldNode.text = _pathNode
                    backend.updateNodePath(_pathNode)

                    return
                }

                else { toast.show(qsTr("The selected file is not nodejs")) }
            }

            else { toast.show(qsTr("The selected file is not an executable file")) }

            textFieldNode.text = preferencesPage.savedNodePath.trim()
        }

        function onFfmpegPathUpdated(ok) {
            loading.visible = false
            loading.running = false

            if (ok) {
                preferencesPage.savedFFmpegPath = textFieldFFmpeg.text.trim()
                toast.show(qsTr("FFmpeg path updated correctly"))
            }

            else {
                textFieldFFmpeg.text = preferencesPage.savedNodePath.trim()
                toast.show(qsTr("FFmpeg path updated incorrectly"))
            }
        }

        function onNodePathUpdated(ok) {
            loading.visible = false
            loading.running = false

            if (ok) {
                preferencesPage.savedNodePath = textFieldNode.text.trim()
                toast.show(qsTr("Node path updated correctly"))
            }

            else {
                textFieldNode.text = preferencesPage.savedNodePath.trim()
                toast.show(qsTr("Node path updated incorrectly"))
            }
        }
    }
}
