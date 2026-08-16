import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Menu {
    property bool lightMode

    property alias p_root: languageMenu
    property alias p_en_US_menuItem: en_US_menuItem
    property alias p_zh_CN_menuItem: zh_CN_menuItem
    property alias p_es_ES_menuItem: es_ES_menuItem
    // property alias p_fr_FR_menuItem: fr_FR_menuItem
    // property alias p_de_DE_menuItem: de_DE_menuItem
    // property alias p_it_IT_menuItem: it_IT_menuItem
    // property alias p_pt_PT_menuItem: pt_PT_menuItem
    // property alias p_ru_RU_menuItem: ru_RU_menuItem
    // property alias p_uk_UA_menuItem: uk_UA_menuItem
    // property alias p_ar_SA_menuItem: ar_SA_menuItem
    // property alias p_ko_KR_menuItem: ko_KR_menuItem
    // property alias p_ja_JP_menuItem: ja_JP_menuItem
    // property alias p_zh_TW_menuItem:  zh_TW_menuItem

    id: languageMenu
    title: qsTr("Language")
    // implicitHeight: Math.min(contentItem.implicitHeight, 300)
    Material.background: lightMode ? "#8fd4ff" : "#6b6b6b"

    MainPage_MenuItem { id: en_US_menuItem; text: qsTr("English"); lightMode: languageMenu.lightMode }

    MainPage_MenuItem { id: zh_CN_menuItem; text: qsTr("中文"); lightMode: languageMenu.lightMode }

    MainPage_MenuItem { id: es_ES_menuItem; text: qsTr("Español"); lightMode: languageMenu.lightMode }

    // MainPage_MenuItem { id: fr_FR_menuItem; text: qsTr("Français"); lightMode: languageMenu.lightMode }

    // MainPage_MenuItem { id: de_DE_menuItem; text: qsTr("Deutsch"); lightMode: languageMenu.lightMode }

    // MainPage_MenuItem { id: it_IT_menuItem; text: qsTr("Italiano"); lightMode: languageMenu.lightMode }

    // MainPage_MenuItem { id: pt_PT_menuItem; text: qsTr("Português"); lightMode: languageMenu.lightMode }

    // MainPage_MenuItem { id: ru_RU_menuItem; text: qsTr("Русский"); lightMode: languageMenu.lightMode }

    // MainPage_MenuItem { id: uk_UA_menuItem; text: qsTr("українська мова"); lightMode: languageMenu.lightMode }

    // MainPage_MenuItem { id: ar_SA_menuItem; text: qsTr("العربية"); lightMode: languageMenu.lightMode }

    // MainPage_MenuItem { id: ko_KR_menuItem; text: qsTr("한국어"); lightMode: languageMenu.lightMode }

    // MainPage_MenuItem { id: ja_JP_menuItem; text: qsTr("日本語"); lightMode: languageMenu.lightMode }

    // MainPage_MenuItem { id: zh_TW_menuItem; text: qsTr("繁體中文"); lightMode: languageMenu.lightMode }
}
