import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../../components" as Comp
import "../../download" as Do

Rectangle {
    readonly property bool showMenuBar: true

    property bool selfCheckWhenStart: true
    property bool onlyDownloading: false
    property bool allDownloading: false

    property alias downloadList: downloadList
    property alias terminalView: terminalView

    id: background

    color: "transparent"

    ColumnLayout {
        id: columnLayout

        anchors.fill: parent
        anchors.margins: 24

        spacing: 20

        RowLayout {
            id: buttonLayoutControl

            Layout.fillWidth: true

            spacing: 10

            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton {
                id: startBtn

                text: qsTr("Start")

                implicitWidth: 220

                enabled: !background.onlyDownloading && !background.allDownloading && downloadList.downloadListView.count > 0

                onClicked: {
                    if (background.onlyDownloading) {
                        toast.show(qsTr("One file is downloading. Please wait for finished"))
                        return
                    }

                    if (background.allDownloading) {
                        toast.show(qsTr("Other files are downloading. Please wait for finished"))
                        return
                    }

                    background.onlyDownloading = false
                    background.allDownloading = true

                    backend.startDownload()
                }
            }

            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton {
                id: stopBtn

                text: qsTr("Stop")

                implicitWidth: 220

                enabled: background.onlyDownloading || background.allDownloading

                onClicked: { backend.stopDownload() }
            }

            Item { Layout.fillWidth: true }

            // Comp.LiquidGlassButton { id: showInformation; text: qsTr("Show Info") }

            // Item { Layout.fillWidth: true }
        }

        ColumnLayout {
            id: downloadListAndTerminalView

            spacing: 10

            Do.DownloadList {
                id: downloadList

                Layout.fillWidth: true
                Layout.fillHeight: true

                onEditDownloadRequest: function(idd) {
                    mainStackView.push(
                        Qt.resolvedUrl("../addnewdownloadpage/AddNewDownloadPage.qml"),
                        {
                            "editMode": true,
                            "internalIdd": idd
                        }
                    )
                }

                onRemoveDownloadRequest: function(idd) { backend.removeDownload(idd) }

                onStartDownloadRequest: function(idd) {
                    if (background.onlyDownloading) {
                        toast.show(qsTr("One file is downloading. Please wait for finished"))
                        return
                    }

                    if (background.allDownloading) {
                        toast.show(qsTr("Other files are downloading. Please wait for finished"))
                        return
                    }

                    background.onlyDownloading = true
                    background.allDownloading = false

                    backend.startDownload(idd)
                }

                onStopDownloadRequest: function(idd) { backend.stopDownload(idd) }
            }

            Comp.LiquidGlassTerminalView {
                id: terminalView

                Layout.fillWidth: true
                Layout.fillHeight: true

                onCommandEntered: function(command) { }
            }
        }

        RowLayout {
            id: buttonLayoutUI

            Layout.fillWidth: true

            spacing: 10

            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton {
                id: addDownload

                text: qsTr("Add New Download")

                implicitWidth: 300

                onClicked: { mainStackView.push(Qt.resolvedUrl("../addnewdownloadpage/AddNewDownloadPage.qml")) }
            }

            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton {
                id: removeAllDownloads

                text: qsTr("Remove All Downloads")

                enabled: downloadList.downloadListView.count > 0 && !background.onlyDownloading && !background.allDownloading

                implicitWidth: 300

                onClicked: { askIfRemoveAllDownloads.open() }
            }

            Item { Layout.fillWidth: true }
        }
    }

    Comp.DownloadFileDialog {
        id: downloadYtDlp

        i_toolsIdx: 0

        p_footer_DialogButtonBox_ButtonAccept.onClicked: {
            if (downloadYtDlp.progressValue < 1) { backend.downloadYtDlp() }

            else {
                downloadYtDlp.close()
                backend.updateYtDlpToNightly()

                loading.running = true
                loading.visible = true
            }
        }

        p_footer_DialogButtonBox_ButtonReject.visible: false
    }

    Comp.DownloadFileDialog {
        id: downloadPoTokenProvider

        i_toolsIdx: 3

        p_footer_DialogButtonBox_ButtonAccept.onClicked: {
            if (downloadPoTokenProvider.progressValue < 1) { backend.downloadPoTokenProvider() }

            else { downloadPoTokenProvider.close() }
        }

        p_footer_DialogButtonBox_ButtonReject.visible: false
    }

    Timer {
        id: delayUI

        interval: 2000

        repeat: false

        onTriggered: { downloadYtDlp.progressValue = 1; backend.saveYtDlpPath() }
    }

    Comp.MessageDialog {
        id: askIfRemoveAllDownloads;

        width: 350
        height: 110

        messageText: qsTr("Are you sure you want to remove all downloads?")

        onAccepted: { backend.removeAllDownloads() }
    }

    Comp.MessageDialog { id: showInfo; b_askType: false }

    Comp.Toast { id: toast }

    Comp.LoadingOverlay { id: loading; overlayWidth: background.width; overlayHeight: background.height; }

    Connections {
        target: messageCenter

        function onDebugSent(message) { terminalView.addDebug(message) }
        function onInfoSent(message) { terminalView.addInfo(message) }
        function onWarningSent(message) { terminalView.addWarning(message) }
        function onErrorSent(message) { terminalView.addError(message) }
        function onOutputSent(message)  { terminalView.addOutput(message) }
    }

    Connections {
        target: backend

        function onSettingsLoaded(obj) {
            const _selfCheck = obj.selfCheck

            if (_selfCheck) {
                loading.running = true
                loading.visible = true
                backend.checkTools()
            }

            else { backend.loadTools() }
        }
    }

    Connections {
        target: backend
        enabled: background.parent.visible

        function onToolsChecked(needYtDlp, needFFmpeg, needNode, ydp, ffp, ndp) {
            loading.visible = false
            loading.running = false

            if (needFFmpeg || needNode) { windowManager.switchToAnotherWindow("ToolsInitConfigWindow.qml") }

            if (needYtDlp) {
                downloadYtDlp.progressValue = 0
                downloadYtDlp.open()
            }

            if (!needYtDlp && !needFFmpeg && !needNode) {
                backend.updateYtDlp()

                loading.running = true
                loading.visible = true
            }
        }

        function onYtDlpDownloaded(ok) {
            if (ok) {
                delayUI.stop()
                delayUI.start()
                return
            }

            downloadYtDlp.progressValue = -1;
        }

        function onYtDlpDownloadedProgress(bytesReceived, bytesTotal) {
            const d1 = Number(bytesReceived)
            const d2 = Number(bytesTotal)

            if (d2 === 0 || d1 === d2) { return }

            downloadYtDlp.progressValue = d1 / d2
        }

        function onYtDlpUpdated(ok) {
            loading.visible = false
            loading.running = false

            if (!ok) {
                showInfo.messageText = qsTr("Failed to update yt-dlp")
                showInfo.open()
            }
        }

        function onYtDlpUpdatedToNightly(ok) {
            loading.visible = false
            loading.running = false

            if (!ok) {
                showInfo.messageText = qsTr("Failed to update yt-dlp nightly")
                showInfo.open()
            }

            else { downloadPoTokenProvider.open() }
        }

        function onPoTokenProviderDownloaded(ok) {
            if (ok) {
                downloadPoTokenProvider.progressValue = 2
                return
            }

            downloadPoTokenProvider.progressValue = -1
        }

        function onPoTokenProviderDownloadedProgress(bytesReceived, bytesTotal) {
            const d1 = Number(bytesReceived)
            const d2 = Number(bytesTotal)

            if (d2 === 0) { return }

            downloadPoTokenProvider.progressValue = d1 / d2
        }

        function onPoTokenProviderStarted(ok) { if (ok) { downloadPoTokenProvider.progressValue = 1 } }

        function onIsAlreadyDownloading(internalId, message) { showInfo.messageText = message.trim(); showInfo.open() }

        function onIsNotDownloading(internalId, message) { showInfo.messageText = message.trim(); showInfo.open() }

        function onDownloadFinished(internalId) { if (background.onlyDownloading) { background.onlyDownloading = false } }

        function onDownloadStopped(internalId, ok, message) {
            if (ok) {
                if (background.onlyDownloading) {
                    background.onlyDownloading = false
                }

                else { background.allDownloading = false }
            }

            showInfo.messageText = message.trim()
            showInfo.open()
        }

        function onDownloadErrorOccurred(internalId, message) {
            if (background.onlyDownloading) { background.onlyDownloading = false }

            toast.show(message.trim())
        }

        function onSubprocessErrorOccurred(internalId, message) {
            if (background.onlyDownloading) { background.onlyDownloading = false }

            toast.show(message.trim())
        }

        function onFailedAtStart(internalId) {
            if (background.onlyDownloading) { background.onlyDownloading = false }

            toast.show(qsTr("Download failed started"))
        }

        function onAllDownloadFinished() {
            showInfo.messageText = qsTr("All downloads finished")
            showInfo.open()

            background.onlyDownloading = false
            background.allDownloading = false
        }

        function onAllDownloadStopped() {
            toast.show(qsTr("All downloads stopped"))

            background.onlyDownloading = false
            background.allDownloading = false
        }
    }
}
