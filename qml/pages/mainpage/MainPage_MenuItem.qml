import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

MenuItem {
    property bool lightMode

    property alias p_root: menuItem
    property alias p_contentItem: contentItem_Text

    id: menuItem

    height: 40

    font: "Avenir Next"

    contentItem: Text {
        id: contentItem_Text

        text: parent.text

        font: "Avenir Next"

        verticalAlignment: Text.AlignVCenter

        color: lightMode ? "#000000" : "#ffffff"
    }
}
