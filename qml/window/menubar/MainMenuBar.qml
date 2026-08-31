import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

MenuBar {
    property bool lightMode

    property alias p_MainFileMenu: fileMenu
    property alias p_MainLanguageMenu: languageMenu
    property alias p_MainHelpMenu: helpMenu

    Material.background: menuBar.lightMode ? "#8fd4ff" : "#535353"

    id: menuBar

    height: 40

    delegate: MainMenuBarItem { id: menuBarItem; lightMode: menuBar.lightMode }

    background: Rectangle { color: menuBar.lightMode ? "#c3e7fa" : "#303030" }

    MainFileMenu {     id: fileMenu;     lightMode: menuBar.lightMode }

    MainLanguageMenu { id: languageMenu; lightMode: menuBar.lightMode }

    MainHelpMenu {     id: helpMenu;     lightMode: menuBar.lightMode }
}
