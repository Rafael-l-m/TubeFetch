import QtQuick
import QtQuick.Layouts
import "../../components" as Comp
import "../../download" as Do

Rectangle {
    id: background

    color: "transparent"

    Component.onCompleted: {} // backend.loadSettings(); }

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

            Comp.LiquidGlassButton { id: startBtn; text: qsTr("Start"); enabled: true }

            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton { id: stopBtn; text: qsTr("Stop"); enabled: false }

            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton { id: showInformation; text: qsTr("Show Info") }

            Item { Layout.fillWidth: true }
        }

        ColumnLayout {
            id: downloadListAndTerminalView

            spacing: 10

            Do.DownloadList {
                id: downloadList

                Layout.fillWidth: true
                Layout.fillHeight: true

                onEditDownloadRequest:   function(idd) { console.log("Edit item: ", idd)   }
                onRemoveDownloadRequest: function(idd) { console.log("Remove item: ", idd) }
                onStartDownloadRequest:  function(idd) { console.log("Start item: ", idd)  }
                onStopDownloadRequest:   function(idd) { console.log("Stop item: ", idd)   }
            }

            Comp.LiquidGlassTerminalView { id: terminalView; Layout.fillWidth: true; Layout.fillHeight: true }
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

            Comp.LiquidGlassButton { id: removeAllDownloads; text: qsTr("Remove All Downloads"); implicitWidth: 300 }

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

    Comp.MessageDialog { id: showInfo; b_askType: false }

    Comp.Toast { id: toast }

    Comp.LoadingOverlay { id: loading; r_overlayWidth: background.width; r_overlayHeight: background.height; }

    Connections {
        target: messageCenter

        function onDebugSent(message)   {
            terminalView.addDebug(message)
            console.log("Debug: ", message)
        }

        function onInfoSent(message)    {
            terminalView.addInfo(message)
            console.log("Info: ", message)
        }

        function onWarningSent(message) {
            terminalView.addWarning(message)
            console.log("Warning: ", message)
        }

        function onErrorSent(message)   {
            terminalView.addError(message)
            console.log("Error: ", message)
        }

        function onOutputSent(message)  {
            terminalView.addOutput(message)
            console.log("Output: ", message)
        }
    }

    Connections {
        target: backend

        function onSettingsLoaded(obj) {
            const _selfCheck = obj.selfCheck

            console.log("Check: ", _selfCheck)

            if (_selfCheck) {
                loading.running = true
                loading.visible = true
                backend.checkTools()
            }

            else {
                backend.loadTools()
            }
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
    }
}
