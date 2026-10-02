import QtQuick
import QtQuick.Controls
import "../components" as Comp

ApplicationWindow {
    id: showCodecsWindow

    visible: true

    width: 1000
    height: 700

    maximumWidth: Screen.width
    maximumHeight: Screen.height

    title: qsTr("Show Codecs")
    flags: Qt.Dialog

    Comp.Background { }

    Rectangle {
        id: rectangle

        anchors.fill: parent

        color: "transparent"

        ScrollView {
            id: scrollView

            anchors.fill: parent

            clip: true

            leftPadding: 10
            rightPadding: 10
            topPadding: 10
            bottomPadding: 10

            TextEdit {
                id: textEdit

                readOnly: true

                wrapMode: TextEdit.NoWrap

                textFormat: TextEdit.PlainText

                font.family: "Menlo"  // "Consolas" -> Windows
                font.pixelSize: 12
                font.bold: true

                color: "#ffffff"

                selectByMouse: true

                width: Math.max(parent.availableWidth, implicitWidth)
                height: implicitHeight
            }
        }
    }

    Connections {
        target: backend
        enabled: showCodecsWindow.visible

        function onCodecsShown(content) { textEdit.text = content }
    }
}
