import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs

import "../components" as Comp
import "../pages/preferences" as Pref


ApplicationWindow {
    id: preferencesWindow

    visible: true

    width: 870
    height: 400

    minimumWidth: 870
    minimumHeight: 400

    maximumWidth: Screen.width
    maximumHeight: Screen.height

    onClosing: (event) => { windowManager.backToMainWindow() }

    Component.onCompleted: { backend.loadSettings() }

    title: qsTr("Preferences")

    property bool lightMode: Application.styleHints.colorScheme === Qt.Light
    property int i_currentIndex: 0

    RowLayout {
        anchors.fill: parent

        spacing: 0

        Rectangle {
            Layout.preferredWidth: parent.width * 0.19
            Layout.fillHeight: true

            color: preferencesWindow.lightMode ? "#eeeeee" : "#1b1919"

            Column {
                anchors.centerIn: parent

                width: parent.width

                spacing: 10

                Repeater {
                    model: [
                        { idd: 0, name: qsTr("General Settings"),  page: "../pages/preferences/GeneralSettings.qml"  },
                        { idd: 1, name: qsTr("Page Settings"),     page: "../pages/preferences/PageSettings.qml"     },
                        { idd: 2, name: qsTr("Download Settings"), page: "../pages/preferences/DownloadSettings.qml" }
                    ]

                    delegate: Rectangle {
                        id: contentBackground

                        width: parent.width
                        height: 50

                        property bool selected: index === preferencesWindow.i_currentIndex
                        property bool hovered: false

                        radius: 6

                        color: {
                            if (selected) { return preferencesWindow.lightMode ? "#dbeafe" : "#2a3b55" }
                            if (hovered)  { return preferencesWindow.lightMode ? "#e5e5e5" : "#303030" }
                            return "transparent"
                        }

                        Text {
                            id: contentText

                            anchors.centerIn: parent

                            text: modelData.name

                            color: contentBackground.selected
                                        ? "#2196F3"
                                        : (preferencesWindow.lightMode ? "#333333" : "#ffffff")

                            font.bold: selected
                            font.pixelSize: 13

                            Behavior on color { ColorAnimation { id: colorAnimation; duration: 150 } }
                        }

                        MouseArea {
                            id: mouseArea

                            anchors.fill: parent
                            hoverEnabled: true

                            onEntered: parent.hovered = true
                            onExited: parent.hovered = false

                            onClicked: {
                                preferencesWindow.i_currentIndex = modelData.idd
                                preferencesStack.replace(modelData.page, {lightMode: preferencesWindow.lightMode})
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: preferencesWindow.lightMode ? "#f5f5f5" : "#202020"

            StackView {
                id: preferencesStack

                anchors.fill: parent

                initialItem: Pref.GeneralSettings { lightMode: preferencesWindow.lightMode }
            }
        }
    }

    background: Rectangle { color: lightMode ? "#ffffff" : "#000000" }

    Connections {
        target: backend
        enabled: preferencesWindow.visible

        function onSettingsLoaded(obj) {
            const lma = obj.lightModeAutomatic
            const lm = obj.lightMode

            if (!lma) { preferencesWindow.lightMode = lm }

            else { preferencesWindow.lightMode = Qt.binding(function() {
                return Application.styleHints.colorScheme === Qt.Light
            })}
        }
    }
}
