import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import "../components" as Comp
import "../dialog" as Dial
import "../download" as Do
import "mainpage" as Mp
import "../window" as Win
import "../window/menubar" as Men

Page {
    property bool lightMode

    property alias p_Rectangle: rectangle
    property alias p_Rectangle_ColumnLayout: columnLayout
    property alias p_Rectangle_ColumnLayout_R1: r1
    property alias p_Rectangle_ColumnLayout_R2_R3_ColumnLayout: r2_r3_columnLayout
    property alias p_Rectangle_ColumnLayout_R2: r2
    property alias p_Rectangle_ColumnLayout_ColumnLayout_TerminalView: r3
    property alias p_Rectangle_ColumnLayout_R4: r4
    property alias p_DownloadYtDlpWindow: downloadYtDlp
    property alias p_DownloadPoTokenProviderWindow: downloadPoTokenProvider
    property alias p_MessageDialogAskIfAllDownloads: askIfRemoveAllDownloads
    property alias p_MessageDialogShowInfo: showInfo
    property alias p_LoadingOverlay: loading
    property alias p_Toast: toast
    property alias p_Timer: delayUI

    property bool b_checkToolsWhenExecute: true
    property bool b_enableRateLimit: true
    property bool b_canPerform: true
    property bool b_onlyDownloading: false
    property bool b_allDownloading: false
    property var qint64_onlyDownloadingId: 0

    id: mainPage

    font.pixelSize: 12

    Component.onCompleted: {
        if (mainPage.b_checkToolsWhenExecute) {
            loading.running = true
            loading.visible = true

            backend.checkTools()
        }

        else { backend.loadTools() }

        if (mainPage.b_enableRateLimit) { backend.consume(0, 0, 0) }
    }

    Rectangle {
        id: rectangle

        anchors.fill: parent

        color: mainPage.lightMode ? "#D7F3FF" : "#20232a"

        ColumnLayout {
            id: columnLayout

            anchors.fill: parent
            anchors.margins: 24

            spacing: 20

            Mp.ButtonLayoutControl {
                id: r1

                lightMode: mainPage.lightMode

                p_PushButtonStart.enabled: r2.p_MouseArea_ListView.count > 0 && !mainPage.b_onlyDownloading && !mainPage.b_allDownloading
                p_PushButtonStart.onClicked: {
                    if (mainPage.b_enableRateLimit && !mainPage.b_canPerform) {
                        toast.show(qsTr("You have reached the limit. Please wait at least one hour before trying again."))
                        return
                    }

                    if (mainPage.b_onlyDownloading) {
                        toast.show(qsTr("One file is downloading. Please wait for finished"))
                        return
                    }

                    if (mainPage.b_allDownloading) {
                        toast.show(qsTr("Other files are downloading. Please wait for finished"))
                        return
                    }

                    if (r2.p_MouseArea_ListView.currentIndex >= 0 && r2.selectedInternalId > 0) {
                        mainPage.b_onlyDownloading = true
                        mainPage.qint64_onlyDownloadingId = r2.selectedInternalId
                        mainPage.b_allDownloading = false
                        backend.startDownload(r2.selectedInternalId)
                    }

                    else {
                        mainPage.b_onlyDownloading = false
                        mainPage.qint64_onlyDownloadingId = 0
                        mainPage.b_allDownloading = true
                        backend.startDownload()
                    }
                }

                p_PushButtonStop.enabled: mainPage.b_onlyDownloading || mainPage.b_allDownloading
                p_PushButtonStop.onClicked: {
                    if (r2.p_MouseArea_ListView.currentIndex >= 0 && r2.selectedInternalId > 0) {
                        if (mainPage.b_onlyDownloading) { backend.stopDownload(r2.selectedInternalId) }
                        else { toast.show(qsTr("All items are downloading in order. To stop all downloads, you shouldn’t select just one item. Please click elsewhere to deselect everything, then try again")) }
                    }

                    else { backend.stopDownload()  }
                }

                p_PushButtonEdit.enabled: r2.p_MouseArea_ListView.currentIndex >= 0 && r2.selectedInternalId > 0 && !mainPage.b_onlyDownloading && !mainPage.b_allDownloading
                p_PushButtonEdit.onClicked: {
                    windowManager.switchToAnotherWindow(
                        "AddNewDownloadWindow.qml",
                        true,
                        true,
                        true,
                        { "b_editMode": true, "qint64_internalId": r2.selectedInternalId }
                    )

                    r2.p_MouseArea_ListView.currentIndex = -1
                    r2.selectedInternalId = -1
                }

                p_PushButtonRemove.enabled: r2.p_MouseArea_ListView.currentIndex >= 0 && r2.selectedInternalId > 0 && !mainPage.b_onlyDownloading && !mainPage.b_allDownloading
                p_PushButtonRemove.onClicked: { backend.removeDownload(r2.selectedInternalId) }
            }

            ColumnLayout {
                id: r2_r3_columnLayout
                spacing: 10

                Do.DownloadList {
                    id: r2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    lightMode: mainPage.lightMode
                }

                Comp.TerminalView {
                    id: r3
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    lightMode: mainPage.lightMode
                }
            }

            Mp.ButtonLayoutUI {
                id: r4

                lightMode: mainPage.lightMode

                p_PushButtonAdd.onClicked: { windowManager.switchToAnotherWindow("AddNewDownloadWindow.qml") }

                p_PushButtonRemoveAll.enabled: r2.p_MouseArea_ListView.count > 0
                p_PushButtonRemoveAll.onClicked: { askIfRemoveAllDownloads.open() }
            }
        }
    }

    Dial.DownloadFileDialog {
        id: downloadYtDlp

        lightMode: mainPage.lightMode

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
    }

    Dial.DownloadFileDialog {
        id: downloadPoTokenProvider

        lightMode: mainPage.lightMode

        i_toolsIdx: 3

        p_footer_DialogButtonBox_ButtonAccept.onClicked: {
            if (downloadPoTokenProvider.progressValue < 1) { backend.downloadPoTokenProvider() }

            else { downloadPoTokenProvider.close() }
        }
    }

    Comp.MessageDialog {
        id: askIfRemoveAllDownloads

        messageText: qsTr("Are you sure you want to remove all downloads?")

        onAccepted: { backend.removeAllDownloads() }
    }

    Comp.MessageDialog { id: showInfo; b_askType: false }

    Comp.LoadingOverlay { id: loading; r_overlayWidth: mainPage.width; r_overlayHeight: mainPage.height }

    Comp.Toast { id: toast; lightMode: mainPage.lightMode }

    Timer {
        id: delayUI

        interval: 2000

        repeat: false

        onTriggered: { downloadYtDlp.progressValue = 1; backend.saveYtDlpPath() }
    }

    Connections {
        target: messageCenter

        function onDebugSent(message)   { r3.addDebug(message)   }
        function onInfoSent(message)    { r3.addInfo(message)    }
        function onWarningSent(message) { r3.addWarning(message) }
        function onErrorSent(message)   { r3.addError(message)   }
        function onOutputSent(message)  { r3.addOutput(message)  }
    }

    Connections {
        target: backend
        enabled: mainPage.visible

        function onToolsChecked(needYtDlp, needFFmpeg, needNode, ydp, ffp, ndp) {
            loading.visible = false
            loading.running = false

            if (needFFmpeg || needNode) { windowManager.switchToAnotherWindow("SearchToolsWindow.qml") }

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

        function onDownloadRemoved(ok, message) {
            if (ok) {
                r2.p_MouseArea_ListView.currentIndex = -1
                r2.selectedInternalId = -1
            }

            else { showInfo.messageText = message; showInfo.open() }
        }

        function onDownloadsRemoved(ok, message) {
            if (ok) {
                r2.p_MouseArea_ListView.currentIndex = -1
                r2.selectedInternalId = -1
            }

            else { showInfo.messageText = message; showInfo.open() }
        }

        function onIsAlreadyDownloading(internalId, message) {
            showInfo.messageText = message
            showInfo.open()
        }

        function onIsNotDownloading(internalId, message) {
            showInfo.messageText = message
            showInfo.open()
        }

        function onDownloadFinished(internalId) {
            if (mainPage.b_onlyDownloading) {
                b_onlyDownloading = false
                mainPage.qint64_onlyDownloadingId = 0
            }
        }

        function onDownloadStopped(internalId, ok, message) {
            if (ok) {
                if (mainPage.b_onlyDownloading) {
                    b_onlyDownloading = false
                    mainPage.qint64_onlyDownloadingId = 0
                }

                else {mainPage.b_allDownloading = false}
            }

            showInfo.messageText = message
            showInfo.open()
        }

        function onDownloadErrorOccurred(internalId, message) {
            if (mainPage.b_onlyDownloading) {
                b_onlyDownloading = false
                mainPage.qint64_onlyDownloadingId = 0
            }

            toast.show(message)
        }

        function onSubprocessErrorOccurred(internalId, message) {
            if (mainPage.b_onlyDownloading) {
                b_onlyDownloading = false
                mainPage.qint64_onlyDownloadingId = 0
            }

            toast.show(message)
        }

        function onFailedAtStart(internalId) {
            if (mainPage.b_onlyDownloading) {
                b_onlyDownloading = false
                mainPage.qint64_onlyDownloadingId = 0
            }

            toast.show(qsTr("Process started failed"))
        }

        function onAllDownloadFinished() {
            showInfo.messageText = qsTr("All downloads finished");
            showInfo.open()

            b_allDownloading = false
            b_onlyDownloading = false
            qint64_onlyDownloadingId = 0
        }

        function onAllDownloadStopped() {
            toast.show(qsTr("All downloads stopped"))

            b_allDownloading = false
            b_onlyDownloading = false
            qint64_onlyDownloadingId = 0
        }

        function onPossiblePerform(ok) {
            if (mainPage.b_enableRateLimit) { mainPage.b_canPerform = ok; }

            if (!ok && mainPage.b_enableRateLimit) {
                backend.stopDownload()
                b_onlyDownloading = false
                b_allDownloading = false
                mainPage.qint64_onlyDownloadingId = 0

                showInfo.messageText = qsTr("You have reached the limit. Please wait at least one hour before trying again.")
                showInfo.open()
            }
        }
    }
}
