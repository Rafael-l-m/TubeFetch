import QtQuick
import QtQuick.Controls

Rectangle {
    property bool lightMode

    property alias p_RectangleSelected: selectedBackground
    property alias p_RectangleSelected_NumberAnimation1: selectedBackgroundNumberAnimationOnX
    property alias p_RectangleSelected_NumberAnimation2: selectedBackgroundNumberAnimationOnScale
    property alias p_RectangleHover: hoverBackground
    property alias p_RectangleHover_NumberAnimation2: hoverBackgroundNumberAnimationOnX
    property alias p_Row: textAndClickArea
    property alias p_Row_Repeater: repeater

    property list<string> options: []
    property int currentIndex: 0
    property int hoveredIndex: -1

    signal activated(int index)

    id: segmentedControl

    width: 240
    height: 30

    radius: 9

    color: segmentedControl.lightMode ? "#e8e8e8" : "#6c6c6c"

    // --------------------------------------------------
    // Selected Background
    // --------------------------------------------------

    Rectangle {
        id: selectedBackground

        x: 2 + segmentedControl.currentIndex * (segmentedControl.width - 4) / segmentedControl.options.length
        y: 2

        width: (segmentedControl.width - 4) / segmentedControl.options.length
        height: segmentedControl.height - 4

        radius: 7

        color: "#ffffff"

        border.width: 1
        border.color: "#ffffff"

        Behavior on x {
            NumberAnimation {
                id: selectedBackgroundNumberAnimationOnX
                duration: 220
                easing.type: Easing.OutCubic
            }
        }

        transformOrigin: Item.Center

        Behavior on scale {
            NumberAnimation {
                id: selectedBackgroundNumberAnimationOnScale
                duration: 100
                easing.type: Easing.OutCubic
            }
        }
    }

    // --------------------------------------------------
    // Hover Background
    // --------------------------------------------------

    Rectangle {
        id: hoverBackground

        visible: segmentedControl.hoveredIndex >= 0
                 && segmentedControl.hoveredIndex !== segmentedControl.currentIndex

        x: 2 + segmentedControl.hoveredIndex * (segmentedControl.width - 4) / segmentedControl.options.length
        y: 2

        width: (segmentedControl.width - 4) / segmentedControl.options.length
        height: segmentedControl.height - 4

        radius: 7

        color: segmentedControl.lightMode ? "#ffffff" : "#c7c7c7"
        opacity: 0.35

        Behavior on x {
            NumberAnimation {
                id: hoverBackgroundNumberAnimationOnX
                duration: 120
                easing.type: Easing.OutCubic
            }
        }
    }

    // --------------------------------------------------
    // Text && Click Area
    // --------------------------------------------------

    Row {
        id: textAndClickArea

        anchors.fill: parent
        anchors.margins: 2

        spacing: 0

        Repeater {
            id: repeater

            model: segmentedControl.options

            delegate: Item {
                required property int index
                required property string modelData

                width: (segmentedControl.width - 4) / segmentedControl.options.length
                height: segmentedControl.height - 4

                Text {
                    anchors.centerIn: parent

                    text: modelData

                    font.pixelSize: 12

                    color: segmentedControl.lightMode
                                ? (segmentedControl.currentIndex === index)
                                    ? "#222222"
                                    : "#666666"
                                : (segmentedControl.currentIndex === index)
                                    ? "#000000"
                                    : "#f0f0f0"

                    Behavior on color { ColorAnimation { duration: 220 } }
                }

                MouseArea {
                    anchors.fill: parent

                    hoverEnabled: true

                    cursorShape: Qt.PointingHandCursor

                    onEntered: { segmentedControl.hoveredIndex = index }
                    onExited: { segmentedControl.hoveredIndex = -1 }

                    onClicked: {
                        segmentedControl.currentIndex = index

                        selectedBackground.scale = 0.96

                        Qt.callLater(function() { selectedBackground.scale = 1.0 })

                        segmentedControl.activated(index)
                    }
                }
            }
        }
    }
}
