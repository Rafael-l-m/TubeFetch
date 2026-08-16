import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    property bool lightMode

    property alias p_root: marqueeText
    property alias p_Text: textItem
    property alias p_SequentialAnimation: marqueeAnimation
    property alias p_pauseAnimation1: pauseAnimation1
    property alias p_pauseAnimation2: pauseAnimation2
    property alias p_pauseAnimation3: pauseAnimation3
    property alias p_numberAnimation1: numberAnimation1
    property alias p_numberAnimation2: numberAnimation2

    property real r_durationPerPixel: 25
    property int i_pauseBeforeStart: 400
    property int i_pauseAtEnd: 800
    property int i_minimumDuration: 1500
    property string s_text: ""
    property bool b_infiniteAnimationWithoutPause: false
    property bool b_hoverEnabled: true

    id: marqueeText
    clip: true

    Text {
        id: textItem
        text: marqueeText.s_text
        color: lightMode ? (enabled ? "#000000" : "#686868") : (enabled ? "#ffffff" : "#dbdbdb")

        wrapMode: Text.NoWrap

        width: implicitWidth
        height: marqueeText.height

        verticalAlignment: Text.AlignVCenter

        SequentialAnimation on x {
            id: marqueeAnimation

            running: { if (marqueeText.b_infiniteAnimationWithoutPause) { return textItem.implicitWidth > marqueeText.width } }

            loops: Animation.Infinite

            PauseAnimation { id: pauseAnimation1; duration: marqueeText.i_pauseBeforeStart }

            NumberAnimation {
                id: numberAnimation1

                from: 0

                to: -(textItem.implicitWidth - marqueeText.width)

                duration: Math.max(
                    marqueeText.i_minimumDuration,
                    (textItem.implicitWidth - marqueeText.width)
                    * marqueeText.r_durationPerPixel
                )

                easing.type: Easing.Linear
            }

            PauseAnimation { id: pauseAnimation2; duration: marqueeText.i_pauseAtEnd }

            NumberAnimation {
                id: numberAnimation2

                from: -(textItem.implicitWidth - marqueeText.width)

                to: 0

                duration: Math.max(
                    marqueeText.i_minimumDuration,
                    (textItem.implicitWidth - marqueeText.width)
                    * marqueeText.r_durationPerPixel
                )

                easing.type: Easing.Linear
            }

            PauseAnimation { id: pauseAnimation3; duration: marqueeText.i_pauseBeforeStart }
        }
    }
}
