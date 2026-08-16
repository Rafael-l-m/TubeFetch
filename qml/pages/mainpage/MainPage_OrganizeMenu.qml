import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Menu {
    property bool lightMode

    property alias p_root: organizeMenu
    property alias p_MenuItem_ByAddOrderAsc: menuItem_ByAddOrderAsc
    property alias p_MenuItem_ByAddOrderDesc: menuItem_ByAddOrderDesc
    property alias p_menuItem_ByNameAsc: menuItem_ByNameAsc
    property alias p_menuItem_ByNameDesc: menuItem_ByNameDesc
    property alias p_menuItem_BySizeAsc: menuItem_BySizeAsc
    property alias p_menuItem_BySizeDesc: menuItem_BySizeDesc

    id: organizeMenu
    title: qsTr("Sort")
    Material.background: lightMode ? "#8fd4ff" : "#6b6b6b"

    MainPage_MenuItem {
        id: menuItem_ByAddOrderAsc
        text: qsTr("By add order asc")
        lightMode: organizeMenu.lightMode
    }

    MainPage_MenuItem {
        id: menuItem_ByAddOrderDesc
        text: qsTr("By add order desc")
        lightMode: organizeMenu.lightMode
    }

    MainPage_MenuItem {
        id: menuItem_ByNameAsc
        text: qsTr("By name asc")
        lightMode: organizeMenu.lightMode
    }

    MainPage_MenuItem {
        id: menuItem_ByNameDesc
        text: qsTr("By name desc")
        lightMode: organizeMenu.lightMode
    }

    MainPage_MenuItem {
        id: menuItem_BySizeAsc
        text: qsTr("By size asc")
        lightMode: organizeMenu.lightMode
    }

    MainPage_MenuItem {
        id: menuItem_BySizeDesc
        text: qsTr("By size desc")
        lightMode: organizeMenu.lightMode
    }
}
