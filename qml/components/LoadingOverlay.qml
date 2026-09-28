import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Rectangle {
    readonly property real defaultPageWidth: 400
    readonly property real defaultPageHeight: 500
    readonly property real defaultIndicatorWidth: 48
    readonly property real defaultIndicatorHeight: 48

    property real overlayWidth: loadingOverlay.defaultPageWidth
    property real overlayHeight: loadingOverlay.defaultPageHeight
    property real indicatorWidthScale: loadingOverlay.overlayWidth / loadingOverlay.defaultPageWidth
    property real indicatorHeightScale: loadingOverlay.overlayHeight / loadingOverlay.defaultPageHeight

    property alias running: busyIndicator.running

    Material.accent: "#007aff"

    id: loadingOverlay

    anchors.fill: parent

    color: "#00000055"

    visible: false

    z: 999

    Behavior on visible { NumberAnimation { id: numberAnimation; duration: 150 } }

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        enabled: loadingOverlay.visible

        hoverEnabled: true

        acceptedButtons: Qt.AllButtons
    }

    Rectangle {
        id: loadingOverlayBackground

        width: loadingOverlay.overlayWidth
        height: loadingOverlay.overlayHeight

        radius: 12

        color: "#ffffff"

        anchors.centerIn: parent

        opacity: 0.9

        BusyIndicator {
            id: busyIndicator

            anchors.centerIn: parent

            width: loadingOverlay.defaultIndicatorWidth * loadingOverlay.indicatorWidthScale
            height: loadingOverlay.defaultIndicatorHeight * loadingOverlay.indicatorHeightScale

            running: false
        }
    }
}
