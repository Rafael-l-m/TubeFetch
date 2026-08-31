import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Menu {
    property bool lightMode

    property alias p_en_US_menuItem: en_US_menuItem
    property alias p_en_GB_menuItem: en_GB_menuItem
    property alias p_zh_CN_menuItem: zh_CN_menuItem
    property alias p_zh_TW_menuItem:  zh_TW_menuItem
    property alias p_zh_LIT_menuItem: zh_LIT_menuItem
    property alias p_es_ES_menuItem: es_ES_menuItem
    property alias p_fr_FR_menuItem: fr_FR_menuItem
    property alias p_it_IT_menuItem: it_IT_menuItem
    property alias p_pt_PT_menuItem: pt_PT_menuItem
    // property alias p_de_DE_menuItem: de_DE_menuItem
    // property alias p_ru_RU_menuItem: ru_RU_menuItem
    // property alias p_uk_UA_menuItem: uk_UA_menuItem
    // property alias p_ar_SA_menuItem: ar_SA_menuItem
    // property alias p_ko_KR_menuItem: ko_KR_menuItem
    // property alias p_ja_JP_menuItem: ja_JP_menuItem
    // property alias p_th_TH_menuItem: th_TH_menuItem
    // property alias p_nb_NO_menuItem: nb_NO_menuItem
    // property alias p_fi_FI_menuItem: fi_FI_menuItem
    // property alias p_sv_SE_menuItem: sv_SE_menuItem

    id: languageMenu

    title: qsTr("Language")

    // implicitHeight: Math.min(contentItem.implicitHeight, 300)

    Material.background: languageMenu.lightMode ? "#8fd4ff" : "#6b6b6b"

    MainMenuItem { id: en_US_menuItem; text: qsTr("English (US)"); lightMode: languageMenu.lightMode }

    MainMenuItem { id: en_GB_menuItem; text:qsTr("English (UK)"); lightMode: languageMenu.lightMode }

    MainMenuItem { id: zh_CN_menuItem; text: qsTr("中文"); lightMode: languageMenu.lightMode }

    MainMenuItem { id: zh_TW_menuItem; text: qsTr("繁體中文"); lightMode: languageMenu.lightMode }

    MainMenuItem { id: zh_LIT_menuItem; text: qsTr("華夏"); lightMode: languageMenu.lightMode }

    MainMenuItem { id: es_ES_menuItem; text: qsTr("Español"); lightMode: languageMenu.lightMode }

    MainMenuItem { id: fr_FR_menuItem; text: qsTr("Français"); lightMode: languageMenu.lightMode }

    MainMenuItem { id: it_IT_menuItem; text: qsTr("Italiano"); lightMode: languageMenu.lightMode }

    MainMenuItem { id: pt_PT_menuItem; text: qsTr("Português"); lightMode: languageMenu.lightMode }

    // MainMenuItem { id: de_DE_menuItem; text: qsTr("Deutsch"); lightMode: languageMenu.lightMode }

    // MainMenuItem { id: ru_RU_menuItem; text: qsTr("Русский"); lightMode: languageMenu.lightMode }

    // MainMenuItem { id: uk_UA_menuItem; text: qsTr("українська мова"); lightMode: languageMenu.lightMode }

    // MainMenuItem { id: ar_SA_menuItem; text: qsTr("العربية"); lightMode: languageMenu.lightMode }

    // MainMenuItem { id: ko_KR_menuItem; text: qsTr("한국어"); lightMode: languageMenu.lightMode }

    // MainMenuItem { id: ja_JP_menuItem; text: qsTr("日本語"); lightMode: languageMenu.lightMode }

    // MainMenuItem { id: th_TH_menuItem; text: qsTr("ภาษาไทย"); lightMode: languageMenu.lightMode }

    // MainMenuItem { id: nb_NO_menuItem; text: qsTr("Bokmål"); lightMode: languageMenu.lightMode }

    // MainMenuItem { id: fi_FI_menuItem; text: qsTr("Suomi"); lightMode: languageMenu.lightMode }

    // MainMenuItem { id: sv_SE_menuItem; text: qsTr("Svenska"); lightMode: languageMenu.lightMode }
}
