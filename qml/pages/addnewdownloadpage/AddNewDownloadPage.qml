import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts
import "../../components" as Comp

Rectangle {
    property bool editMode: false

    property var internalIdd
    property string audioItag: ""
    property string videoItag: ""
    property string nonDashItag: ""

    id: addNewDownloadPage

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

    Component.onCompleted: {
        backend.cleanDownloadUrlInfo()

        if (addNewDownloadPage.editMode && addNewDownloadPage.internalIdd !== 0) { backend.requestEditInformation(addNewDownloadPage.internalIdd) }
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
                        if (addNewDownloadPage.editMode) { return }

                        if (textFieldURL.text.trim().length === 0) {
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
                        if (addNewDownloadPage.editMode) { return }

                        const url = textFieldURL.text.trim()

                        if (url.length === 0) { return }

                        backend.cutUrl(url);
                    }
                }

                Comp.LiquidGlassButton {
                    id: searchInfoBtn

                    text: remainingSeconds1 > 0 ? "0" + Math.floor(remainingSeconds1 / 60) + ":" +
                            (remainingSeconds1 % 60).toString().padStart(2, "0") : qsTr("Search Info")

                    enabled: remainingSeconds1 === 0 && textFieldURL.text.trim().length > 0

                    implicitWidth: 160

                    onClicked: {
                        remainingSeconds1 = 60 * 2
                        countdownTimer1.start()
                        backend.isValidUrl(textFieldURL.text.trim())
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

                Comp.LiquidGlassButton {
                    id: showCodecBtn

                    text: remainingSeconds2 > 0 ? "0" + Math.floor(remainingSeconds2 / 60) + ":" +
                            (remainingSeconds2 % 60).toString().padStart(2, "0") : qsTr("Show Codecs")

                    implicitWidth: 160

                    enabled: remainingSeconds2 === 0 && textFieldTitle.text.trim().length > 0

                    onClicked: {
                        remainingSeconds2 = 5 * 60
                        countdownTimer2.start()
                        backend.showCodecs(textFieldURL.text.trim())
                        windowManager.switchToIndependentWindow("ShowCodecsWindow.qml")
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

                Comp.LiquidGlassComboBox {
                    id: modeComboBox

                    fontsize: 14

                    popupMaxHeight: 150

                    model: [qsTr("Best Video"), qsTr("Best Audio"), qsTr("Personalized")]

                    onActivated: {
                        if (modeComboBox.currentIndex !== 2) {
                            audioCodecComboBox.visible = false
                            videoCodecComboBox.visible = false
                            nonDashCodecComboBox.visible = false
                        }

                        else {
                            audioCodecComboBox.visible = true
                            videoCodecComboBox.visible = true
                            nonDashCodecComboBox.visible = true
                        }

                        metadataCodecComboBox.visible = true
                        subtitlesCodecComboBox.visible = true
                        hideMode1.visible = false
                        hideMode2.visible = false
                        hideMode3.visible = false
                        hideMode4.visible = false
                        hideMode5.visible = false
                        hideMode6.visible = false

                        if (addNewDownloadPage.editMode) { textFieldSaveAs.text = "" }
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

                    enabled: nonDashCodecComboBox.currentIndex === 0

                    model: [qsTr("None")]

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

                    enabled: nonDashCodecComboBox.currentIndex === 0

                    model: [qsTr("None")]

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

                    enabled: audioCodecComboBox.currentIndex === 0 && videoCodecComboBox.currentIndex === 0

                    model: [qsTr("None")]

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

                    onPopupOpened: {
                        if (modeComboBox.currentIndex === 2) {
                            textFieldSaveAs.visible = false
                            hideMode6.visible = true
                        }
                    }

                    onPopupClosed: {
                        if (modeComboBox.currentIndex === 2) {
                            textFieldSaveAs.visible = true
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

                Comp.LiquidGlassTextField { id: textFieldSaveAs; implicitWidth: 500; font.pixelSize: 12; readOnly: true }

                Comp.LiquidGlassTextField { id: hideMode6; opacity: 0.1; implicitWidth: 500; font.pixelSize: 12; visible: false }

                Comp.LiquidGlassButton {
                    id: saveAsBtn

                    text: qsTr("Save As")

                    implicitWidth: 160
                    implicitHeight: 50

                    onClicked: {
                        if (checkRequirements()) {
                            askFilters()
                            saveAs.open()
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

                enabled:
                    textFieldURL.text.trim().length > 0
                        && textFieldTitle.text.trim().length > 0
                            && textFieldSaveAs.text.trim().length > 0

                onClicked: {
                    saveAsBtn.enabled = false
                    backBtn.enabled = false

                    const url = textFieldURL.text.trim()
                    const title = textFieldTitle.text.trim()
                    const modeIdx = modeComboBox.currentIndex
                    const ba = modeIdx === 1
                    const bv = modeIdx === 0

                    let ac = ""; let vc = ""; let ndc = ""

                    if (modeIdx === 2) {
                        ac = (audioCodecComboBox.currentIndex === 0) ? "" : audioCodecComboBox.currentText.trim()
                        vc = (videoCodecComboBox.currentIndex === 0) ? "" : videoCodecComboBox.currentText.trim()
                        ndc = (nonDashCodecComboBox.currentIndex === 0) ? "" : nonDashCodecComboBox.currentText.trim()
                    }

                    const sp = textFieldSaveAs.text.trim()

                    if (sp.length === 0) {
                        toast.show("Requires an output path")
                        return
                    }

                    const met = metadataCodecComboBox.currentIndex === 1
                    const subt = subtitlesCodecComboBox.currentIndex === 1

                    loading.running = true
                    loading.visible = true

                    if (addNewDownloadPage.editMode) { backend.editDownload(addNewDownloadPage.internalIdd, url, title, ac, vc, ndc, ba, bv, sp, sp, met, subt) }

                    else { backend.addNewDownload(url, title, ac, vc, ndc, ba, bv, sp, sp, met, subt) }
                }
            }

            Item { Layout.fillWidth: true }
        }
    }


    // ============================
    // Timer for "Search Info"
    // ============================

    property int remainingSeconds1: 0

    Timer {
        id: countdownTimer1

        interval: 1000

        repeat: true

        onTriggered: {
            if (remainingSeconds1 > 0) { --remainingSeconds1 }
            if (remainingSeconds1 === 0) { countdownTimer1.stop() }
        }
    }


    // ============================
    // Timer for "Show Codecs"
    // ============================

    property int remainingSeconds2: 0

    Timer {
        id: countdownTimer2

        interval: 1000

        repeat: true

        onTriggered: {
            if (remainingSeconds2 > 0) { --remainingSeconds2 }
            if (remainingSeconds2 === 0) { countdownTimer2.stop() }
        }
    }


    // ===========================================================
    // Delay Timer: search info after waiting 3 s in editMode
    // ===========================================================

    Timer {
        id: delayTimer

        interval: 3000
        repeat: false

        onTriggered: { backend.getUrlInfo(textFieldURL.text.trim()) }
    }

    FileDialog {
        id: saveAs

        fileMode: FileDialog.SaveFile

        title: qsTr("Save As")

        onAccepted: {
            loading.running = true
            loading.visible = true

            disableInnecessaryComponents()
            backend.checkAvailablePath(saveAs.selectedFile)
        }
    }

    Comp.Toast { id: toast }

    Comp.LoadingOverlay { id: loading; overlayWidth: addNewDownloadPage.width; overlayHeight: addNewDownloadPage.height }

    function disableInnecessaryComponents() {
        textFieldURL.readOnly = true
        remainingSeconds1 = 0
        countdownTimer1.stop()
        searchInfoBtn.enabled = false
        textFieldTitle.readOnly = true
        remainingSeconds2 = 0
        countdownTimer2.stop()
        showCodecBtn.enabled = false
        modeComboBox.readOnly = true
        audioCodecComboBox.readOnly = true
        videoCodecComboBox.readOnly = true
        nonDashCodecComboBox.readOnly = true
        metadataCodecComboBox.readOnly = true
        subtitlesCodecComboBox.readOnly = true
    }

    function checkRequirements() {
        const url = textFieldURL.text.trim()

        if (url.length === 0) { toast.show(qsTr("A URL is required before choosing the save path")); return false }

        const title = textFieldTitle.text.trim()

        if (title.length === 0 || title === qsTr("Failed")) { toast.show(qsTr("A title is required before choosing the save path")); return false }

        const modeIdx = modeComboBox.currentIndex

        if (modeIdx === 0 || modeIdx === 1) { return true }

        const audioIdx = audioCodecComboBox.currentIndex
        const videoIdx = videoCodecComboBox.currentIndex
        const nonDashIdx = nonDashCodecComboBox.currentIndex

        if ((audioIdx === 0 && nonDashIdx === 0)
                || (audioIdx === 0 && videoIdx === 0 && nonDashIdx === 0))
        {
            toast.show(qsTr("An audio code is required before choosing the save path"))
            return false
        }

        return true
    }

    function askFilters() {
        const modeIdx = modeComboBox.currentIndex

        if (modeIdx === 0) { backend.askVideoFilters() }

        else if (modeIdx === 1) { backend.askAudioFilters() }

        else {
            const videoCodecIdx = videoCodecComboBox.currentIndex
            const nonDashIdx = nonDashCodecComboBox.currentIndex

            if (nonDashIdx !== 0) { backend.askVideoFilters() }

            else { if (videoCodecIdx === 0) { backend.askAudioFilters() } else { backend.askVideoFilters() } }
        }
    }

    Connections {
        target: backend
        enabled: addNewDownloadPage.visible

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
            const hasSubtitles = info.hasSubtitles

            for (let aud of info.mpeg_dash_audio_formats) { audio_arr.push(aud) }
            for (let vid of info.mpeg_dash_video_formats) { video_arr.push(vid) }

            audio_arr.unshift(non_selected)
            video_arr.unshift(non_selected)
            non_dash_arr.unshift(non_selected)

            textFieldTitle.text = info.title
            audioCodecComboBox.model = audio_arr
            videoCodecComboBox.model = video_arr
            nonDashCodecComboBox.model = non_dash_arr

            if (!hasSubtitles) {
                subtitlesCodecComboBox.model = [qsTr("Without Subtitles")]
                subtitlesCodecComboBox.currentIndex = 0
            }

            if (addNewDownloadPage.editMode) {
                if (addNewDownloadPage.audioItag.trim() !== "")   {
                    audioCodecComboBox.currentIndex =
                        audioCodecComboBox.find(addNewDownloadPage.audioItag.trim())
                }

                if (addNewDownloadPage.videoItag.trim() !== "")   {
                    videoCodecComboBox.currentIndex =
                        videoCodecComboBox.find(addNewDownloadPage.videoItag.trim())
                }

                if (addNewDownloadPage.nonDashItag.trim() !== "") {
                    nonDashCodecComboBox.currentIndex =
                        nonDashCodecComboBox.find(addNewDownloadPage.nonDashItag.trim())
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

            if (ok) { textFieldSaveAs.text = path }

            else { toast.show(qsTr("Invalid Save Path: is not writable")) }
        }

        function onNewDownloadAdded(accepted, message) {
            loading.visible = false
            loading.running = false

            if (accepted) { Qt.callLater(function() { mainStackView.pop() }) }

            else { toast.show(message); backBtn.enabled = true }
        }

        function onEditInformationRequest(obj, message) {
            if (addNewDownloadPage.editMode && addNewDownloadPage.internalIdd !== 0 && obj.internalId === addNewDownloadPage.internalIdd) {
                textFieldURL.text = obj.url.trim()
                textFieldURL.readOnly = true
                searchInfoBtn.enabled = false
                textFieldTitle.text = obj.title.trim()
                showCodecBtn.enabled = false

                if (obj.bestAudio) { modeComboBox.currentIndex = 1 }

                else if (obj.bestVideo) { modeComboBox.currentIndex = 0 }

                else {
                    modeComboBox.currentIndex = 2
                    addNewDownloadPage.audioItag = obj.audioCode.trim()
                    addNewDownloadPage.videoItag = obj.videoCode.trim()
                    addNewDownloadPage.nonDashItag = obj.nonDashCode.trim()
                }

                delayTimer.restart()

                metadataCodecComboBox.currentIndex = (obj.metadata ? 1 : 0)
                subtitlesCodecComboBox.currentIndex = (obj.subtitles ? 1 : 0)
                textFieldSaveAs.text = modeComboBox.currentIndex === 2 ? "" : obj.savePath.trim()

                loading.running = true
                loading.visible = true
            }

            else { toast.show(message) }
        }
    }
}
