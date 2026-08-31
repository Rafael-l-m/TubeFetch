import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Menu {
    property bool lightMode

    property alias p_MenuItem_VisitRepo: goRepoMenuItem
    property alias p_MenuItem_HelpDoc: helpDocMenuItem
    property alias p_MenuItem_CheckUpdate: checkUpdateMenuItem
    property alias p_MenuItem_ReportIssues: reportIssuesMenuItem

    id: helpMenu

    title: qsTr("Help")

    Material.background: helpMenu.lightMode ? "#8fd4ff" : "#6b6b6b"

    MainMenuItem { id: goRepoMenuItem;       text: qsTr("Visit Repo");         lightMode: helpMenu.lightMode }

    MainMenuItem { id: helpDocMenuItem;      text: qsTr("Help Documentation"); lightMode: helpMenu.lightMode }

    MainMenuItem { id: checkUpdateMenuItem;  text: qsTr("Check for Updates");  lightMode: helpMenu.lightMode }

    MainMenuItem { id: reportIssuesMenuItem; text: qsTr("Report Issues");      lightMode: helpMenu.lightMode }
}
