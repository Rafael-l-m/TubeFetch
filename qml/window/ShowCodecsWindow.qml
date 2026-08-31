import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

ApplicationWindow {
    property bool lightMode: Application.styleHints.colorScheme === Qt.Light

    id: showCodecsWindow

    visible: true

    width: 1000
    height: 700

    maximumWidth: Screen.width
    maximumHeight: Screen.height

    title: qsTr("Show Codecs")
    flags: Qt.Dialog

    Component.onCompleted: { backend.loadSettings() }

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

        function onCodecsShown(content) { textEdit.text = content }

        function onSettingsLoaded(obj) {
            const lma = obj.lightModeAutomatic
            const lm = obj.lightMode

            if (!lma) { toolsWindow.lightMode = lm }

            else { toolsWindow.lightMode = Qt.binding(function() {
                return Application.styleHints.colorScheme === Qt.Light
            })}
        }
    }
}
