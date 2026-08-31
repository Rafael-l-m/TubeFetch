import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

ComboBox {
    property bool lightMode

    property bool b_delegate_ItemDelegate_fontBold: true
    property bool b_delegate_ItemDelegate_contentItem_Text_fontBold: true

    property color c_delegate_ItemDelegate_background_Rectangle_hoveredLight: "#06b8ff"
    property color c_delegate_ItemDelegate_background_Rectangle_N_hoveredLight: "#93dbfa"
    property color c_delegate_ItemDelegate_background_Rectangle_hoveredDark: "#c4c4c4"
    property color c_delegate_ItemDelegate_background_Rectangle_N_hoveredDark: "#eeeeee"
    property color c_ModelDataTextColorLight: "#2e3a45"
    property color c_ModelDataTextColorDark: "#0f0f0f"

    property real r_delegate_ItemDelegate_fontPixelSize: 12
    property real r_delegate_ItemDelegate_Rectangle_radius: 8
    property real r_delegate_ItemDelegate_contentItem_Text_fontPixelSize: 12
    property real r_delegate_ItemDelegate_contentItem_Text_leftPadding: 0

    property string s_delegate_ItemDelegate_fontFamily: "Avenir Next"
    property string s_delegate_ItemDelegate_contentItem_Text_fontFamily: "Avenir Next"

    property alias p_comboBox_background_Rectangle: background_Rectangle
    property alias p_comboBox_contentItem_Text: contentItem_Text
    property alias p_popup_Popup: popup_Popup
    property alias p_popup_Popup_background_Rectangle: popup_Popup_background_Rectangle
    property alias p_popup_Popup_contentItem_ListView: popup_Popup_contentItem_ListView
    property alias modelData: comboBox.model

    id: comboBox

    implicitWidth: 200
    implicitHeight: 40

    background: Rectangle {
        id: background_Rectangle

        radius: 6

        color: comboBox.lightMode ? "#93dbfa" : "#686868"
    }

    contentItem: Text {
        id: contentItem_Text

        text: comboBox.displayText

        font.pixelSize: 12
        font.bold: true

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        anchors.verticalCenter: parent.verticalCenter

        color: comboBox.lightMode ? "#2e3a45" : "#ffffff"
    }

    delegate: ItemDelegate {
        width: parent.width

        text: modelData

        font.pixelSize: comboBox.r_delegate_ItemDelegate_fontPixelSize
        font.bold: comboBox.b_delegate_ItemDelegate_fontBold
        font.family: comboBox.s_delegate_ItemDelegate_fontFamily

        highlighted: ComboBox.highlightedIndex === index && 1 == 0

        background: Rectangle {
            radius: comboBox.r_delegate_ItemDelegate_Rectangle_radius

            color: comboBox.lightMode
                        ? (hovered ? comboBox.c_delegate_ItemDelegate_background_Rectangle_hoveredLight
                                   : comboBox.c_delegate_ItemDelegate_background_Rectangle_N_hoveredLight)
                        : (hovered ? comboBox.c_delegate_ItemDelegate_background_Rectangle_hoveredDark
                                   : comboBox.c_delegate_ItemDelegate_background_Rectangle_N_hoveredDark)
        }

        contentItem: Text {
            text: modelData

            font.pixelSize: comboBox.r_delegate_ItemDelegate_contentItem_Text_fontPixelSize
            font.bold: comboBox.b_delegate_ItemDelegate_contentItem_Text_fontBold
            font.family: comboBox.s_delegate_ItemDelegate_contentItem_Text_fontFamily

            leftPadding: comboBox.r_delegate_ItemDelegate_contentItem_Text_leftPadding

            color: comboBox.lightMode ? comboBox.c_ModelDataTextColorLight : comboBox.c_ModelDataTextColorDark

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    popup: Popup {
        id: popup_Popup

        y: comboBox.height

        width: comboBox.width
        height: Math.min(contentItem.contentHeight + 25, 250)

        background: Rectangle {
            id: popup_Popup_background_Rectangle

            radius: 8

            color: comboBox.lightMode ? "#93dbfa" : "#eeeeee"
        }

        contentItem: ListView {
            id: popup_Popup_contentItem_ListView

            clip: true

            implicitHeight: contentHeight

            model: comboBox.popup.visible ? comboBox.delegateModel : null

            currentIndex: comboBox.highlightedIndex
        }
    }
}
