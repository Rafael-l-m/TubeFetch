import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Rectangle {
    property bool lightMode

    property alias p_root: toast
    property alias p_Text: toastText
    property alias p_SequentialAnimation: toastAnimation
    property alias p_SequentialAnimation_ParallelAnimation1: parallelAnimation1
    property alias p_SequentialAnimation_ParallelAnimation2: parallelAnimation2
    property alias p_SequentialAnimation_ParallelAnimation1_PropertyAnimation1: propertyAnimation11
    property alias p_SequentialAnimation_ParallelAnimation1_PropertyAnimation2: propertyAnimation12
    property alias p_SequentialAnimation_ParallelAnimation2_PropertyAnimation1: propertyAnimation21
    property alias p_SequentialAnimation_ParallelAnimation2_PropertyAnimation2: propertyAnimation22

    readonly property real defaultPageWidth: 400
    readonly property real defaultPageHeight: 500

    property real toastWidth: toast.defaultPageWidth
    property real toastHeight: toast.defaultPageHeight
    property real toastWidthScale: toast.toastWidth / toast.defaultPageWidth
    property real toastHeightScale: toast.toastHeight / toast.defaultPageHeight
    property real maxWidth: parent ? parent.width * 0.8 : toast.defaultPageWidth - 100

    id: toast
    color: lightMode ? "#f9f9f9" : "#555555"
    radius: 8
    opacity: 0
    z: 999
    width: maxWidth

    anchors.horizontalCenter: parent ? parent.horizontalCenter : undefined
    anchors.bottom: parent ? parent.bottom : undefined
    anchors.bottomMargin: 20 * toast.toastHeightScale

    property real hiddenY: anchors.bottomMargin + 20 * toast.toastHeightScale

    Text {
        id: toastText

        anchors {
            left: parent.left
            right: parent.right

            leftMargin: 20
            rightMargin: 20

            verticalCenter: parent.verticalCenter
        }

        color: lightMode ? "#000000" :"#ffffff"

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
