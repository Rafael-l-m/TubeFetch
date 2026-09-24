import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qml/components" as Comp
import "qml/pages/mainpage" as Mp

ApplicationWindow {
    id: window
    visible: true

    width: 900
    height: 700

    minimumWidth: 700
    minimumHeight: 400

    maximumWidth: Screen.width
    maximumHeight: Screen.height

    title: qsTr("TubeFetch (v3.0.0)")

    Comp.Background { }

    /* ColumnLayout {
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
            Layout.preferredHeight: parent.height * 0.1

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
    } */

    StackView {
        id: mainStackView

        anchors.fill: parent

        initialItem: Mp.MainPage {
            id: mainPage
        }
    }
}
