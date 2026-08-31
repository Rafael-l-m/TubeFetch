import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Imagine
import QtQuick.Layouts
import QtQuick.Dialogs

import "../../components" as Comp


Flickable {
    property bool lightMode

    property alias p_ColumnLayout: mainLayout
    property alias p_ColumnLayout_GridLayout: gridLayout
    property alias p_ColumnLayout_GridLayout_LabelFFmpeg: ffmpegLabel
    property alias p_ColumnLayout_GridLayout_InputFieldFFmpeg: ffmpegInputField
    property alias p_ColumnLayout_GridLayout_PushButtonFFmpeg: searchFFmpeg
    property alias p_ColumnLayout_GridLayout_LabelNode: nodeLabel
    property alias p_ColumnLayout_GridLayout_InputFieldNode: nodeInputField
    property alias p_ColumnLayout_GridLayout_PushButtonNode: searchNode
    property alias p_FileDialogSearchFFmpeg: search_ffmpeg
    property alias p_FileDialogSearchNode: search_node
    property alias p_MessageDialog: showInfo
    property alias p_LoadingOverlay: loading

    id: downloadSettings

    contentWidth: width
    contentHeight: mainLayout.implicitHeight

    Component.onCompleted: { backend.loadSettings() }

    ColumnLayout {
        id: mainLayout

        width: parent.width

        anchors.top: parent.top
        anchors.topMargin: 40

        spacing: 30

        GridLayout {
            id: gridLayout

            Layout.alignment: Qt.AlignHCenter

            columns: 3
            columnSpacing: 20

            rowSpacing: 30

            Label {
                id: ffmpegLabel

                text: qsTr("FFmpeg Path:")

                font.pixelSize: 12
                font.bold: true

                color: downloadSettings.lightMode ? "#000000" : "#ffffff"

                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            Comp.InputField {
                id: ffmpegInputField

                readOnly: true

                implicitWidth: 410

                lightMode: downloadSettings.lightMode
            }

            Comp.PushButton {
                id: searchFFmpeg

                text: qsTr("Search")

                lightMode: downloadSettings.lightMode

                onClicked: { search_ffmpeg.open() }
            }

            Label {
                id: nodeLabel

                text: qsTr("Node Path:")

                font.pixelSize: 12
                font.bold: true

                color: downloadSettings.lightMode ? "#000000" : "#ffffff"

                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            Comp.InputField {
                id: nodeInputField

                readOnly: true

                implicitWidth: 410

                lightMode: downloadSettings.lightMode
            }

            Comp.PushButton {
                id: searchNode

                text: qsTr("Search")

                lightMode: downloadSettings.lightMode

                onClicked: { search_node.open() }
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

    Comp.MessageDialog { id: showInfo; lightMode: downloadSettings.lightMode }

    Comp.LoadingOverlay { id: loading; lightMode: downloadSettings.lightMode; r_overlayWidth: downloadSettings.width; r_overlayHeight: downloadSettings.height }

    Connections {
        target: backend
        enabled: downloadSettings.visible

        function onSettingsLoaded(obj) {
            ffmpegInputField.text = obj.ffmpegPath.trim()
            nodeInputField.text = obj.nodePath.trim()
        }

        function onFfmpegChecked(isExecutable, isFFmpeg, filePath) {
            loading.visible = false
            loading.running = false

            if (isExecutable) {
                if (isFFmpeg) {
                    ffmpegInputField.text = filePath
                    backend.updateFFmpegPath(filePath)
                }

                else {
                    showInfo.messageText = qsTr("The selected file is not ffmpeg")
                    showInfo.open()
                }
            }

            else {
                showInfo.messageText = qsTr("The selected file is not an executable file")
                showInfo.open()
            }
        }

        function onNodeChecked(isExecutable, isNode, filePath) {
            loading.visible = false
            loading.running = false

            if (isExecutable) {
                if (isNode) {
                    nodeInputField.text = filePath
                    backend.updateNodePath(filePath)
                }

                else {
                    showInfo.messageText = qsTr("The selected file is not nodejs")
                    showInfo.open()
                }
            }

            else {
                showInfo.messageText = qsTr("The selected file is not an executable file")
                showInfo.open()
            }
        }
    }
}
