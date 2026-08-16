import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

MenuBarItem {
    property bool lightMode

    property alias p_root: menuBarItem
    property alias p_contentItem_Text: contentItem_Text

    id: menuBarItem

    contentItem: Text {
        id: contentItem_Text

        text: menuBarItem.text

        color: lightMode ? "#000000" : "#ffffff"

        font.pixelSize: 12

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
