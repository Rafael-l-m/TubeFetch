import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Dialog {
    property bool lightMode

    property bool b_askType: true

    property alias p_contentItem_Flickable: flick
    property alias p_contentItem_Flickable_Column: contentColumn
    property alias p_contentItem_Flickable_Column_HighlightText: content
    property alias p_contentItem_Flickable_ScrollBar_Vertical_ScrollBar: contentScrollBar
    property alias p_footer_DialogButtonBox: buttons
    property alias p_footer_DialogButtonBox_AcceptButton: acceptButton
    property alias p_footer_DialogButtonBox_RejectButton: rejectButton
    property alias p_footer_DialogButtonBox_AcceptButton_background_Rectangle: acceptButtonBackground
    property alias p_footer_DialogButtonBox_RejectButton_background_Rectangle: rejectButtonBackground

    property alias messageText: content.sourceText
    property alias highlights: content.highlights

    id: messageDialog

    parent: Overlay.overlay

    modal: true
    focus: true

    closePolicy: Popup.NoAutoClose

    height: 150
    width: 300

    x: (parent.width - messageDialog.width) / 2
    y: (parent.height - messageDialog.height) / 2

    contentItem: Flickable {
        id: flick

        clip: true

        width: messageDialog.availableWidth
        height: Math.min(contentColumn.implicitHeight, messageDialog.availableHeight)

        contentWidth: width
        contentHeight: contentColumn.implicitHeight

        interactive: flick.contentHeight > flick.height

        boundsBehavior: Flickable.StopAtBounds

        Column {
            id: contentColumn

            width: flick.width

            spacing: 10

            HighlightText {
                id: content

                width: parent.width

                lightMode: messageDialog.lightMode

                p_TextEdit_Rectangle.color: "transparent"
            }
        }

        ScrollBar.vertical: ScrollBar {
            id: contentScrollBar

            width: 7

            policy: ScrollBar.AsNeeded
        }
    }

    footer: DialogButtonBox {
        id: buttons

        Button {
            id: acceptButton

            text: qsTr("OK")

            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole

            background: Rectangle {
                id: acceptButtonBackground

                radius: acceptButton.height / 2

                color: {
                    if (acceptButton.down) return "#b8b8b8"
                    if (acceptButton.hovered) return "#d1d1d1"
                    return "transparent"
                }
            }
        }

        Button {
            id: rejectButton

            text: qsTr("Cancel")

            visible: messageDialog.b_askType

            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole

            background: Rectangle {
                id: rejectButtonBackground

                radius: rejectButton.height / 2

                color: {
                    if (rejectButton.down) return "#b8b8b8"
                    if (rejectButton.hovered) return "#d1d1d1"
                    return "transparent"
                }
            }
        }
    }
}
