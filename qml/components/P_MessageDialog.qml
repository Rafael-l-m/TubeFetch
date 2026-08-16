import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Dialog {
    property alias p_root: messageDialog
    property alias p_TextMetrics: textMetrics
    property alias p_contentItem_Flickable: flickable
    property alias p_ContentItem_Flickable_ScrollBar: scrollBar
    property alias p_contentItem_Column: contentItem_Column
    property alias p_contentItem_Column_Text: contentItem_Column_Text
    property alias messageText: contentItem_Column_Text.text

    id: messageDialog

    modal: true
    focus: true

    closePolicy: Popup.NoAutoClose
    standardButtons: Dialog.Ok | Dialog.Cancel
    parent: Overlay.overlay

    TextMetrics { id: textMetrics; text: contentItem_Column_Text.text; font: contentItem_Column_Text.font }

    width: Math.min(
        Math.max(
            400,
            textMetrics.width + 120
        ),
        600,
        parent.width * 0.8
    )

    height: Math.min(
        Math.max(
            200,
            contentItem_Column_Text.contentHeight + 120
        ),
        450,
        parent.height * 0.8
    )

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    contentItem: Flickable {
        id: flickable

        clip: true

        contentWidth: flickable.width
        contentHeight: contentItem_Column.implicitHeight

        boundsBehavior: Flickable.StopAtBounds

        ScrollBar.vertical: ScrollBar { id: scrollBar; policy: ScrollBar.AsNeeded }

        Column {
            id: contentItem_Column

            width: flickable.width
            spacing: 10

            TextEdit {
                id: contentItem_Column_Text

                width: parent.width

                color: "#000000"
                font.pixelSize: 12

                wrapMode: TextEdit.Wrap

                readOnly: true

                selectByMouse: true
                selectByKeyboard: true

                textFormat: TextEdit.PlainText

                padding: 6
            }
        }
    }
}
