import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qml/components" as Comp

ApplicationWindow {
    id: window

    width: 900
    //height: 1100
    height: 400

    minimumWidth: 200
    minimumHeight: 250

    visible: true
    title: qsTr("Hello World")
    property bool lightMode: Application.styleHints.colorScheme === Qt.Light
    property color reallyDark: "#1f1f1f"
    property color dark: "#262626"
    property color reallyLight: "#e7e7e7"
    property color light: "#e0e0e0"


    Comp.Background { }

    /*ColumnLayout {
        anchors.centerIn: parent
        spacing: 20

        Comp.LiquidGlassButton {
            // enabled: true
            text: qsTr("Test")
            onClicked: { console.log("Button Clicked") }
        }

        Comp.LiquidGlassSwitch {
            // enabled: false
            // checked: true
            onToggled: function(checked) { console.log("Switch changed: ", checked) }
        }

        Comp.LiquidGlassTextField {
            // text: "System Generated Id"
            placeholderText: "Enter download address ..."
            // readOnly: true
            // enabled: false

            onTextEdited: function(text) { console.log("Text Changed: ", text) }

            onAccepted: { console.log("Liquid Glass TextField Accepted") }
        }

        Comp.LiquidGlassSegmentedSelection {
            id: segmentedSelection
            // enabled: false
            // currentIndex: 1

            Component.onCompleted: {
                append("Home")
                append("Explore")
                append("Library")
                append("Settings")
            }

            onClicked: function(index) { console.log("Segmented Selection -> onClicked: ", index) }

            onSelectionChanged: function (index, text) { console.log("Segmented Selection -> selectionChanged:", index, text) }
        }

        Comp.LiquidGlassComboBox {
            //readOnly: true
            width: 280

            model: [
                "English",
                "Chinese",
                "Japanese",
                "Spanish",
                "E1",
                "C1",
                "J1",
                "S1",
            ]

            currentIndex: 0

            onActivated: function(index) { console.log("Liquid Glass ComboBox: ", index, currentText) }
        }
    }*/

    ColumnLayout {
        anchors.fill: parent

        spacing: 0

        Comp.LiquidGlassButton {
            onClicked: {
                segmentedSelection.append("Test")
            }
        }

        Comp.LiquidGlassButton {
            onClicked: {
                segmentedSelection.remove(2)
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: "transparent"

            StackView {
                id: mainStackView

                anchors.fill: parent


            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.15

            color: "transparent"

            Comp.LiquidGlassSegmentedSelection {
                id: segmentedSelection

                anchors.centerIn: parent

                width: parent.width * 0.35

                Component.onCompleted: {
                    append("Main")
                }
            }
        }
    }
}
