import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Menu {
    property bool lightMode

    property alias p_root: helpMenu
    property alias p_MenuItem_VisitRepo: goRepoMenuItem
    property alias p_MenuItem_HelpDoc: helpDocMenuItem
    property alias p_MenuItem_CheckUpdate: checkUpdateMenuItem
    property alias p_MenuItem_ReportIssues: reportIssuesMenuItem

    id: helpMenu
    title: qsTr("Help")
    Material.background: lightMode ? "#8fd4ff" : "#6b6b6b"

    MainPage_MenuItem { id: goRepoMenuItem; text: qsTr("Visit Repo"); lightMode: helpMenu.lightMode }

    MainPage_MenuItem { id: helpDocMenuItem; text: qsTr("Help Documentation"); lightMode: helpMenu.lightMode }

    MainPage_MenuItem { id: checkUpdateMenuItem; text: qsTr("Check for Updates"); lightMode: helpMenu.lightMode }

    MainPage_MenuItem { id: reportIssuesMenuItem; text: qsTr("Report Issues"); lightMode: helpMenu.lightMode }
}
