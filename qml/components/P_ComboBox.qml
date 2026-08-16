import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

ComboBox {
    property bool lightMode

    property alias p_root: comboBox
    property alias p_comboBox_background_Rectangle: background_Rectangle
    property alias p_comboBox_contentItem_Text: contentItem_Text
    property alias p_popup_Popup: popup_Popup
    property alias p_popup_Popup_background_Rectangle: popup_Popup_background_Rectangle
    property alias p_popup_Popup_contentItem_ListView: popup_Popuo_contentItem_ListView
    property alias modelData: comboBox.model

    property bool b_delegate_ItemDelegate_fontBold: true
    property bool b_delegate_ItemDelegate_contentItem_Text_fontBold: true

    property real r_delegate_ItemDelegate_fontPixelSize: 12
    property real r_delegate_ItemDelegate_Rectangle_radius: 8
    property real r_delegate_ItemDelegate_contentItem_Text_fontPixelSize: 12
    property real r_delegate_ItemDelegate_contentItem_Text_leftPadding: 0

    property string s_delegate_ItemDelegate_fontFamily: "Avenir Next"
    property string s_delegate_ItemDelegate_contentItem_Text_fontFamily: "Avenir Next"
    property string s_delegate_ItemDelegate_background_Rectangle_hoveredLight: "#eeeeee"
    property string s_delegate_ItemDelegate_background_Rectangle_N_hoveredLight: "#ffffff"
    property string s_delegate_ItemDelegate_background_Rectangle_hoveredDark: "#c4c4c4"
    property string s_delegate_ItemDelegate_background_Rectangle_N_hoveredDark: "#eeeeee"
    property string s_delegate_ItemDelegate_contentItem_Text_highlighted: "#ffffff"
    property string s_delegate_ItemDelegate_contentItem_Text_N_highlighted: "#000000"

    id: comboBox

    implicitWidth: 200
    implicitHeight: 40

    background: Rectangle {
        id: background_Rectangle

        radius: 6

        color: lightMode ? "#ffffff" : "#686868"
    }

    contentItem: Text {
        id: contentItem_Text

        text: comboBox.displayText

        font.pixelSize: 12
        font.bold: true
        font.family: "Avenir Next"

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        anchors.verticalCenter: parent.verticalCenter

        color: lightMode ? "#000000" : "#ffffff"
    }

    delegate: ItemDelegate {
        width: parent.width

        text: modelData

        font.pixelSize: r_delegate_ItemDelegate_fontPixelSize
        font.bold: b_delegate_ItemDelegate_fontBold
        font.family: s_delegate_ItemDelegate_fontFamily

        highlighted: ComboBox.highlightedIndex === index && 1 == 0

        background: Rectangle {
            radius: r_delegate_ItemDelegate_Rectangle_radius

            color: lightMode
                        ? (hovered ? s_delegate_ItemDelegate_background_Rectangle_hoveredLight
                                   : s_delegate_ItemDelegate_background_Rectangle_N_hoveredLight)
                        : (hovered ? s_delegate_ItemDelegate_background_Rectangle_hoveredDark
                                   : s_delegate_ItemDelegate_background_Rectangle_N_hoveredDark)
        }

        contentItem: Text {
            text: modelData

            font.pixelSize: r_delegate_ItemDelegate_contentItem_Text_fontPixelSize
            font.bold: b_delegate_ItemDelegate_contentItem_Text_fontBold
            font.family: s_delegate_ItemDelegate_contentItem_Text_fontFamily

            leftPadding: r_delegate_ItemDelegate_contentItem_Text_leftPadding

            color: highlighted ? s_delegate_ItemDelegate_contentItem_Text_highlighted : s_delegate_ItemDelegate_contentItem_Text_N_highlighted

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

            color: lightMode ? "#ffffff" : "#eeeeee"
        }

        contentItem: ListView {
            id: popup_Popuo_contentItem_ListView

            clip: true

            implicitHeight: contentHeight

            model: comboBox.popup.visible ? comboBox.delegateModel : null

            currentIndex: comboBox.highlightedIndex
        }
    }
}
