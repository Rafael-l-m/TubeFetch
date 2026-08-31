import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

MenuItem {
    property bool lightMode

    property alias p_contentItem: contentItem_Text

    id: menuItem

    height: 40

    contentItem: Text {
        id: contentItem_Text

        text: parent.text

        verticalAlignment: Text.AlignVCenter

        color: lightMode ? "#000000" : "#ffffff"
    }
}
