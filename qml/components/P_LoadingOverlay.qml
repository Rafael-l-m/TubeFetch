import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Rectangle {
    property bool lightMode

    readonly property real defaultPageWidth: 400
    readonly property real defaultPageHeight: 500
    readonly property real defaultIndicatorWidth: 48
    readonly property real defaultIndicatorHeight: 48

    property real overlayWidth: overlay.defaultPageWidth
    property real overlayHeight: overlay.defaultPageHeight
    property real indicatorWidthScale: overlay.overlayWidth / overlay.defaultPageWidth
    property real indicatorHeightScale: overlay.overlayHeight / overlay.defaultPageHeight

    property alias p_root: overlay
    property alias p_numberAnimation: numberAnimation
    property alias p_mouseArea: mouseArea
    property alias p_rectangle: rectangle
    property alias p_busyIndicator: indicator
    property alias running: indicator.running

    id: overlay

    anchors.fill: parent

    color: "#00000055"

    visible: false

    z: 999

    Behavior on visible { NumberAnimation { id: numberAnimation; duration: 150 } }

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        enabled: overlay.visible

        hoverEnabled: true

        acceptedButtons: Qt.AllButtons
    }

    Rectangle {
        id: rectangle

        width: overlayWidth
        height: overlayHeight

        radius: 12

        color: "#ffffff"

        anchors.centerIn: parent

        opacity: 0.9

        BusyIndicator {
            id: indicator

            anchors.centerIn: parent

            width: defaultIndicatorWidth * indicatorWidthScale
            height: defaultIndicatorHeight * indicatorHeightScale

            running: false

            Material.accent: "#007AFF"
        }
    }
}
