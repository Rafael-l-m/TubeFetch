import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    property bool lightMode
    property bool b_backgroundColorWithTextContentSize: true

    property alias p_TextEdit: textEdit
    property alias p_TextEdit_MouseArea: selectArea
    property alias p_TextEdit_Rectangle: textBackground
    property alias p_Menu: contextMenu
    property alias p_Menu_MenuItem_Copy: contextMenu_Copy
    property alias p_Menu_MenuItem_SelectAll: contextMenu_SelectAll

    property string sourceText: ""
    property var highlights: []

    id: highlightText

    width: highlightText.parent ? highlightText.parent.width : 0
    height: highlightText.implicitHeight

    implicitWidth: textEdit.contentWidth
    implicitHeight: textEdit.contentHeight

    TextEdit {
        id: textEdit

        readOnly: true

        textFormat: TextEdit.RichText
        wrapMode: TextEdit.Wrap

        selectByKeyboard: true
        selectByMouse: true

        clip: true

        width: parent.width

        onLinkActivated: function(link) { Qt.openUrlExternally(link) }

        function escapeRegExp(str) { return str.replace(/[.*+?^${}()|[\]\\]/g, "\\$&") }

        text: {
            let result = highlightText.sourceText

            for (var i = 0; i < highlights.length; i++) {
                var h = highlights[i]

                if (!h.word) { continue }

                var style = ""

                if (h.color) style += "color:" + h.color + ";"
                if (h.background) style += "background:" + h.background + ";"
                if (h.bold) style += "font-weight:bold;"
                if (h.underline) style += "text-decoration:underline;"
                if (h.fontSize) style += "font-size:" + h.fontSize + "px;"

                var span

                if (h.url) {
                    span =
                        "<a href='" + h.url + "' style='" + style + "'>" +
                        h.word +
                        "</a>"
                }

                else {
                    span = "<span style='" + style + "'>" +
                           h.word +
                           "</span>"
                }

                var regex = new RegExp(escapeRegExp(h.word), "g")

                result = result.replace(regex, span)
            }

            result = result.replace(/\n/g, "<br>")

            return result
        }

        MouseArea {
            id: selectArea

            anchors.fill: parent

            acceptedButtons: Qt.RightButton

            hoverEnabled: true

            cursorShape: Qt.IBeamCursor

            onClicked: function(mouse) {
                if (mouse.button === Qt.RightButton) {
                    contextMenu.popup(mouse.x, mouse.y)
                }
            }
        }

        Rectangle {
            id: textBackground

            width: highlightText.b_backgroundColorWithTextContentSize  ? textEdit.contentWidth  : textEdit.width
            height: highlightText.b_backgroundColorWithTextContentSize ? textEdit.contentHeight : textEdit.height

            color: highlightText.lightMode ? "#ffcfe0" : "#a7a7a7"

            z: -1
        }
    }

    Menu {
        id: contextMenu

        MenuItem {
            id: contextMenu_Copy

            height: 30

            font.bold: true
            font.pixelSize: 10
            font.family: "Avenir Next"

            text: qsTr("Copy")

            onTriggered: textEdit.copy()
        }

        MenuItem {
            id: contextMenu_SelectAll

            height: 30

            font.bold: true
            font.pixelSize: 10
            font.family: "Avenir Next"

            text: qsTr("Select All")

            onTriggered: textEdit.selectAll()
        }
    }
}
