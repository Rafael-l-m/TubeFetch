import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Rectangle {
    property bool lightMode

    readonly property real r_defaultPageWidth: 400
    readonly property real r_defaultPageHeight: 500
    readonly property real r_defaultIndicatorWidth: 48
    readonly property real r_defaultIndicatorHeight: 48

    property real r_overlayWidth: loadingOverlay.r_defaultPageWidth
    property real r_overlayHeight: loadingOverlay.r_defaultPageHeight
    property real r_indicatorWidthScale: loadingOverlay.r_overlayWidth / loadingOverlay.r_defaultPageWidth
    property real r_indicatorHeightScale: loadingOverlay.r_overlayHeight / loadingOverlay.r_defaultPageHeight

    property alias p_NumberAnimation: numberAnimation
    property alias p_MouseArea: mouseArea
    property alias p_Rectangle: loadingOverlayBackground
    property alias p_Rectangle_BusyIndicator: busyIndicator
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

        width: loadingOverlay.r_overlayWidth
        height: loadingOverlay.r_overlayHeight

        radius: 12

        color: loadingOverlay.lightMode ? "#f0f0f0" : "#ffffff"

        anchors.centerIn: parent

        opacity: 0.9

        BusyIndicator {
            id: busyIndicator

            anchors.centerIn: parent

            width: loadingOverlay.r_defaultIndicatorWidth * loadingOverlay.r_indicatorWidthScale
            height: loadingOverlay.r_defaultIndicatorHeight * loadingOverlay.r_indicatorHeightScale

            running: false
        }
    }
}
