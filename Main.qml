import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import QtQuick.Dialogs
import "qml/components" as Comp
import "qml/pages" as Pg
import "qml/pages/mainpage" as Mp

ApplicationWindow {
    property bool lightMode: Application.styleHints.colorScheme === Qt.Light

    id: mainWindow

    width: 900
    height: 700

    minimumWidth: 800
    minimumHeight: 350

    maximumWidth: Screen.width
    maximumHeight: Screen.height

    visible: true
    title: qsTr("Youtube Media Downloader (v2.0.0)")

    onClosing: (event) => { backend.requestQuitApp(); event.accepted = true }

    Component.onCompleted: { windowManager.setMainWindow(mainWindow) }

    menuBar: Mp.MainPage_MenuBar {
        id: menuBar

        lightMode: mainWindow.lightMode


        // File Menu

        p_fileMenu.p_MenuItem_exportData.onClicked: { expDt.open() }
        p_fileMenu.p_MenuItem_importData.onClicked: { impDt.open() }
        p_fileMenu.p_MenuItem_clearDownloadStatus.onClicked: { backend.clearAllStatus() }
        p_fileMenu.p_MenuItem_quit.onClicked: { mainWindow.close() }


        // Language Menu

        p_languageMenu.p_en_US_menuItem.onClicked: { languageManager.setLanguage("en_US") }

        p_languageMenu.p_zh_CN_menuItem.onClicked: { languageManager.setLanguage("zh_CN") }

        p_languageMenu.p_es_ES_menuItem.onClicked: { languageManager.setLanguage("es_ES") }


        // Preferences Menu

        p_preferencesMenu.p_menuItem_Tools.onClicked: { windowManager.switchToAnotherWindow("preferences/ToolsWindow.qml") }


        // Help Menu

        p_helpMenu.p_MenuItem_VisitRepo.onClicked: { updateChecker.visitRepo() }

        p_helpMenu.p_MenuItem_HelpDoc.onClicked: { updateChecker.helpDoc() }

        p_helpMenu.p_MenuItem_CheckUpdate.onClicked: {
            loading.running = true
            loading.visible = true

            updateChecker.checkUpdate()
        }

        p_helpMenu.p_MenuItem_ReportIssues.onClicked: { updateChecker.reportIssues() }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: Pg.MainPage { lightMode: mainWindow.lightMode }
    }

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

    Comp.P_MessageDialog { id: showInfo; standardButtons: Dialog.Ok }

    Comp.P_LoadingOverlay { id: loading; overlayWidth: mainWindow.width; overlayHeight: mainWindow.height; lightMode: mainWindow.lightMode }

    Connections {
        target: backend
        enabled: visible

        function onDataExported(ok, message) {
            loading.visible = false
            loading.running = false

            showInfo.messageText = message
            showInfo.open()
        }

        function onDataImported(ok, message) {
            loading.visible = false
            loading.running = false

            showInfo.messageText = message
            showInfo.open()
        }
    }

    Connections {
        target: updateChecker
        enabled: visible

        function onUpdateChecked(updateStatus, latestVersion, notes, downloadUrl, message) {
            loading.visible = false
            loading.running = false

            if (updateStatus === 1) {
                showInfo.messageText = qsTr("Exists new version") + ": " + latestVersion + "\n\n" + qsTr("Download Url") + ": " + downloadUrl + "\n\n" + qsTr("Download notes") + ": " + notes
                showInfo.open()
            }

            else if (updateStatus === -1) {
                showInfo.messageText = qsTr("Failed to check updates") + "\n\n" + qsTr("Message") + ": " + message
                showInfo.open()
            }

            else if (updateStatus === 0) {
                showInfo.messageText = message
                showInfo.open()
            }
        }
    }

    Connections {
        target: languageManager

        function onLanguageChanged() { Qt.uiLanguage = languageManager.getLanguage() }
    }
}
