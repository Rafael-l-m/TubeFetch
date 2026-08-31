import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import "../components" as Comp


Dialog {
    property bool lightMode

    property alias p_ColumnLayoutMain: mainLayout
    property alias p_ColumnLayoutMain_RowLayout: titleLayout
    property alias p_ColumnLayoutMain_RowLayout_ColumnLayout: contentLayout
    property alias p_ColumnLayoutMain_RowLayout_ColumnLayout_HighlightTextTitle: downloadTitle
    property alias p_ColumnLayoutMain_RowLayout_ColumnLayout_HighlightTextContent: downloadContent
    property alias p_ColumnLayoutMain_ColumnLayoutProgress: progressColumnLayout
    property alias p_ColumnLayoutMain_ColumnLayout_ProgressBar: progressBar
    property alias p_ColumnLayoutMain_ColumnLayout_ProgressBar_NumberAnimation: numberAnimation
    property alias p_ColumnLayoutMain_ColumnLayout_HighlightText: statusText
    property alias p_header_Rectangle: headerTextBackground
    property alias p_header_Rectangle_Text: headerText
    property alias p_background_Rectangle: backgroundWindow
    property alias p_footer_DialogButtonBox: buttons
    property alias p_footer_DialogButtonBox_background_Rectangle: backgroundFooter
    property alias p_footer_DialogButtonBox_ButtonAccept: acceptButton
    property alias p_footer_DialogButtonBox_ButtonAccept_background_Rectangle: acceptButtonBackground
    property alias p_footer_DialogButtonBox_ButtonReject: rejectButton
    property alias p_footer_DialogButtonBox_ButtonReject_background_Rectangle: rejectButtonBackground

    property int i_toolsIdx: 0

    property alias progressValue: progressBar.value

    id: downloadToolsWindow

    parent: Overlay.overlay

    visible: false
    modal: true
    focus: true

    closePolicy: Popup.NoAutoClose

    width: 520
    height: 360

    x: (parent.width - downloadToolsWindow.width) / 2
    y: (parent.height - downloadToolsWindow.height) / 2

    ColumnLayout {
        id: mainLayout

        anchors.fill: parent
        anchors.margins: 20

        spacing: 16

        RowLayout {
            id: titleLayout

            spacing: 12

            ColumnLayout {
                id: contentLayout

                spacing: 20

                Comp.HighlightText {
                    id: downloadTitle

                    width: downloadToolsWindow.width * 0.85

                    lightMode: downloadToolsWindow.lightMode

                    p_TextEdit.font.bold: true
                    p_TextEdit.font.pixelSize: 18
                    p_TextEdit.color: downloadToolsWindow.lightMode ? "#0f0f0f" : "#f0f0f0"

                    p_TextEdit_Rectangle.color: "transparent"

                    sourceText:
                        downloadToolsWindow.i_toolsIdx === 0
                            ? qsTr("Downloading yt-dlp")
                            : (downloadToolsWindow.i_toolsIdx === 1)
                                ? qsTr("Downloading ffmpeg")
                                : (downloadToolsWindow.i_toolsIdx === 2)
                                    ? qsTr("Downloading node")
                                    : (downloadToolsWindow.i_toolsIdx === 3)
                                        ? qsTr("Downloading bgutil-ytdlp-pot-provider-rs")
                                        : qsTr("Nothing to download")
                }

                Comp.HighlightText {
                    id: downloadContent

                    width: downloadToolsWindow.width * 0.85

                    lightMode: downloadToolsWindow.lightMode

                    p_TextEdit.font.pixelSize: 14
                    p_TextEdit.color: downloadToolsWindow.lightMode ? "#0f0f0f" : "#f0f0f0"

                    p_TextEdit_Rectangle.color: "transparent"

                    sourceText:
                        downloadToolsWindow.i_toolsIdx === 0
                            ? qsTr("TubeFetch uses yt-dlp in order to download media content from the Internet.\n yt-dlp is developed by an independent team of Open Source developers and released into the public domain.\nLearn more on github.com/yt-dlp/yt-dlp\n\nClick on Continue to download yt-dlp")
                            : (downloadToolsWindow.i_toolsIdx === 1)
                                ? qsTr("TubeFetch uses FFmpeg to process and convert media files.\nFFmpeg is developed by an independent community of Open Source contributors and is released under the LGPL/GPL licenses.\nLearn more at ffmpeg.org\n\nClick on Continue to download FFmpeg")
                                : (downloadToolsWindow.i_toolsIdx === 2)
                                    ? qsTr("TubeFetch uses Node.js to run internal processing tasks.\nNode.js is maintained by the OpenJS Foundation and developed by a large community of Open Source contributors.\nLearn more at nodejs.org\n\nClick on Continue to install Node.js")
                                    : (downloadToolsWindow.i_toolsIdx === 3)
                                        ? qsTr("TubeFerch requires bgutil-pot-provider-rs as the token provider for yt-dlp to automatically fetch tokens.\nLearn more on github.com/jim60105/bgutil-ytdlp-pot-provider-rs\n\nClick on Continue to download bgutil-pot-provider-rs")
                                        : qsTr("None")

                    highlights:
                        downloadToolsWindow.i_toolsIdx === 0
                            ? [
                                {
                                    word: "github.com/yt-dlp/yt-dlp",
                                    url: "https://github.com/yt-dlp/yt-dlp",
                                    color: downloadToolsWindow.lightMode ? "#0400ff" : "#726ffe",
                                    underline: true
                                },

                                {
                                    word: qsTr("Continue"),
                                    color: "#fc694f",
                                    underline: true
                                }

                            ]
                            : (downloadToolsWindow.i_toolsIdx === 1)
                                ? [
                                    {
                                        word: "ffmpeg.org",
                                        url: "https://ffmpeg.org",
                                        color: downloadToolsWindow.lightMode ? "#0400ff" : "#726ffe",
                                        underline: true
                                    },

                                    {
                                        word: qsTr("Continue"),
                                        color: "#fc694f",
                                        underline: true
                                    }
                                ]
                                : (downloadToolsWindow.i_toolsIdx === 2)
                                    ? [
                                        {
                                            word: "nodejs.org",
                                            url: "https://nodejs.org",
                                            color: downloadToolsWindow.lightMode ? "#0400ff" : "#726ffe",
                                            underline: true
                                        },

                                        {
                                            word: qsTr("Continue"),
                                            color: "#fc694f",
                                            underline: true
                                        }
                                    ]
                                    : (downloadToolsWindow.i_toolsIdx === 3)
                                        ? [
                                            {
                                                word: "github.com/jim60105/bgutil-ytdlp-pot-provider-rs",
                                                url: "https://github.com/jim60105/bgutil-ytdlp-pot-provider-rs",
                                                color: downloadToolsWindow.lightMode ? "#0400ff" : "#726ffe",
                                                underline: true
                                            },

                                            {
                                                word: qsTr("Continue"),
                                                color: "#fc694f",
                                                underline: true
                                            }
                                        ]
                                        : []
                }
            }
        }

        ColumnLayout {
            id: progressColumnLayout

            spacing: 8

            ProgressBar {
                id: progressBar

                Layout.fillWidth: true

                value: 0.0

                Material.accent: downloadToolsWindow.lightMode ? "#0532ff" : "#fffb0d"

                Behavior on value {
                    NumberAnimation {
                        id: numberAnimation

                        duration: 800

                        easing.type: Easing.InOutQuad
                    }
                }
            }

            Comp.HighlightText {
                id: statusText

                width: downloadToolsWindow.width * 0.9

                lightMode: downloadToolsWindow.lightMode

                p_TextEdit.font.pixelSize: 14
                p_TextEdit.color: downloadToolsWindow.lightMode ? "#0f0f0f" : "#f0f0f0"

                p_TextEdit_Rectangle.color: "transparent"

                visible: progressBar.value !== 0

                sourceText: progressBar.value < 0
                                ? qsTr("Failed")
                                : (progressBar.value < 1)
                                    ? qsTr("Downloading ...")
                                    : (progressBar.value === 2)
                                        ? qsTr("Starting server ...")
                                        : qsTr("Finished")
            }
        }
    }

    header: Rectangle {
        id: headerTextBackground

        height: 40

        radius: 8

        color: downloadToolsWindow.lightMode ? "#f0f0f0" : "#1e1e1e"

        Text {
            id: headerText

            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left

            leftPadding: 20

            text: qsTr("Download Tools")

            font.pixelSize: 16
            font.bold: true

            color: downloadToolsWindow.lightMode ? "#000000" : "#ffffff"
        }
    }

    background: Rectangle {
        id: backgroundWindow

        color: downloadToolsWindow.lightMode ? "#f0f0f0" : "#1e1e1e"

        radius: 8
    }

    footer: DialogButtonBox {
        id: buttons

        background: Rectangle {
            id: backgroundFooter

            radius: 8

            color: downloadToolsWindow.lightMode ? "#f0f0f0" : "#1e1e1e"
        }

        Button {
            id: acceptButton

            text: qsTr("Continue")

            visible: (progressBar.value <= 0 || progressBar.value >= 1) && progressBar.value !== 2

            background: Rectangle {
                id: acceptButtonBackground

                radius: acceptButton.height / 2

                color: {
                    if (acceptButton.down) return "#b8b8b8"
                    if (acceptButton.hovered) return "#d1d1d1"
                    return "transparent"
                }
            }
        }

        Button {
            id: rejectButton

            text: qsTr("Exit TubeFetch")

            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole

            background: Rectangle {
                id: rejectButtonBackground

                radius: rejectButton.height / 2

                color: {
                    if (rejectButton.down) return "#b8b8b8"
                    if (rejectButton.hovered) return "#d1d1d1"
                    return "transparent"
                }
            }
        }
    }
}
