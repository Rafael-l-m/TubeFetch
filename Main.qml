import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts
import "qml/components" as Comp
import "qml/pages/mainpage" as Mp

ApplicationWindow {
    id: window
    visible: true

    width: 1000
    height: 960

    minimumWidth: 940
    minimumHeight: 960

    maximumWidth: Screen.width
    maximumHeight: Screen.height

    title: qsTr("TubeFetch (v3.0.0)")

    onClosing: (event) => { backend.requestQuitApp(); event.accepted = true }

    Component.onCompleted: { windowManager.setMainWindow(window) }

    // ======================================================
    // ======================================================
    // ======================================================
    // ======================================================
    // ======================================================
    // Change URLInfoManager->analizeInfo() -> Add 'subtitles'
    // PreferencesWindow -> Redesign Settings
    // ======================================================
    // ======================================================
    // ======================================================
    // ======================================================
    // ======================================================

    menuBar: MenuBar {
        id: menuBar

        visible: mainStackView.currentItem.showMenuBar ? true : false

        height: 40

        background: Rectangle { color: "#303030" }

        delegate: MenuBarItem {
            id: menuBarItem

            contentItem: Text {
                text: menuBarItem.text

                color: "#ffffff"

                font.pixelSize: 12
                font.bold: true

                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }

        Menu {
            title: qsTr("File")

            Material.background: "#6b6b6b"

            MenuItem {
                text: qsTr("Export Data")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }

                onTriggered: { expDt.open() }
            }

            MenuItem {
                text: qsTr("Import Data")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }

                onTriggered: {
                    if (mainStackView.currentItem.downloadList.downloadListView.count > 0) {
                        showInfo.messageText = qsTr("Importing data requires clearing the list")
                        showInfo.open()
                        return
                    }

                    impDt.open()

                }
            }

            MenuItem {
                text: qsTr("Export Output")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }

                onTriggered: { expOpt.open() }
            }

            MenuSeparator { }

            MenuItem {
                text: qsTr("Clear Download Status")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }

                onTriggered: { backend.clearAllDownloadStatus() }
            }

            MenuItem {
                text: qsTr("Clear Caches")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }

                onTriggered: {
                    backend.removeAllDownloadUrlInfo()
                    showInfo.messageText = qsTr("Successfully cleared caches")
                    showInfo.open()
                }
            }

            MenuSeparator { }

            MenuItem {
                text: qsTr("Preferences")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuSeparator { }

            MenuItem {
                text: qsTr("Quit")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }

                onTriggered: { window.close() }
            }
        }

        Menu {
            title: qsTr("Language")

            Material.background: "#6b6b6b"

            implicitHeight: Math.min(contentItem.implicitHeight, 300)

            MenuItem {
                text: qsTr("English (US)")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("English (UK)")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("简体中文")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("Español")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("Português")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("Français")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("Italiano")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("Deutsch")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("Русский")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("українська мова")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("한국어")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("日本語")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("ภาษาไทย")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("Bokmål")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("Suomi")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("Svenska")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("العربية")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("繁體中文")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }

            MenuItem {
                text: qsTr("華夏")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }
            }
        }

        Menu {
            title: qsTr("Help")

            Material.background: "#6b6b6b"

            MenuItem {
                text: qsTr("Visit Repository")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }

                onTriggered: { backend.visitRepo() }
            }

            MenuItem {
                text: qsTr("Help Documentation")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }

                onTriggered: { backend.helpDoc() }
            }

            MenuItem {
                text: qsTr("Check Updates")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }

                onTriggered: {
                    loading.running = true
                    loading.visible = true

                    backend.checkUpdate()
                }
            }

            MenuItem {
                text: qsTr("Report Issues")

                height: 40

                contentItem: Text {
                    text: parent.text

                    verticalAlignment: Text.AlignVCenter

                    color: "#ffffff"
                }

                onTriggered: { backend.reportIssues() }
            }
        }
    }

    Comp.Background { }

    StackView {
        id: mainStackView

        anchors.fill: parent

        initialItem: Mp.MainPage { id: mainPage }
    }


    // ================
    // Export Data
    // ================

    FileDialog {
        id: expDt

        fileMode: FileDialog.SaveFile

        title: qsTr("Save As")

        nameFilters: ["*.dat", "*.data"]

        onAccepted: {
            loading.running = true
            loading.visible = true

            backend.exportData(expDt.selectedFile)
        }
    }


    // ================
    // Import Data
    // ================

    FileDialog {
        id: impDt

        fileMode: FileDialog.OpenFile

        title: qsTr("Load File")

        nameFilters: ["*.dat", "*.data"]

        onAccepted: {
            loading.running = true
            loading.visible = true

            backend.importData(impDt.selectedFile)
        }
    }


    // ===================
    // Export Outputs
    // ===================

    FileDialog {
        id: expOpt

        fileMode: FileDialog.SaveFile

        title: qsTr("Export Outputs")

        nameFilters: ["*.txt"]

        onAccepted: {
            loading.running = true
            loading.visible = true

            backend.exportOutputs(expOpt.selectedFile, modelToTxt())
        }
    }

    Comp.MessageDialog { id: showInfo; b_askType: false }

    Comp.MessageDialog { id: showUpdateInfo; b_askType: false; width: 500; height: 300 }

    Comp.LoadingOverlay { id: loading; overlayWidth: window.width; overlayHeight: window.height }

    Connections {
        target: backend
        enabled: window.visible

        function onDataExported(ok, message) {
            loading.visible = false
            loading.running = false

            showInfo.messageText = message.trim()
            showInfo.open()
        }

        function onDataImported(ok, message) {
            loading.visible = false
            loading.running = false

            showInfo.messageText = message.trim()
            showInfo.open()
        }

        function onOutputExported(ok, message) {
            loading.visible = false
            loading.running = false

            showInfo.messageText = message.trim()
            showInfo.open()
        }

        function onUpdateChecked(updateStatus, latestVersion, notes, downloadUrl, message) {
            loading.visible = false
            loading.running = false

            if (updateStatus === 1) {
                showUpdateInfo.messageText = qsTr("Exists new version: ") + latestVersion + "\n\n" + qsTr("Download Url: ") + downloadUrl + "\n\n" + qsTr("Download notes: ") + notes
                showUpdateInfo.open()
            }

            else if (updateStatus === -1) {
                showInfo.messageText = qsTr("Failed to check updates") + "\n\n" + qsTr("Message: ") + message
                showInfo.open()
            }

            else if (updateStatus === 0) {
                showInfo.messageText = message
                showInfo.open()
            }
        }
    }


    function modelToTxt() {
        let text = ""

        const mod = mainStackView.currentItem.terminalView.terminalModelList

        for (let i = 0; i < mod.count; ++i) {
            const item = mod.get(i)
            text += item.content + "\n"
        }

        return text
    }
}
