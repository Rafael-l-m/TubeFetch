import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import "../components" as Comp
import "../download" as Do
import "mainpage" as Mp

Page {
    property bool lightMode

    property alias p_root: mainPage
    property alias p_Rectangle: rectangle
    property alias p_Rectangle_ColumnLayout: columnLayout
    property alias p_Rectangle_ColumnLayout_R1: r1
    property alias p_Rectangle_ColumnLayout_R2_R3_ColumnLayout: r2_r3_columnLayout
    property alias p_Rectangle_ColumnLayout_R2: r2
    property alias p_Rectangle_ColumnLayout_R3: r3
    property alias p_Rectangle_ColumnLayout_R4: r4
    property alias p_MessageDialog1: askIfRemoveAllDownloads
    property alias p_MessageDialog2: showInfo
    property alias p_toast: toast

    id: mainPage

    Material.theme: Material.Light
    Material.accent: "#007AFF"

    font.pixelSize: 12

    Component.onCompleted: {
        if (backend.needConfig()) {
            windowManager.switchToAnotherWindow(
                "preferences/ToolsWindow.qml",
                true,
                true,
                true,
                { "b_beforeMainPage": true }
            )
        }
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

            Mp.ButtonLayout_Control {
                id: r1

                lightMode: mainPage.lightMode

                p_startBtn.onClicked: {
                    if (r2.p_MouseArea_ListView.currentIndex >= 0 && r2.selectedInternalId > 0) {
                        backend.startDownload(r2.selectedInternalId)
                    }

                    else { backend.startDownloads() }
                }

                p_stopBtn.enabled: true
                p_stopBtn.onClicked: { backend.stopDownloads() }

                p_editBtn.enabled: r2.p_MouseArea_ListView.currentIndex >= 0 && r2.selectedInternalId > 0
                p_editBtn.onClicked: {
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

                p_removeBtn.enabled: r2.p_MouseArea_ListView.currentIndex >= 0 && r2.selectedInternalId > 0
                p_removeBtn.onClicked: { backend.removeDownload(r2.selectedInternalId) }
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

                Comp.P_TerminalView {
                    id: r3
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    lightMode: mainPage.lightMode
                }
            }

            Mp.ButtonLayout_UI {
                id: r4
                lightMode: mainPage.lightMode

                p_addBtn.onClicked: { windowManager.switchToAnotherWindow("AddNewDownloadWindow.qml") }

                p_removeAllBtn.enabled: { r2.p_MouseArea_ListView.count > 0 }
                p_removeAllBtn.onClicked: { askIfRemoveAllDownloads.open() }
            }
        }
    }

    Comp.P_MessageDialog {
        id: askIfRemoveAllDownloads

        messageText: qsTr("Are you sure you want to remove all downloads?")

        onAccepted: { backend.removeAllDownloads() }
    }

    Comp.P_MessageDialog { id: showInfo; standardButtons: Dialog.Ok }

    Comp.P_Toast { id: toast }

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
        enabled: visible

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

        function onIsAlreadyDownloading() {
            showInfo.messageText = qsTr("Download in progress. Please wait")
            showInfo.open()
        }

        function onNoAvailableDownloads() {
            showInfo.messageText = qsTr("No valid download items")
            showInfo.open()
        }

        function onDownloadAllFinished() { toast.show("All downloads finished") }
    }
}
