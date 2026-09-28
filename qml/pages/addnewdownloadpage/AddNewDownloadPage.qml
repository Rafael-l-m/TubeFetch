import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts
import "../../components" as Comp

Rectangle {
    property bool editMode: false

    property string audioItag: ""
    property string videoItag: ""
    property string nonDashItag: ""

    id: background

    color: "transparent"

    // ESC -> Rollback
    focus: true
    Keys.onEscapePressed: { mainStackView.pop() }

    // Simulate: Rollback
    MultiPointTouchArea {
        anchors.fill: parent
        minimumTouchPoints: 1
        maximumTouchPoints: 1
        onReleased: (points) => { if (points[0].x - points[0].startX > 120) { mainStackView.pop() } }
    }

    ColumnLayout {
        id: columnLayout

        anchors.fill: parent
        anchors.margins: 50

        spacing: 25

        GroupBox {
            Layout.fillWidth: true

            title: qsTr("URL")

            GridLayout {
                anchors.centerIn: parent
                anchors.margins: 20

                columns: 3
                columnSpacing: 16
                rowSpacing: 20

                Label {
                    text: qsTr("URL:")

                    font.pixelSize: 14
                    font.bold: true

                    opacity: 0.7

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                Comp.LiquidGlassTextField {
                    id: textFieldURL

                    implicitWidth: 500

                    font.pixelSize: 14

                    placeholderText: qsTr("Enter download address ...")

                    onTextChanged: {
                        const newContent = textFieldURL.text.trim()

                        if (newContent.length === 0) {
                            textFieldURL.text = ""
                            textFieldTitle.text = ""

                            videoCodecComboBox.model = [qsTr("None")]
                            audioCodecComboBox.model = [qsTr("None")]
                            nonDashCodecComboBox.model = [qsTr("None")]
                            metadataCodecComboBox.currentIndex = 0
                            subtitlesCodecComboBox.currentIndex = 0
                        }
                    }

                    extraTimer.onTriggered: {
                        if (background.editMode) { return }

                        const url = textFieldURL.text.trim()

                        if (url.length === 0) { return }

                        backend.cutUrl(url);
                    }
                }

                Comp.LiquidGlassButton {
                    id: searchInfoBtn

                    text: remainingSeconds > 0 ? "0" + Math.floor(remainingSeconds / 60) + ":" +
                            (remainingSeconds % 60).toString().padStart(2, "0") : qsTr("Search Info")

                    enabled: remainingSeconds === 0 && textFieldURL.text.trim().length > 0

                    implicitWidth: 160

                    onClicked: {
                        backend.isValidUrl(textFieldURL.text.trim())
                        remainingSeconds = 120
                        countdownTimer.start()
                    }
                }

                Label {
                    text: qsTr("Title:")

                    font.pixelSize: 14
                    font.bold: true

                    opacity: 0.7

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                Comp.LiquidGlassTextField {
                    id: textFieldTitle

                    implicitWidth: 500

                    font.pixelSize: 14

                    readOnly: true
                }
            }

            label: Label {
                text: parent.title

                anchors.left: parent.left
                anchors.leftMargin: 12

                font.pixelSize: 12
                font.bold: true

                opacity: 0.3

                color: "#ffffff"
            }

            background: Rectangle {
                border.color: "#dedede"
                border.width: 1

                opacity: 0.3

                radius: 8

                color: "transparent"
            }
        }

        GroupBox {
            Layout.fillWidth: true

            title: qsTr("Config")

            GridLayout {
                anchors.centerIn: parent
                anchors.margins: 20

                columns: 2
                columnSpacing: 16
                rowSpacing: 20

                Label {
                    text: qsTr("Mode:")

                    font.pixelSize: 14
                    font.bold: true

                    opacity: 0.7

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                Row {
                    spacing: 25

                    Comp.LiquidGlassComboBox {
                        id: modeComboBox

                        fontsize: 14

                        popupMaxHeight: 150

                        model: [qsTr("Best Video"), qsTr("Best Audio"), qsTr("Personalized")]

                        onActivated: {
                            console.log("Model changed, index: ", currentIndex, currentText)
                        }

                        onPopupOpened: {
                            if (modeComboBox.currentIndex !== 2) {
                                metadataCodecComboBox.visible = false
                                subtitlesCodecComboBox.visible = false
                                hideMode4.visible = true
                                hideMode5.visible = true
                            }

                            else {
                                videoCodecComboBox.visible = false
                                audioCodecComboBox.visible = false
                                hideMode1.visible = true
                                hideMode2.visible = true
                            }
                        }

                        onPopupClosed: {
                            if (modeComboBox.currentIndex !== 2) {
                                metadataCodecComboBox.visible = true
                                subtitlesCodecComboBox.visible = true
                                hideMode4.visible = false
                                hideMode5.visible = false
                            }

                            else {
                                videoCodecComboBox.visible = true
                                audioCodecComboBox.visible = true
                                hideMode1.visible = false
                                hideMode2.visible = false
                            }
                        }
                    }

                    Comp.LiquidGlassButton {
                        id: showCodecBtn

                        text: qsTr("Show Codecs")

                        implicitHeight: 52

                        enabled: textFieldTitle.text.trim().length > 0

                        visible: modeComboBox.currentIndex === 2
                    }
                }

                Label {
                    text: qsTr("Video Codec:")

                    font.pixelSize: 14
                    font.bold: true

                    opacity: 0.7

                    visible: modeComboBox.currentIndex === 2

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                Comp.LiquidGlassComboBox { id: hideMode1; opacity: 0.1; readOnly: true; visible: false }

                Comp.LiquidGlassComboBox {
                    id: videoCodecComboBox

                    fontsize: 14

                    popupMaxHeight: 150

                    visible: modeComboBox.currentIndex === 2

                    model: [qsTr("None")]
                    // change gridLayout to rowlayout is possible
                    // when access -> autoclean
                    onActivated: {
                        console.log("Model changed, index: ", currentIndex, currentText)

                        metadataCodecComboBox.visible = true
                        subtitlesCodecComboBox.visible = true
                        hideMode1.visible = false
                        hideMode2.visible = false
                        hideMode3.visible = false
                        hideMode4.visible = false
                        hideMode5.visible = false
                        hideMode6.visible = false
                    }

                    onPopupOpened: {
                        audioCodecComboBox.visible = false
                        nonDashCodecComboBox.visible = false
                        hideMode2.visible = true
                        hideMode3.visible = true
                    }

                    onPopupClosed: {
                        audioCodecComboBox.visible = true
                        nonDashCodecComboBox.visible = true
                        hideMode2.visible = false
                        hideMode3.visible = false
                    }
                }

                Label {
                    text: qsTr("Audio Codec:")

                    font.pixelSize: 14
                    font.bold: true

                    opacity: 0.7

                    visible: modeComboBox.currentIndex === 2

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                Comp.LiquidGlassComboBox { id: hideMode2; opacity: 0.1; readOnly: false; visible: false }

                Comp.LiquidGlassComboBox {
                    id: audioCodecComboBox

                    fontsize: 14

                    popupMaxHeight: 150

                    visible: modeComboBox.currentIndex === 2

                    model: [qsTr("None")]

                    onActivated: {
                        console.log("Model changed, index: ", currentIndex, currentText)
                    }

                    onPopupOpened: {
                        nonDashCodecComboBox.visible = false
                        metadataCodecComboBox.visible = false
                        hideMode3.visible = true
                        hideMode4.visible = true
                    }

                    onPopupClosed: {
                        nonDashCodecComboBox.visible = true
                        metadataCodecComboBox.visible = true
                        hideMode3.visible = false
                        hideMode4.visible = false
                    }
                }

                Label {
                    text: qsTr("Non-Dash Codec:")

                    font.pixelSize: 14
                    font.bold: true

                    opacity: 0.7

                    visible: modeComboBox.currentIndex === 2

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                Comp.LiquidGlassComboBox { id: hideMode3; opacity: 0.1; readOnly: true; visible: false }

                Comp.LiquidGlassComboBox {
                    id: nonDashCodecComboBox

                    fontsize: 14

                    popupMaxHeight: 150

                    visible: modeComboBox.currentIndex === 2

                    model: [qsTr("None")]

                    onActivated: {
                        console.log("Model changed, index: ", currentIndex, currentText)
                    }

                    onPopupOpened: {
                        metadataCodecComboBox.visible = false
                        subtitlesCodecComboBox.visible = false
                        hideMode4.visible = true
                        hideMode5.visible = true
                    }

                    onPopupClosed: {
                        metadataCodecComboBox.visible = true
                        subtitlesCodecComboBox.visible = true
                        hideMode4.visible = false
                        hideMode5.visible = false
                    }
                }

                Label {
                    text: qsTr("Metadata:")

                    font.pixelSize: 14
                    font.bold: true

                    opacity: 0.7

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                Comp.LiquidGlassComboBox { id: hideMode4; opacity: 0.1; readOnly: true; visible: false }

                Comp.LiquidGlassComboBox {
                    id: metadataCodecComboBox

                    fontsize: 14

                    model: [qsTr("Without Metadata"), qsTr("With Metadata")]

                    onActivated: {
                        console.log("Model changed, index: ", currentIndex, currentText)
                    }

                    onPopupOpened: {
                        subtitlesCodecComboBox.visible = false
                        hideMode5.visible = true
                    }

                    onPopupClosed: {
                        subtitlesCodecComboBox.visible = true
                        hideMode5.visible = false
                    }
                }

                Label {
                    text: qsTr("Subtitles:")

                    font.pixelSize: 14
                    font.bold: true

                    opacity: 0.7

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                Comp.LiquidGlassComboBox { id: hideMode5; opacity: 0.1; readOnly: true; visible: false }

                Comp.LiquidGlassComboBox {
                    id: subtitlesCodecComboBox

                    fontsize: 14

                    model: [qsTr("Without Subtitles"), qsTr("With Subtitles")]

                    onActivated: {
                        console.log("Model changed, index: ", currentIndex, currentText)
                    }

                    onPopupOpened: {
                        if (modeComboBox.currentIndex === 2) {
                            saveAsField.visible = false
                            hideMode6.visible = true
                        }
                    }

                    onPopupClosed: {
                        if (modeComboBox.currentIndex === 2) {
                            saveAsField.visible = true
                            hideMode6.visible = false
                        }
                    }
                }
            }

            label: Label {
                text: parent.title

                anchors.left: parent.left
                anchors.leftMargin: 12

                font.pixelSize: 12
                font.bold: true

                opacity: 0.3

                color: "#ffffff"
            }

            background: Rectangle {
                border.color: "#dedede"
                border.width: 1

                opacity: 0.3

                radius: 8

                color: "transparent"
            }
        }

        GroupBox {
            Layout.fillWidth: true

            title: qsTr("Output")

            GridLayout {
                anchors.centerIn: parent
                anchors.margins: 20

                columns: 3
                columnSpacing: 16
                rowSpacing: 20

                Label {
                    text: qsTr("Output:")

                    font.pixelSize: 12
                    font.bold: true

                    opacity: 0.7

                    color: "#ffffff"

                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }

                Comp.LiquidGlassTextField {
                    id: saveAsField

                    implicitWidth: 500

                    font.pixelSize: 12
                }

                Comp.LiquidGlassTextField { id: hideMode6; opacity: 0.1; implicitWidth: 500; font.pixelSize: 12; visible: false }

                Comp.LiquidGlassButton {
                    id: saveAsBtn

                    text: qsTr("Save As")

                    implicitWidth: 160
                    implicitHeight: 50
                }
            }

            label: Label {
                text: parent.title

                anchors.left: parent.left
                anchors.leftMargin: 12

                font.pixelSize: 12
                font.bold: true

                opacity: 0.3

                color: "#ffffff"
            }

            background: Rectangle {
                border.color: "#dedede"
                border.width: 1

                opacity: 0.3

                radius: 8

                color: "transparent"
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton {
                id: backBtn

                text: qsTr("Back")

                implicitWidth: 260

                onClicked: { mainStackView.pop() }
            }

            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton {
                id: okBtn

                text: qsTr("Continue")

                implicitWidth: 260
            }

            Item { Layout.fillWidth: true }
        }
    }

    property int remainingSeconds: 0

    Timer {
        id: countdownTimer

        interval: 1000

        repeat: true

        onTriggered: {
            if (remainingSeconds > 0) { --remainingSeconds }

            if (remainingSeconds === 0) { countdownTimer.stop() }
        }
    }

    FileDialog {
        id: saveAs

        fileMode: FileDialog.SaveFile

        title: qsTr("Save As")

        onAccepted: {
            loading.running = true
            loading.visible = true

            backend.checkAvailablePath(saveAs.selectedFile)
        }
    }

    Comp.MessageDialog {
        id: showBeforeChooseSavePath

        width: 450
        height: 220

        messageText: qsTr("After selecting the file save path, the previous options can no longer be modified\n\nThey can still be adjusted later\n\n**This prompt can be turned off in the settings\n\nAre you sure you want to proceed?")

        onAccepted: {
            // beforeSaving()
            saveAs.open()
        }
    }

    Comp.Toast { id: toast }

    Comp.LoadingOverlay { id: loading; r_overlayWidth: background.width; r_overlayHeight: background.height }

    Connections {
        target: backend
        enabled: background.visible

        function onUrlCut(url) { textFieldURL.text = url.trim() }

        function onUrlValid(url, ok) {
            if (!ok) {
                textFieldTitle.text = qsTr("Failed")
                toast.show(qsTr("URL Invalid: ") + url)
                return
            }

            loading.running = true
            loading.visible = true

            backend.getUrlInfo(url)
        }

        function onJsonReady(info) {
            loading.running = false
            loading.visible = false

            const non_selected = qsTr("Non Selected")
            const audio_arr = []
            const video_arr = []
            const non_dash_arr = info.non_dash_formats

            for (let aud of info.mpeg_dash_audio_formats) { audio_arr.push(aud) }
            for (let vid of info.mpeg_dash_video_formats) { video_arr.push(vid) }

            audio_arr.unshift(non_selected)
            video_arr.unshift(non_selected)
            non_dash_arr.unshift(non_selected)

            textFieldTitle.text = info.title
            audioCodecComboBox.model = audio_arr
            videoCodecComboBox.model = video_arr
            nonDashCodecComboBox.model = non_dash_arr

            if (background.editMode) {
                if (background.audioItag.trim() !== "")   {
                    audioCodecComboBox.currentIndex =
                        audioCodecComboBox.find(background.audioItag.trim())
                }

                if (background.videoItag.trim() !== "")   {
                    videoCodecComboBox.currentIndex =
                        videoCodecComboBox.find(background.videoItag.trim())
                }

                if (background.nonDashItag.trim() !== "") {
                    nonDashCodecComboBox.currentIndex =
                        nonDashCodecComboBox.find(background.nonDashItag.trim())
                }
            }
        }

        function onJsonError(message) {
            loading.running = false
            loading.visible = false

            textFieldTitle.text = qsTr("Failed")

            toast.show(message)
        }

        function onFiltersAsked(mediaFilters) { saveAs.nameFilters = mediaFilters }

        function onAvailablePathChecked(ok, path) {
            loading.visible = false
            loading.running = false

            if (ok) { r3.p_GridLayout_InputField.text = path }

            else { toast.show(qsTr("Invalid Save Path: is not writable")) }
        }

        function onNewDownloadAdded(accepted, message) {
            loading.visible = false
            loading.running = false

            if (accepted) { Qt.callLater(function() { windowManager.backToMainWindow() }) }

            else { toast.show(message) }
        }

        function onEditInformationRequest(obj, message) {
            if (addNewDownloadPage.b_editMode && addNewDownloadPage.qint64_internalId !== 0 && obj.internalId === qint64_internalId) {
                r1.p_GridLayout_InputFieldURL.text = obj.url.trim()
                r1.p_GridLayout_InputFieldURL.readOnly = true
                r1.p_GridLayout_InputFieldTitle.text = obj.title.trim()

                if (obj.bestAudio) { r2.p_GridLayout_Row_ChooseComboBox.currentIndex = 1 }

                else if (obj.bestVideo) { r2.p_GridLayout_Row_ChooseComboBox.currentIndex = 0 }

                else {
                    r2.p_GridLayout_Row_ChooseComboBox.currentIndex = 2
                    r2.p_GridLayout_Row_PushButton.enabled = true
                    r2.p_GridLayout_Row_PushButton.visible = true
                    r2.p_GridLayout_LabelAudioCodec.visible = true
                    r2.p_GridLayout_ChooseComboBoxAudioCodec.visible = true
                    r2.p_GridLayout_LabelVideoCodec.visible = true
                    r2.p_GridLayout_ChooseComboBoxVideoCodec.visible = true
                    r2.p_GridLayout_LabelNonDashCodec.visible = true
                    r2.p_GridLayout_ChooseComboBoxNonDash.visible = true

                    s_audioItag = obj.audioCode.trim()
                    s_videoItag = obj.videoCode.trim()
                    s_nonDashItag = obj.nonDashCode.trim()
                }

                addNewDownloadPage.s_pendingUrl = obj.url.trim()
                delayTimer.restart()

                r2.p_GridLayout_ChooseComboBoxMetadata.currentIndex = (obj.metadata ? 1 : 0)
                r2.p_GridLayout_ChooseComboBoxSubtitles.currentIndex = 0
                r3.p_GridLayout_InputField.text = obj.savePath

                addNewDownloadPage.b_showInfoBeforeSaving = false

                loading.running = true
                loading.visible = true
            }

            else { toast.show(message) }
        }

        function onPossiblePerform(ok) {
            if (addNewDownloadPage.b_enableRateLimit) { addNewDownloadPage.b_canPerform = ok; }

            if (!ok && addNewDownloadPage.b_enableRateLimit) { toast.show(qsTr("You have reached the limit. Please wait at least one hour before trying again.")) }
        }
    }
}
