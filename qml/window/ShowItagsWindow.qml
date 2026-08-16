import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

ApplicationWindow {
    property bool lightMode: Application.styleHints.colorScheme === Qt.Light

    property alias p_root: showItagsWindow
    property alias p_Rectangle: rectangle
    property alias p_Rectangle_GradientStop1: gradientStop1
    property alias p_Rectangle_GradientStop2: gradientStop2
    property alias p_Rectangle_GradientStop3: gradientStop3
    property alias p_rectangle_ScrollView: scrollView
    property alias p_rectangle_ScrollView_TextEdit: textEdit

    id: showItagsWindow

    visible: false

    width: 850
    height: 500

    title: qsTr("Show Itags")
    flags: Qt.Dialog

    Rectangle {
        id: rectangle
        anchors.fill: parent

        gradient: Gradient {
            GradientStop { id: gradientStop1; position: 0.0; color: lightMode ? "#a3d5f7" : "#000000" }
            GradientStop { id: gradientStop2; position: 0.5; color: lightMode ? "#c0e0f0" : "#181818" }
            GradientStop { id: gradientStop3; position: 1.0; color: lightMode ? "#a3d5f7" : "#000000" }
        }

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

                font.family: "Menlo"
                font.pixelSize: 12
                font.bold: true

                color: lightMode ? "#4b0082" : "#ffffff"

                selectByMouse: true

                width: Math.max(parent.availableWidth, implicitWidth)
                height: implicitHeight
            }
        }
    }

    Connections {
        target: backend
        enabled: visible

        function onItagsShown(content) { textEdit.text = content }
    }
}
