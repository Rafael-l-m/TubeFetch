import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Menu {
    property bool lightMode

    property alias p_root: preferencesMenu
    // property alias p_menuItem_Download: downloadMenuItem
    // property alias p_menuItem_Pages: pagesMenuItem
    property alias p_menuItem_Tools: toolsMenuItem

    id: preferencesMenu
    title: qsTr("Preferences")
    Material.background: lightMode ? "#8fd4ff" : "#6b6b6b"

    // MainPage_MenuItem { id: downloadMenuItem; text: qsTr("Downloads"); lightMode: preferencesMenu.lightMode }

    // MainPage_MenuItem { id: pagesMenuItem;    text: qsTr("Pages");     lightMode: preferencesMenu.lightMode }

    MainPage_MenuItem { id: toolsMenuItem;    text: qsTr("Tools");     lightMode: preferencesMenu.lightMode }
}
