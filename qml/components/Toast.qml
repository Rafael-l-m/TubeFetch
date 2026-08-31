import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Rectangle {
    property bool lightMode

    readonly property real r_defaultPageWidth: 400
    readonly property real r_defaultPageHeight: 500

    property real r_toastWidth: toast.r_defaultPageWidth
    property real r_toastHeight: toast.r_defaultPageHeight
    property real r_toastWidthScale: toast.r_toastWidth / toast.r_defaultPageWidth
    property real r_toastHeightScale: toast.r_toastHeight / toast.r_defaultPageHeight
    property real r_maxWidth: parent ? parent.width * 0.8 : toast.r_defaultPageWidth - 100

    property alias p_Text: toastText
    property alias p_SequentialAnimation: toastAnimation
    property alias p_SequentialAnimation_ParallelAnimation1: parallelAnimation1
    property alias p_SequentialAnimation_ParallelAnimation2: parallelAnimation2
    property alias p_SequentialAnimation_ParallelAnimation1_PropertyAnimation1: propertyAnimation11
    property alias p_SequentialAnimation_ParallelAnimation1_PropertyAnimation2: propertyAnimation12
    property alias p_SequentialAnimation_ParallelAnimation2_PropertyAnimation1: propertyAnimation21
    property alias p_SequentialAnimation_ParallelAnimation2_PropertyAnimation2: propertyAnimation22
    property alias p_SequentialAnimation_PauseAnimation: pauseAnimation

    id: toast

    color: toast.lightMode ? "#f9f9f9" : "#555555"

    radius: 8

    opacity: 0

    z: 999

    width: r_maxWidth

    anchors.horizontalCenter: parent ? parent.horizontalCenter : undefined
    anchors.bottom: parent ? parent.bottom : undefined
    anchors.bottomMargin: 20 * toast.r_toastHeightScale

    property real hiddenY: anchors.bottomMargin + 20 * toast.r_toastHeightScale

    Text {
        id: toastText

        anchors {
            left: parent.left
            right: parent.right

            leftMargin: 20
            rightMargin: 20

            verticalCenter: parent.verticalCenter
        }

        color: toast.lightMode ? "#000000" :"#ffffff"

        font.bold: true
        font.pixelSize: 14

        wrapMode: Text.Wrap

        horizontalAlignment: Text.AlignHCenter

        maximumLineCount: 10

        elide: Text.ElideRight
    }

    height: toastText.paintedHeight + 40

    SequentialAnimation {
        id: toastAnimation

        ParallelAnimation {
            id: parallelAnimation1
            PropertyAnimation { id: propertyAnimation11; target: toast; property: "opacity"; to: 1; duration: 400 }
            PropertyAnimation { id: propertyAnimation12; target: toast; property: "y"; to: parent.height - toast.height - anchors.bottomMargin; duration: 200; easing.type: Easing.OutCubic }
        }

        PauseAnimation { id: pauseAnimation; duration: 2500 }

        ParallelAnimation {
            id: parallelAnimation2
            PropertyAnimation { id: propertyAnimation21; target: toast; property: "opacity"; to: 0; duration: 1000 }
            PropertyAnimation { id: propertyAnimation22; target: toast; property: "y"; to: parent.height + hiddenY; duration: 300; easing.type: Easing.InCubic }
        }
    }

    function show(message) {
        toastText.text = message
        toast.y = parent.height + hiddenY
        toastAnimation.start()
    }
}
