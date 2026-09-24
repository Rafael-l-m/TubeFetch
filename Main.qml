import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qml/components" as Comp
import "qml/pages/mainpage" as Mp

ApplicationWindow {
    id: window
    visible: true

    width: 900
    //height: 1100
    height: 400

    //minimumWidth: 200
    maximumWidth: Screen.width
    //minimumHeight: 250
    maximumHeight: Screen.height

    title: qsTr("Hello World")

    property bool lightMode: Application.styleHints.colorScheme === Qt.Light
    property color reallyDark: "#1f1f1f"
    property color dark: "#262626"
    property color reallyLight: "#e7e7e7"
    property color light: "#e0e0e0"

    Comp.Background { }

    ColumnLayout {
        anchors.fill: parent

        spacing: 20

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: "transparent"

            StackView {
                id: mainStackView

                anchors.fill: parent

                initialItem: Mp.MainPage {

                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.15

            color: "transparent"

            Comp.LiquidGlassSegmentedSelection {
                id: segmentedSelection

                anchors.centerIn: parent

                Component.onCompleted: { append("Main") }

                onClicked: function(index) { }

                onSelectionChanged: function(index, text) { }

                onItemAdded: function(id, index, text) {
                    segmentedSelection.currentIndex = index
                }

                onAddRejected: function(text) { }

                onItemRemoved: function(id, index, text) { }
            }
        }
    }
}
