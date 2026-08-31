import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs

import "qml/components" as Comp
import "qml/pages" as Pg
import "qml/window/menubar" as Mb


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
    title: qsTr("TubeFetch (v2.1.0)")

    onClosing: (event) => { backend.requestQuitApp(); event.accepted = true }

    Component.onCompleted: { backend.loadSettings(); windowManager.setMainWindow(mainWindow) }

    menuBar: Mb.MainMenuBar {
        id: menuBar

        lightMode: mainWindow.lightMode


        // File Menu

        p_MainFileMenu.p_MenuItemExportData.onClicked: {
            if (stackView.currentItem.b_onlyDownloading || stackView.currentItem.b_allDownloading) {
                showInfo.messageText = qsTr("Unable to export data during download")
                showInfo.open()
                return
            }

            expDt.open()
        }

        p_MainFileMenu.p_MenuItemImportData.onClicked: {
            if (stackView.currentItem.p_Rectangle_ColumnLayout_R2.p_MouseArea_ListView.count > 0) {
                showInfo.messageText = qsTr("Importing data requires clearing the list")
                showInfo.open()
                return
            }

            if (stackView.currentItem.b_onlyDownloading || stackView.currentItem.b_allDownloading) {
                showInfo.messageText = qsTr("Unable to import data during download")
                showInfo.open()
                return
            }

            impDt.open()
        }

        p_MainFileMenu.p_MenuItemExportOutputs.onClicked: {
            if (stackView.currentItem.b_onlyDownloading || stackView.currentItem.b_allDownloading) {
                showInfo.messageText = qsTr("Unable to export output during download")
                showInfo.open()
                return
            }

            expOpt.open()
        }

        p_MainFileMenu.p_MenuItemClearDownloadStatus.onClicked: {
            if (stackView.currentItem.b_onlyDownloading || stackView.currentItem.b_allDownloading) {
                showInfo.messageText = qsTr("Unable to export output during download")
                showInfo.open()
                return
            }

            backend.clearAllDownloadStatus()
        }

        p_MainFileMenu.p_MenuItemPreferences.onClicked: {
            if (stackView.currentItem.b_onlyDownloading || stackView.currentItem.b_allDownloading) {
                showInfo.messageText = qsTr("Unable to export output during download")
                showInfo.open()
                return
            }

            windowManager.switchToAnotherWindow("PreferencesWindow.qml")
        }

        p_MainFileMenu.p_MenuItem_quit.onClicked: mainWindow.close()


        // Language Menu

        p_MainLanguageMenu.p_en_US_menuItem.onClicked:  { languageManager.setLanguage("en_US")  }
        p_MainLanguageMenu.p_en_GB_menuItem.onClicked:  { languageManager.setLanguage("en_GB")  }
        p_MainLanguageMenu.p_zh_CN_menuItem.onClicked:  { languageManager.setLanguage("zh_CN")  }
        p_MainLanguageMenu.p_zh_TW_menuItem.onClicked:  { languageManager.setLanguage("zh_TW")  }
        p_MainLanguageMenu.p_zh_LIT_menuItem.onClicked: { languageManager.setLanguage("zh_LIT") }
        p_MainLanguageMenu.p_es_ES_menuItem.onClicked:  { languageManager.setLanguage("es_ES")  }
        p_MainLanguageMenu.p_fr_FR_menuItem.onClicked:  { languageManager.setLanguage("fr_FR")  }
        p_MainLanguageMenu.p_it_IT_menuItem.onClicked:  { languageManager.setLanguage("it_IT")  }
        p_MainLanguageMenu.p_pt_PT_menuItem.onClicked:  { languageManager.setLanguage("pt_PT")  }


        // Help Menu

        p_MainHelpMenu.p_MenuItem_VisitRepo.onClicked: { backend.visitRepo() }
        p_MainHelpMenu.p_MenuItem_HelpDoc.onClicked: { backend.helpDoc() }
        p_MainHelpMenu.p_MenuItem_ReportIssues.onClicked: { backend.reportIssues() }

        p_MainHelpMenu.p_MenuItem_CheckUpdate.onClicked: {
            loading.running = true
            loading.visible = true

            backend.checkUpdate()
        }
    }

    StackView {
        id: stackView

        anchors.fill: parent

        initialItem: Pg.MainPage {
            lightMode: mainWindow.lightMode

            p_DownloadYtDlpWindow.onRejected:  { mainWindow.close() }
            p_DownloadPoTokenProviderWindow.onRejected: { mainWindow.close() }
        }
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

    FileDialog {
        id: expOpt

        fileMode: FileDialog.SaveFile

        title: qsTr("Load File")

        nameFilters: ["*.txt"]

        onAccepted: {
            loading.running = true
            loading.visible = true

            backend.exportOutputs(expOpt.selectedFile, modelToTxt())
        }
    }

    Comp.MessageDialog { id: showInfo; b_askType: false; lightMode: mainWindow.lightMode }

    Comp.MessageDialog { id: showUpdateInfo; b_askType: false; lightMode: mainWindow.lightMode}

    Comp.LoadingOverlay { id: loading; r_overlayWidth: mainWindow.width; r_overlayHeight: mainWindow.height; lightMode: mainWindow.lightMode }

    Connections {
        target: backend

        function onSettingsLoaded(obj) {
            const lma = obj.lightModeAutomatic
            const lm = obj.lightMode
            const sc = obj.selfCheck
            const rl = obj.rateLimit

            if (!lma) { mainWindow.lightMode = lm }

            else { mainWindow.lightMode = Qt.binding(function() {
                return Application.styleHints.colorScheme === Qt.Light
            })}

            stackView.currentItem.b_checkToolsWhenExecute = sc
            stackView.currentItem.b_enableRateLimit = rl
        }
    }

    Connections {
        target: backend
        enabled: mainWindow.visible

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

        function onOutputExported(ok, message) {
            loading.visible = false
            loading.running = false

            showInfo.messageText = message
            showInfo.open()
        }

        function onUpdateChecked(updateStatus, latestVersion, notes, downloadUrl, message) {
            loading.visible = false
            loading.running = false

            if (updateStatus === 1) {
                showUpdateInfo.width = 500
                showUpdateInfo.height = 300
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

    Connections {
        target: languageManager

        function onLanguageChanged() { Qt.uiLanguage = languageManager.getLanguage() }
    }

    function modelToTxt() {
        let text = ""

        const mod = stackView.currentItem.p_Rectangle_ColumnLayout_ColumnLayout_TerminalView.p_ListModel

        for (let i = 0; i < mod.count; ++i) {
            const item = mod.get(i)
            text += item.content + "\n"
        }

        return text
    }
}
