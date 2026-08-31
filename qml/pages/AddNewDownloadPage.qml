import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs

import "../components" as Comp
import "addnewdownloadpage" as LayoutsComp


Page {
    property bool lightMode

    property alias p_Rectangle: rectangle
    property alias p_Rectangle_ColumnLayout: columnLayout
    property alias p_Rectangle_ColumnLayout_GroupBoxURL: r1
    property alias p_Rectangle_ColumnLayout_GroupBoxConfig: r2
    property alias p_Rectangle_ColumnLayout_GroupBoxOutput: r3
    property alias p_Rectangle_ColumnLayout_PushButton: okBtn
    property alias p_FileDialogSaveAs: saveAs
    property alias p_Toast: toast
    property alias p_LoadingOverlay: loading
    property alias p_MessageDialog: showBeforeChooseSavePath
    property alias p_Timer: delayTimer

    property bool b_showInfoBeforeSaving: true
    property bool b_enableRateLimit: true
    property bool b_canPerform: true
    property bool b_editMode: false
    property var qint64_internalId: 0
    property string s_pendingUrl: ""
    property string s_audioItag: ""
    property string s_videoItag: ""
    property string s_nonDashItag: ""

    id: addNewDownloadPage

    font.pixelSize: 12

    Component.onCompleted: {
        if (addNewDownloadPage.b_enableRateLimit) { backend.consume(0, 0, 0) }

        if (addNewDownloadPage.b_editMode && addNewDownloadPage.qint64_internalId !== 0) {
            backend.requestEditInformation(addNewDownloadPage.qint64_internalId)
        }
    }

    Rectangle {
        id: rectangle

        anchors.fill: parent

        color: addNewDownloadPage.lightMode ? "#d7f3ff" : "#20232a"

        ColumnLayout {
            id: columnLayout

            width: parent.width * 0.9

            anchors.centerIn: parent

            spacing: 30

            LayoutsComp.GroupBoxURL {
                id: r1

                title: qsTr("URL")

                lightMode: addNewDownloadPage.lightMode

                p_GridLayout_LabelURL.text: qsTr("URL:")

                p_GridLayout_InputFieldURL.placeholderText:
                    p_GridLayout_InputFieldURL.text.trim() === ""
                        ? qsTr("Enter download address ...")
                        : ""

                p_GridLayout_InputFieldURL.onTextChanged: {
                    const newContent = p_GridLayout_InputFieldURL.text.trim()

                    if (newContent === "") {
                        p_GridLayout_InputFieldURL.text = ""
                        p_GridLayout_InputFieldTitle.text = ""

                        r2.p_GridLayout_ChooseComboBoxAudioCodec.modelData = [qsTr("None")]
                        r2.p_GridLayout_ChooseComboBoxVideoCodec.modelData = [qsTr("None")]
                        r2.p_GridLayout_ChooseComboBoxNonDash.modelData = [qsTr("None")]
                        r2.p_GridLayout_ChooseComboBoxMetadata.currentIndex = 0
                        r2.p_GridLayout_ChooseComboBoxSubtitles.currentIndex = 0

                        return
                    }

                    if (!addNewDownloadPage.b_editMode) { p_Timer.restart() }
                }

                p_GridLayout_LabelTitle.text: qsTr("Title:")

                p_GridLayout_InputFieldTitle.readOnly: true

                p_Timer.onTriggered: {
                    if (addNewDownloadPage.b_editMode) { return }

                    const url = p_GridLayout_InputFieldURL.text.trim()

                    if (url === "") { return }

                    if (!addNewDownloadPage.b_canPerform && addNewDownloadPage.b_enableRateLimit) {
                        toast.show("You have reached the limit. Please wait at least one hour before trying again.")
                        return
                    }

                    backend.isValidUrl(url)
                }
            }

            LayoutsComp.GroupBoxConfig {
                id: r2

                title: qsTr("Config")

                lightMode: addNewDownloadPage.lightMode

                p_GridLayout_LabelMode.text: qsTr("Mode:")

                p_GridLayout_Row_ChooseComboBox.modelData: [qsTr("Best Video"), qsTr("Best Audio"), qsTr("Personalized")]
                p_GridLayout_Row_ChooseComboBox.onCurrentIndexChanged: checkChangeTypeEditMode()
                p_GridLayout_Row_ChooseComboBox.onActivated:
                    function (index) {
                        if (index === 2) {
                            p_GridLayout_Row_PushButton.visible = true
                            p_GridLayout_LabelVideoCodec.visible = true
                            p_GridLayout_ChooseComboBoxVideoCodec.visible = true
                            p_GridLayout_LabelAudioCodec.visible = true
                            p_GridLayout_ChooseComboBoxAudioCodec.visible = true
                            p_GridLayout_LabelNonDashCodec.visible = true
                            p_GridLayout_ChooseComboBoxNonDash.visible = true
                        }

                        else {
                            p_GridLayout_Row_PushButton.visible = false
                            p_GridLayout_LabelVideoCodec.visible = false
                            p_GridLayout_ChooseComboBoxVideoCodec.visible = false
                            p_GridLayout_LabelAudioCodec.visible = false
                            p_GridLayout_ChooseComboBoxAudioCodec.visible = false
                            p_GridLayout_LabelNonDashCodec.visible = false
                            p_GridLayout_ChooseComboBoxNonDash.visible = false
                        }
                    }

                p_GridLayout_Row_PushButton.text: qsTr("Show Codecs")
                p_GridLayout_Row_PushButton.enabled: r1.p_GridLayout_InputFieldTitle.text.trim() !== ""
                p_GridLayout_Row_PushButton.visible: false
                p_GridLayout_Row_PushButton.onClicked: {
                    if (!addNewDownloadPage.b_canPerform && addNewDownloadPage.b_enableRateLimit) {
                        toast.show(qsTr("You have reached the limit. Please wait at least one hour before trying again."))
                        return
                    }

                    backend.showCodecs(r1.p_GridLayout_InputFieldURL.text.trim())
                    windowManager.switchToIndependentWindow("ShowCodecsWindow.qml")

                    if (addNewDownloadPage.b_enableRateLimit) { backend.consume(0, 1, 1) }
                }

                p_GridLayout_LabelVideoCodec.text: qsTr("Video Codecs:")
                p_GridLayout_LabelVideoCodec.visible: false
                p_GridLayout_ChooseComboBoxVideoCodec.modelData: [qsTr("None")]
                p_GridLayout_ChooseComboBoxVideoCodec.visible: false
                p_GridLayout_ChooseComboBoxVideoCodec.onCurrentIndexChanged: {
                    checkSelection()
                    checkChangeTypeEditMode()
                }

                p_GridLayout_LabelAudioCodec.text: qsTr("Audio Codecs:")
                p_GridLayout_LabelAudioCodec.visible: false
                p_GridLayout_ChooseComboBoxAudioCodec.modelData: [qsTr("None")]
                p_GridLayout_ChooseComboBoxAudioCodec.visible: false
                p_GridLayout_ChooseComboBoxAudioCodec.onCurrentIndexChanged: {
                    checkSelection()
                    checkChangeTypeEditMode()
                }

                p_GridLayout_LabelNonDashCodec.text: qsTr("Non Dash:")
                p_GridLayout_LabelNonDashCodec.visible: false
                p_GridLayout_ChooseComboBoxNonDash.modelData: [qsTr("None")]
                p_GridLayout_ChooseComboBoxNonDash.visible: false
                p_GridLayout_ChooseComboBoxNonDash.onCurrentIndexChanged: {
                    p_GridLayout_ChooseComboBoxAudioCodec.enabled = p_GridLayout_ChooseComboBoxNonDash.currentIndex === 0
                    p_GridLayout_ChooseComboBoxVideoCodec.enabled = p_GridLayout_ChooseComboBoxNonDash.currentIndex === 0
                    checkChangeTypeEditMode()
                }

                p_GridLayout_LabelMetadata.text: qsTr("Metadata:")
                p_GridLayout_ChooseComboBoxMetadata.modelData: [qsTr("Without Metadata"), qsTr("With Metadata")]

                p_GridLayout_LabelSubtitles.text: qsTr("Subtitles:")
                p_GridLayout_LabelSubtitles.visible: false
                p_GridLayout_ChooseComboBoxSubtitles.modelData: [qsTr("Without Subtitles")]
                p_GridLayout_ChooseComboBoxSubtitles.visible: false
            }

            LayoutsComp.GroupBoxOutput {
                id: r3

                title: qsTr("Output")

                lightMode: addNewDownloadPage.lightMode

                p_GridLayout_Label.text: qsTr("Output Path:")

                p_GridLayout_PushButton.text: qsTr("Save As")
                p_GridLayout_PushButton.onClicked: checkRequirements()
            }

            Comp.PushButton {
                id: okBtn

                text: qsTr("OK")

                Layout.alignment: Qt.AlignHCenter

                lightMode: addNewDownloadPage.lightMode

                onClicked: {
                    const url = r1.p_GridLayout_InputFieldURL.text.trim()

                    if (url === "") {
                        toast.show(qsTr("Enter download address first"))
                        return
                    }

                    const tt = r1.p_GridLayout_InputFieldTitle.text.trim()

                    if (tt === "" || tt === qsTr("Failed")) {
                        toast.show(qsTr("Requires a valid title"))
                        return
                    }

                    const mode = r2.p_GridLayout_Row_ChooseComboBox.currentIndex
                    const ba = mode === 1
                    const bv = mode === 0

                    let ac = ""; let vc = ""; let ndc = "";

                    if (mode === 2) {
                        ac = (r2.p_GridLayout_ChooseComboBoxAudioCodec.currentIndex === 0) ? "" : r2.p_GridLayout_ChooseComboBoxAudioCodec.currentText
                        vc = (r2.p_GridLayout_ChooseComboBoxVideoCodec.currentIndex === 0) ? "" : r2.p_GridLayout_ChooseComboBoxVideoCodec.currentText
                        ndc = (r2.p_GridLayout_ChooseComboBoxNonDash.currentIndex === 0) ? "" : r2.p_GridLayout_ChooseComboBoxNonDash.currentText
                    }

                    if (!ba && !bv) {
                        const _ac = ac.trim()
                        const _vc = vc.trim()
                        const _ndc = ndc.trim()

                        if ((_ac === "" && _vc === "" && _ndc === "") || (_ac === "" && _vc !== "")) {
                            toast.show(qsTr("An audio code is required"))
                            return
                        }
                    }

                    const sp = r3.p_GridLayout_InputField.text

                    if (sp === "") {
                        toast.show("Requires an output path")
                        return
                    }

                    const met = r2.p_GridLayout_ChooseComboBoxMetadata.currentIndex === 1
                    const subt = r2.p_GridLayout_ChooseComboBoxSubtitles.currentIndex === 1

                    loading.running = true
                    loading.visible = true

                    if (addNewDownloadPage.b_editMode) {
                        backend.editDownload(
                            addNewDownloadPage.qint64_internalId,
                            url,
                            tt,
                            ac,
                            vc,
                            ndc,
                            ba,
                            bv,
                            sp,
                            sp,
                            met,
                            subt
                        )
                    }

                    else {
                        backend.addNewDownload(
                            url,
                            tt,
                            ac,
                            vc,
                            ndc,
                            ba,
                            bv,
                            sp,
                            sp,
                            met,
                            subt
                        )
                    }
                }
            }
        }
    }

    function checkSelection() { r2.p_GridLayout_ChooseComboBoxNonDash.enabled = r2.p_GridLayout_ChooseComboBoxAudioCodec.currentIndex === 0 && r2.p_GridLayout_ChooseComboBoxVideoCodec.currentIndex === 0 }

    function checkChangeTypeEditMode() { if (addNewDownloadPage.b_editMode) { r3.p_GridLayout_InputField.text = "" } }

    function checkRequirements() {
        const url = r1.p_GridLayout_InputFieldURL.text.trim()

        if (url === "") { toast.show(qsTr("A URL is required before choosing the save path")); return }

        const title = r1.p_GridLayout_InputFieldTitle.text.trim()

        if (title === "" || title === qsTr("Failed")) { toast.show(qsTr("A title is required before choosing the save path")); return }

        const modeIdx = r2.p_GridLayout_Row_ChooseComboBox.currentIndex

        if (modeIdx === 0 || modeIdx === 1) {
            if (addNewDownloadPage.b_showInfoBeforeSaving) { showBeforeChooseSavePath.open() }

            else { beforeSaving(); saveAs.open() }

            return
        }

        const audioIdx = r2.p_GridLayout_ChooseComboBoxAudioCodec.currentIndex
        const videoIdx = r2.p_GridLayout_ChooseComboBoxVideoCodec.currentIndex
        const nonDashIdx = r2.p_GridLayout_ChooseComboBoxNonDash.currentIndex

        if ((audioIdx === 0 && nonDashIdx === 0)
                || (audioIdx === 0 && videoIdx === 0 && nonDashIdx === 0))
        {
            toast.show(qsTr("An audio code is required before choosing the save path"))
            return
        }

        if (addNewDownloadPage.b_showInfoBeforeSaving) { showBeforeChooseSavePath.open(); return }

        beforeSaving()
        saveAs.open()
    }

    function beforeSaving() {
        r1.p_GridLayout_InputFieldURL.enabled = false
        r1.p_GridLayout_InputFieldTitle.enabled = false
        r2.p_GridLayout_Row_ChooseComboBox.enabled = false
        r2.p_GridLayout_ChooseComboBoxAudioCodec.enabled = false
        r2.p_GridLayout_ChooseComboBoxVideoCodec.enabled = false
        r2.p_GridLayout_ChooseComboBoxNonDash.enabled = false
        r2.p_GridLayout_ChooseComboBoxMetadata.enabled = false
        r2.p_GridLayout_ChooseComboBoxSubtitles.enabled = false
        r2.p_GridLayout_Row_PushButton.enabled = false

        const modeIdx = r2.p_GridLayout_Row_ChooseComboBox.currentIndex

        if (modeIdx === 0) { backend.askVideoFilters() }

        else if (modeIdx === 1) { backend.askAudioFilters() }

        else {
            const audioIdx = r2.p_GridLayout_ChooseComboBoxAudioCodec.currentIndex
            const videoIdx = r2.p_GridLayout_ChooseComboBoxVideoCodec.currentIndex
            const nonDashIdx = r2.p_GridLayout_ChooseComboBoxNonDash.currentIndex

            if (nonDashIdx !== 0) backend.askVideoFilters()

            else {
                if (videoIdx === 0) backend.askAudioFilters()

                else { backend.askVideoFilters() }
            }
        }
    }

    Comp.Toast { id: toast; lightMode: addNewDownloadPage.lightMode }

    Comp.LoadingOverlay { id: loading; r_overlayWidth: width; r_overlayHeight: height; lightMode: addNewDownloadPage.lightMode }

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
            beforeSaving()
            saveAs.open()
        }
    }

    Timer {
        id: delayTimer

        interval: 3000
        repeat: false

        onTriggered: {
            if (!addNewDownloadPage.b_canPerform
                    && addNewDownloadPage.b_enableRateLimit)
            {
                loading.visible = false
                loading.running = false

                toast.show(qsTr("You have reached the limit. Please wait at least one hour before trying again."))
            }

            else { backend.getUrlInfo(addNewDownloadPage.s_pendingUrl) }
        }
    }

    Connections {
        target: backend
        enabled: addNewDownloadPage.visible

        function onUrlValid(url, ok) {
            if (!ok) {
                r1.p_GridLayout_InputFieldTitle.text = qsTr("Failed")
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

            if (addNewDownloadPage.b_enableRateLimit) { backend.consume(0, 0, 1) }

            const non_selected = qsTr("Non Selected")
            const audio_arr = []
            const video_arr = []
            const non_dash_arr = info.non_dash_formats

            for (let aud of info.mpeg_dash_audio_formats) { audio_arr.push(aud) }
            for (let vid of info.mpeg_dash_video_formats) { video_arr.push(vid) }

            audio_arr.unshift(non_selected)
            video_arr.unshift(non_selected)
            non_dash_arr.unshift(non_selected)

            r1.p_GridLayout_InputFieldTitle.text = info.title
            r2.p_GridLayout_ChooseComboBoxAudioCodec.modelData = audio_arr
            r2.p_GridLayout_ChooseComboBoxVideoCodec.modelData = video_arr
            r2.p_GridLayout_ChooseComboBoxNonDash.modelData = non_dash_arr

            if (addNewDownloadPage.b_editMode) {
                if (addNewDownloadPage.s_audioItag.trim() != "")   {
                    r2.p_GridLayout_ChooseComboBoxAudioCodec.currentIndex =
                        r2.p_GridLayout_ChooseComboBoxAudioCodec.find(addNewDownloadPage.s_audioItag.trim())
                }

                if (addNewDownloadPage.s_videoItag.trim() != "")   {
                    r2.p_GridLayout_ChooseComboBoxVideoCodec.currentIndex =
                        r2.p_GridLayout_ChooseComboBoxVideoCodec.find(addNewDownloadPage.s_videoItag.trim())
                }

                if (addNewDownloadPage.s_nonDashItag.trim() != "") {
                    r2.p_GridLayout_ChooseComboBoxNonDash.currentIndex =
                        r2.p_GridLayout_ChooseComboBoxNonDash.find(addNewDownloadPage.s_nonDashItag.trim())
                }
            }
        }

        function onJsonError(message) {
            loading.running = false
            loading.visible = false

            if (addNewDownloadPage.b_enableRateLimit) { backend.consume(0, 0, 1) }

            r1.p_GridLayout_InputFieldTitle.text = qsTr("Failed")

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
