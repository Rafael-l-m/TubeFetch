import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Menu {
    property bool lightMode

    property alias p_root: fileMenu
    property alias p_MenuItem_exportData: exportDataItem
    property alias p_MenuItem_importData: importDataItem
    property alias p_MenuItem_clearDownloadStatus: clearDownloadStatusItem
    property alias p_MenuItem_quit: exitItem

    id: fileMenu
    title: qsTr("File")
    Material.background: lightMode ? "#8fd4ff" : "#6b6b6b"

    MainPage_MenuItem { id: exportDataItem; text: qsTr("Export Data"); lightMode: fileMenu.lightMode }

    MainPage_MenuItem { id: importDataItem; text: qsTr("Import Data"); lightMode: fileMenu.lightMode }

    MainPage_MenuItem { id: clearDownloadStatusItem; text: qsTr("Clear Download Status"); lightMode: fileMenu.lightMode }

    MenuSeparator {}

    MainPage_MenuItem { id: exitItem; text: qsTr("Quit"); lightMode: fileMenu.lightMode }
}
