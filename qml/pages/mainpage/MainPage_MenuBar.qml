import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

MenuBar {
    property bool lightMode

    property alias p_root: menuBar
    property alias p_fileMenu: fileMenu
    property alias p_languageMenu: languageMenu
    // property alias p_organizeMenu: organizeMenu
    property alias p_preferencesMenu: preferencesMenu
    property alias p_helpMenu: helpMenu

    id: menuBar
    height: 40
    delegate: MainPage_MenuBarItem { id: menuBarItem; lightMode: menuBar.lightMode }
    background: Rectangle { color: lightMode ? "#c3e7fa" : "#303030" }

    MainPage_FileMenu { id: fileMenu; lightMode: menuBar.lightMode }

    MainPage_LanguageMenu { id: languageMenu; lightMode: menuBar.lightMode }

    // MainPage_OrganizeMenu { id: organizeMenu; lightMode: menuBar.lightMode }

    MainPage_PreferencesMenu { id: preferencesMenu; lightMode: menuBar.lightMode }

    MainPage_HelpMenu { id: helpMenu; lightMode: menuBar.lightMode }
}
