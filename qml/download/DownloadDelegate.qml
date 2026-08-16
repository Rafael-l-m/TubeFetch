import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import "../components" as Comp

Rectangle {
    property bool lightMode
    property bool b_alwaysOnAnimation: false

    property alias p_root: downloadDelegate
    property alias p_ColumnLayout: columnLayout
    property alias p_ColumnLayout_Row: rowDelegate
    property alias p_marqueeText1: titleContainer
    property alias p_marqueeText2: progressContainer
    property alias p_marqueeText3: sizeTextContainer
    property alias p_marqueeText4: speedContainer
    property alias p_marqueeText5: remainTextContainer
    property alias p_marqueeText6: statusContainer
    property alias p_progressContainer_Background: progressContainer_Background
    property alias p_progressContainer_ProgressBar: progressContainer_ProgressBar
    property alias p_progressContainer_ProgressBar_Gradient: gradient
    property alias p_progressContainer_ProgressBar_Gradient_GradientStop1: gradientStop1
    property alias p_progressContainer_ProgressBar_Gradient_GradientStop2: gradientStop2
    property alias p_progressContainer_ProgressBar_SmoothedAnimation: smoothedAnimation
    property alias p_progressContainer_Text: progressBarText
    property alias p_MouseArea: mouseArea

    property ListView listViewRef

    id: downloadDelegate
    radius: 8
    implicitHeight: 50

    border.color: lightMode ? "#dddddd" : "#666666"

    color: lightMode
                ? (listViewRef.currentIndex === index
                        ? (enabled ? "#ccc8ff" : "#f8fafc")
                        : "#f8fafc"
                  )
                : (listViewRef.currentIndex === index)
                        ? (enabled ? "#00a6de" : "#686868")
                        : "#686868"

    ColumnLayout{
        id: columnLayout

        anchors.fill: parent
        anchors.margins: 10

        spacing: 6

        Row {
            id: rowDelegate

            Layout.fillWidth: true

            spacing: 20

            property real totalSpacing: rowDelegate.spacing * 5
            property real availableWidth: rowDelegate.width - totalSpacing

            Comp.P_MarqueeText {
                id: titleContainer

                width: rowDelegate.availableWidth * 0.25
                height: 20

                lightMode: downloadDelegate.lightMode

                s_text: title

                b_infiniteAnimationWithoutPause: b_alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_Text.font.bold: true

                p_SequentialAnimation.running: {
                    if (b_infiniteAnimationWithoutPause) { return p_Text.implicitWidth > titleContainer.width }
                    else { return b_hoverEnabled && mouseArea.containsMouse && p_Text.implicitWidth > titleContainer.width }
                }
            }

            Item {
                id: progressContainer

                width: rowDelegate.availableWidth * 0.27
                height: 20

                Rectangle {
                    id: progressContainer_Background

                    anchors.fill: parent

                    radius: height / 2

                    color: lightMode ? "#dadada" : "#080808"
                }

                Rectangle {
                    id: progressContainer_ProgressBar

                    width: parent.width * progress / 100
                    height: parent.height

                    radius: height / 2

                    gradient:
                        Gradient {
                            id: gradient
                            orientation: Gradient.Horizontal
                            GradientStop { id: gradientStop1; position: 0; color: "#4facfe" }
                            GradientStop { id: gradientStop2; position: 1; color: "#00f2fe" }
                        }

                    Behavior on width { SmoothedAnimation { id: smoothedAnimation; velocity: 800 } }
                }

                Text {
                    id: progressBarText

                    anchors.centerIn: parent

                    text: progress + "%"

                    color: lightMode ? "#000000" : "#ffffff"

                    font.pixelSize: 10
                    font.bold: true
                }
            }

            Comp.P_MarqueeText {
                id: sizeTextContainer

                width: rowDelegate.availableWidth * 0.14
                height: 20

                lightMode: downloadDelegate.lightMode

                s_text: downloadedBytes + "/" + totalBytes + " MiB"

                b_infiniteAnimationWithoutPause: b_alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_numberAnimation1.duration: titleContainer.p_numberAnimation1.duration
                p_numberAnimation2.duration: titleContainer.p_numberAnimation2.duration

                p_SequentialAnimation.running: {
                    if (b_infiniteAnimationWithoutPause) { return p_Text.implicitWidth > sizeTextContainer.width }
                    else { return b_hoverEnabled && mouseArea.containsMouse && p_Text.implicitWidth > sizeTextContainer.width }
                }
            }

            Comp.P_MarqueeText {
                id: speedContainer

                width: rowDelegate.availableWidth * 0.1
                height: 20

                lightMode: downloadDelegate.lightMode

                s_text: speed + " KiB"

                b_infiniteAnimationWithoutPause: b_alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_numberAnimation1.duration: titleContainer.p_numberAnimation1.duration
                p_numberAnimation2.duration: titleContainer.p_numberAnimation2.duration

                p_SequentialAnimation.running: {
                    if (b_infiniteAnimationWithoutPause) { return p_Text.implicitWidth > speedContainer.width }
                    else { return b_hoverEnabled && mouseArea.containsMouse && p_Text.implicitWidth > speedContainer.width }
                }
            }

            Comp.P_MarqueeText {
                id: remainTextContainer

                width: rowDelegate.availableWidth * 0.1
                height: 20

                lightMode: downloadDelegate.lightMode

                s_text: remain

                b_infiniteAnimationWithoutPause: b_alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_numberAnimation1.duration: titleContainer.p_numberAnimation1.duration
                p_numberAnimation2.duration: titleContainer.p_numberAnimation2.duration

                p_SequentialAnimation.running: {
                    if (b_infiniteAnimationWithoutPause) { return p_Text.implicitWidth > speedContainer.width }
                    else { return b_hoverEnabled && mouseArea.containsMouse && p_Text.implicitWidth > speedContainer.width }
                }
            }

            Comp.P_MarqueeText {
                id: statusContainer

                width: rowDelegate.availableWidth * 0.1
                height: 20

                s_text: status

                lightMode: downloadDelegate.lightMode

                b_infiniteAnimationWithoutPause: b_alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_numberAnimation1.duration: titleContainer.p_numberAnimation1.duration

                p_numberAnimation2.duration: titleContainer.p_numberAnimation2.duration

                p_SequentialAnimation.running: {
                    if (b_infiniteAnimationWithoutPause) { return p_Text.implicitWidth > speedContainer.width }
                    else { return b_hoverEnabled && mouseArea.containsMouse && p_Text.implicitWidth > speedContainer.width }
                }
            }
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        hoverEnabled: true

        onClicked:      { downloadDelegate.listViewRef.currentIndex = index }
        onPressAndHold: { downloadDelegate.listViewRef.currentIndex = index }
        onPressed:      { downloadDelegate.listViewRef.currentIndex = index }
    }
}
