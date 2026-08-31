import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Menu {
    property bool lightMode

    property alias p_MenuItemExportData: exportDataItem
    property alias p_MenuItemImportData: importDataItem
    property alias p_MenuItemExportOutputs: exportOutputsItem
    property alias p_MenuItemClearDownloadStatus: clearDownloadStatusItem
    property alias p_MenuItemPreferences: preferencesItem
    property alias p_MenuItem_quit: exitItem

    id: fileMenu

    title: qsTr("File")

    Material.background: fileMenu.lightMode ? "#8fd4ff" : "#6b6b6b"

    MainMenuItem { id: exportDataItem;          text: qsTr("Export Data");           lightMode: fileMenu.lightMode }

    MainMenuItem { id: importDataItem;          text: qsTr("Import Data");           lightMode: fileMenu.lightMode }

    MainMenuItem { id: exportOutputsItem;       text: qsTr("Export Output");         lightMode: fileMenu.lightMode }

    MainMenuItem { id: clearDownloadStatusItem; text: qsTr("Clear Download Status"); lightMode: fileMenu.lightMode }

    MainMenuItem { id: preferencesItem;         text: qsTr("Preferences");           lightMode: fileMenu.lightMode }

    MenuSeparator {}

    MainMenuItem { id: exitItem;                text: qsTr("Quit");                  lightMode: fileMenu.lightMode }
}
