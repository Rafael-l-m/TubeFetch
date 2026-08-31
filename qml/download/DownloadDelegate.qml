import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import "../components" as Comp


Rectangle {
    property bool b_lightMode

    property alias p_ColumnLayout: columnLayout
    property alias p_ColumnLayout_Row: rowDelegate
    property alias p_ColumnLayout_Row_MarqueeText_Title: titleContainer
    property alias p_ColumnLayout_Row_Item: progressContainer
    property alias p_ColumnLayout_Row_MarqueeText_SizeText: sizeTextContainer
    property alias p_ColumnLayout_Row_MarqueeText_SpeedText: speedContainer
    property alias p_ColumnLayout_Row_MarqueeText_RemainText: remainTextContainer
    property alias p_ColumnLayout_Row_MarqueeText_StatusText: statusContainer
    property alias p_ColumnLayout_Row_Item_Rectangle_Background: progressContainer_Background
    property alias p_ColumnLayout_Row_Item_Rectangle_Progress: progressContainer_ProgressBar
    property alias p_ColumnLayout_Row_Item_Rectangle_Progress_gradient_Gradient: gradient
    property alias p_ColumnLayout_Row_Item_Rectangle_Progress_gradient_GradientStop0: gradientStop1
    property alias p_ColumnLayout_Row_Item_Rectangle_Progress_gradient_GradientStop1: gradientStop2
    property alias p_ColumnLayout_Row_Item_Rectangle_Progress_SmoothedAnimation: smoothedAnimation
    property alias p_ColumnLayout_Row_Item_Text: progressBarText
    property alias p_MouseArea: mouseArea

    property bool b_alwaysOnAnimation: false

    property ListView listViewRef

    id: downloadDelegate

    radius: 8

    implicitHeight: 50

    border.color: downloadDelegate.b_lightMode ? "#dddddd" : "#666666"

    color: downloadDelegate.b_lightMode
                ? (listViewRef.currentIndex === index
                        ? (downloadDelegate.enabled ? "#ccc8ff" : "#f8fafc")
                        : "#f8fafc"
                  )
                : (listViewRef.currentIndex === index)
                        ? (downloadDelegate.enabled ? "#00a6de" : "#686868")
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

            Comp.MarqueeText {
                id: titleContainer

                width: rowDelegate.availableWidth * 0.25
                height: 20

                contentText: title

                lightMode: downloadDelegate.b_lightMode
                b_infiniteAnimationWithoutPause: downloadDelegate.b_alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_Text.font.bold: true

                p_SequentialAnimation.running: {
                    if (downloadDelegate.b_infiniteAnimationWithoutPause) {
                        return p_Text.implicitWidth > titleContainer.width
                    }

                    else {
                        return mouseArea.containsMouse && p_Text.implicitWidth > titleContainer.width
                    }
                }
            }

            Item {
                id: progressContainer

                width: rowDelegate.availableWidth * 0.27
                height: 20

                Rectangle {
                    id: progressContainer_Background

                    anchors.fill: parent

                    radius: progressContainer.height / 2

                    color: downloadDelegate.b_lightMode ? "#dadada" : "#080808"
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

                    color: downloadDelegate.b_lightMode ? "#000000" : "#ffffff"

                    font.pixelSize: 10
                    font.bold: true
                }
            }

            Comp.MarqueeText {
                id: sizeTextContainer

                width: rowDelegate.availableWidth * 0.14
                height: 20

                contentText: downloadedBytes + "/" + totalBytes + " MiB"

                lightMode: downloadDelegate.b_lightMode
                b_infiniteAnimationWithoutPause: downloadDelegate.b_alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_SequentialAnimation_NumberAnimation1.duration: titleContainer.p_SequentialAnimation_NumberAnimation1.duration
                p_SequentialAnimation_NumberAnimation2.duration: titleContainer.p_SequentialAnimation_NumberAnimation2.duration

                p_SequentialAnimation.running: {
                    if (downloadDelegate.b_infiniteAnimationWithoutPause) {
                        return p_Text.implicitWidth > sizeTextContainer.width
                    }

                    else {
                        return mouseArea.containsMouse && p_Text.implicitWidth > sizeTextContainer.width
                    }
                }
            }

            Comp.MarqueeText {
                id: speedContainer

                width: rowDelegate.availableWidth * 0.1
                height: 20

                contentText: speed + " KiB"

                lightMode: downloadDelegate.b_lightMode
                b_infiniteAnimationWithoutPause: downloadDelegate.b_alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_SequentialAnimation_NumberAnimation1.duration: titleContainer.p_SequentialAnimation_NumberAnimation1.duration
                p_SequentialAnimation_NumberAnimation2.duration: titleContainer.p_SequentialAnimation_NumberAnimation2.duration

                p_SequentialAnimation.running: {
                    if (downloadDelegate.b_infiniteAnimationWithoutPause) {
                        return p_Text.implicitWidth > speedContainer.width
                    }

                    else {
                        return mouseArea.containsMouse && p_Text.implicitWidth > speedContainer.width
                    }
                }
            }

            Comp.MarqueeText {
                id: remainTextContainer

                width: rowDelegate.availableWidth * 0.1
                height: 20

                contentText: remain

                lightMode: downloadDelegate.b_lightMode
                b_infiniteAnimationWithoutPause: downloadDelegate.b_alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_SequentialAnimation_NumberAnimation1.duration: titleContainer.p_SequentialAnimation_NumberAnimation1.duration
                p_SequentialAnimation_NumberAnimation2.duration: titleContainer.p_SequentialAnimation_NumberAnimation2.duration

                p_SequentialAnimation.running: {
                    if (downloadDelegate.b_infiniteAnimationWithoutPause) {
                        return p_Text.implicitWidth > speedContainer.width
                    }

                    else {
                        return mouseArea.containsMouse && p_Text.implicitWidth > speedContainer.width
                    }
                }
            }

            Comp.MarqueeText {
                id: statusContainer

                width: rowDelegate.availableWidth * 0.1
                height: 20

                contentText: status

                lightMode: downloadDelegate.b_lightMode
                b_infiniteAnimationWithoutPause: downloadDelegate.b_alwaysOnAnimation

                enabled: downloadDelegate.enabled

                p_SequentialAnimation_NumberAnimation1.duration: titleContainer.p_SequentialAnimation_NumberAnimation1.duration
                p_SequentialAnimation_NumberAnimation2.duration: titleContainer.p_SequentialAnimation_NumberAnimation2.duration

                p_SequentialAnimation.running: {
                    if (downloadDelegate.b_infiniteAnimationWithoutPause) {
                        return p_Text.implicitWidth > speedContainer.width
                    }

                    else {
                        return mouseArea.containsMouse && p_Text.implicitWidth > speedContainer.width
                    }
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
