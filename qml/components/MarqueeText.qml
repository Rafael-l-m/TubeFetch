import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    property bool lightMode

    property bool b_infiniteAnimationWithoutPause: false
    property bool b_hoverEnabled: true
    property bool needMarquee: marqueeText.width > 0 && textItem.implicitWidth > marqueeText.width

    property alias p_Text: textItem
    property alias p_SequentialAnimation: marqueeAnimation
    property alias p_SequentialAnimation_PauseAnimation1: pauseAnimation1
    property alias p_SequentialAnimation_PauseAnimation2: pauseAnimation2
    property alias p_SequentialAnimation_PauseAnimation3: pauseAnimation3
    property alias p_SequentialAnimation_NumberAnimation1: numberAnimation1
    property alias p_SequentialAnimation_NumberAnimation2: numberAnimation2

    property alias contentText: textItem.text

    id: marqueeText

    clip: true

    // implicitWidth: textItem.implicitWidth
    // implicitHeight: textItem.implicitHeight

    Text {
        id: textItem

        color: marqueeText.lightMode
                    ? (textItem.enabled ? "#000000" : "#686868")
                    : (textItem.enabled ? "#ffffff" : "#dbdbdb")

        wrapMode: Text.NoWrap

        width: implicitWidth
        height: marqueeText.height

        verticalAlignment: Text.AlignVCenter

        x:0

        SequentialAnimation on x {
            id: marqueeAnimation

            running: {
                if (marqueeText.b_infiniteAnimationWithoutPause) {
                    return textItem.implicitWidth > marqueeText.width
                }
            }

            loops: Animation.Infinite

            PauseAnimation { id: pauseAnimation1; duration: 600 }

            NumberAnimation {
                id: numberAnimation1

                from: 0
                to: -(textItem.implicitWidth - marqueeText.width)

                duration: Math.max(1500, (textItem.implicitWidth - marqueeText.width) * 25)

                easing.type: Easing.Linear
            }

            PauseAnimation { id: pauseAnimation2; duration: 1000 }

            NumberAnimation {
                id: numberAnimation2

                from: -(textItem.implicitWidth - marqueeText.width)
                to: 0

                duration: Math.max(1500, (textItem.implicitWidth - marqueeText.width) * 25)

                easing.type: Easing.Linear
            }

            PauseAnimation { id: pauseAnimation3; duration: 600 }
        }
    }

    onNeedMarqueeChanged: {
        if (!needMarquee) {
            marqueeAnimation.stop()
            textItem.x = 0
        }
    }
}
