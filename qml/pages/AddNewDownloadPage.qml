import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts
import "../components" as Comp
import "addnewdownloadpage" as LayoutsComp

Page {
    property bool lightMode
    property bool b_showInfoBeforeSaving: true

    property bool b_editMode
    property var qint64_internalId
    property string s_audioItag: ""
    property string s_videoItag: ""
    property string s_nonDashItag: ""

    property alias p_rectangle: rectangle
    property alias p_columnLayout: columnLayout
    property alias p_r1: r1
    property alias p_r2: r2
    property alias p_r3: r3
    property alias p_okBtn: okBtn
    property alias p_FileDialog_SaveAs: saveAs
    property alias p_toast: toast
    property alias p_loadingOverlay: loading
    property alias p_messageDialog: showBeforeChooseSavePath

    id: addNewDownloadPage

    font.pixelSize: 12

    Component.onCompleted: {
        if (b_editMode && qint64_internalId !== 0) {
            backend.requestEditInformation(addNewDownloadPage.qint64_internalId)
        }
    }

    Rectangle {
        id: rectangle

        anchors.fill: parent

        color: lightMode ? "#d7f3ff" : "#20232a"

        ColumnLayout {
            id: columnLayout

            width: parent.width * 0.9

            anchors.centerIn: parent

            spacing: 30

            LayoutsComp.GroupBox_URL {
                id: r1

                title: qsTr("URL:")

                lightMode: addNewDownloadPage.lightMode

                p_gridLayout_label_URL.text: qsTr("URL:")

                p_gridLayout_textField_URL.placeholderText: qsTr("Enter download address ...")
                p_gridLayout_textField_URL.onTextChanged: {
                    if (!addNewDownloadPage.b_editMode) {
                        p_timer1.restart()
                    }
                }

                p_gridLayout_label_Title.text: qsTr("Title:")

                p_gridLayout_textField_Title.enabled: false
                p_gridLayout_textField_Title.color: "#ffffff"

                p_timer1.interval: 1500
                p_timer1.repeat: false
                p_timer1.onTriggered: {
                    if (!addNewDownloadPage.b_editMode) {
                        const url_input = p_gridLayout_textField_URL.text.trim()

                        if (url_input !== "") { urlInfoManager.isValidUrl(url_input) }

                        else {
                            p_gridLayout_textField_URL.text = ""
                            p_gridLayout_textField_Title.text = ""

                            r2.p_gridLayout_comboBox_AudioItag.modelData = [qsTr("None")]
                            r2.p_gridLayout_comboBox_VideoItag.modelData = [qsTr("None")]
                            r2.p_gridLayout_comboBox_NonDash.modelData = [qsTr("None")]
                            r2.p_gridLayout_comboBox_Subtitles.modelData = [qsTr("None")]
                            r2.p_gridLayout_comboBox_Metadata.currentIndex = 0
                            // r2.p_gridLayout_comboBox_Subtitles.currentIndex = 0
                        }
                    }
                }
            }

            LayoutsComp.GroupBox_Config {
                id: r2
                title: qsTr("Config")
                lightMode: addNewDownloadPage.lightMode

                p_gridLayout_label_Mode.text: qsTr("Mode:")
                p_gridLayout_showItagBtn.text: qsTr("Show Itags")
                p_gridLayout_showItagBtn.enabled: r1.p_gridLayout_textField_Title.text.trim() !== ""
                p_gridLayout_showItagBtn.visible: false
                p_gridLayout_showItagBtn.onClicked: {
                    backend.showItags(r1.p_gridLayout_textField_URL.text)
                    windowManager.switchToIndependentWindow("ShowItagsWindow.qml")
                }

                p_gridLayout_comboBox_Mode.modelData: [qsTr("Best Video"), qsTr("Best Audio"), qsTr("Personalized")]
                p_gridLayout_comboBox_Mode.onCurrentIndexChanged: checkChangeTypeEditMode()
                p_gridLayout_comboBox_Mode.onActivated:
                    function (index) {
                        if (index === 2) {
                            p_gridLayout_showItagBtn.visible = true
                            p_gridLayout_label_VideoItag.visible = true
                            p_gridLayout_comboBox_VideoItag.visible = true
                            p_gridLayout_label_AudioItag.visible = true
                            p_gridLayout_comboBox_AudioItag.visible = true
                            p_gridLayout_label_NonDash.visible = true
                            p_gridLayout_comboBox_NonDash.visible = true
                        }

                        else {
                            p_gridLayout_showItagBtn.visible = false
                            p_gridLayout_label_VideoItag.visible = false
                            p_gridLayout_comboBox_VideoItag.visible = false
                            p_gridLayout_label_AudioItag.visible = false
                            p_gridLayout_comboBox_AudioItag.visible = false
                            p_gridLayout_label_NonDash.visible = false
                            p_gridLayout_comboBox_NonDash.visible = false
                        }
                    }

                p_gridLayout_label_AudioItag.visible: false
                p_gridLayout_label_AudioItag.text: qsTr("Audio Itags:")
                p_gridLayout_comboBox_AudioItag.modelData: [qsTr("None")]
                p_gridLayout_comboBox_AudioItag.visible: false
                p_gridLayout_comboBox_AudioItag.onCurrentIndexChanged: {
                    checkSelection()
                    checkChangeTypeEditMode()
                }

                p_gridLayout_label_VideoItag.visible: false
                p_gridLayout_label_VideoItag.text: qsTr("Video Itags:")
                p_gridLayout_comboBox_VideoItag.modelData: [qsTr("None")]
                p_gridLayout_comboBox_VideoItag.visible: false
                p_gridLayout_comboBox_VideoItag.onCurrentIndexChanged: {
                    checkSelection()
                    checkChangeTypeEditMode()
                }

                p_gridLayout_label_NonDash.visible: false
                p_gridLayout_label_NonDash.text: qsTr("Non Dash:")
                p_gridLayout_comboBox_NonDash.modelData: [qsTr("None")]
                p_gridLayout_comboBox_NonDash.visible: false
                p_gridLayout_comboBox_NonDash.onCurrentIndexChanged: {
                    p_gridLayout_comboBox_AudioItag.enabled = p_gridLayout_comboBox_NonDash.currentIndex === 0
                    p_gridLayout_comboBox_VideoItag.enabled = p_gridLayout_comboBox_NonDash.currentIndex === 0
                    checkChangeTypeEditMode()
                }

                p_gridLayout_label_Metadata.text: qsTr("Metadata:")
                p_gridLayout_comboBox_Metadata.modelData: [qsTr("Without Metadata"), qsTr("With Metadata")]

                p_gridLayout_label_Subtitles.text: qsTr("Subtitles:")
                p_gridLayout_label_Subtitles.enabled: false
                p_gridLayout_label_Subtitles.visible: false
                p_gridLayout_comboBox_Subtitles.modelData: [qsTr("None")]
                p_gridLayout_comboBox_Subtitles.enabled: false
                p_gridLayout_comboBox_Subtitles.visible: false
            }

            LayoutsComp.GroupBox_Output {
                id: r3

                title: qsTr("Output")

                lightMode: addNewDownloadPage.lightMode

                p_gridLayout_label_SavePath.text: qsTr("Output Path:")

                p_gridLayout_saveBtn.text: qsTr("Save As")

                p_gridLayout_saveBtn.onClicked: checkRequirements()
            }

            Comp.P_Button {
                id: okBtn

                text: qsTr("OK")

                Layout.alignment: Qt.AlignHCenter

                lightMode: addNewDownloadPage.lightMode

                onClicked: {
                    const url = r1.p_gridLayout_textField_URL.text.trim()
                    const tt = r1.p_gridLayout_textField_Title.text.trim()
                    const mode = r2.p_gridLayout_comboBox_Mode.currentIndex
                    const ba = mode === 1
                    const bv = mode === 0

                    let ac = ""; let vc = ""; let ndc = "";

                    if (mode === 2) {
                        ac = (r2.p_gridLayout_comboBox_AudioItag.currentIndex === 0) ? "" : r2.p_gridLayout_comboBox_AudioItag.currentText
                        vc = (r2.p_gridLayout_comboBox_VideoItag.currentIndex === 0) ? "" : r2.p_gridLayout_comboBox_VideoItag.currentText
                        ndc = (r2.p_gridLayout_comboBox_NonDash.currentIndex === 0) ? "" : r2.p_gridLayout_comboBox_NonDash.currentText
                    }

                    if (addNewDownloadPage.b_editMode) {
                        if (!ba && !bv) {
                            const _ac = ac.trim()
                            const _vc = vc.trim()
                            const _ndc = ndc.trim()

                            if ((_ac === "" && _vc === "" && _ndc === "") || (_ac === "" && _vc !== "")) {
                                toast.show(qsTr("An audio code is required"))
                                return
                            }
                        }
                    }

                    const sp = r3.p_gridLayout_textField_SavePath.text
                    const met = r2.p_gridLayout_comboBox_Metadata.currentIndex === 1
                    const subt = r2.p_gridLayout_comboBox_Subtitles.currentIndex === 1

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

    Comp.P_Toast { id: toast; lightMode: addNewDownloadPage.lightMode }

    Comp.P_LoadingOverlay { id: loading; overlayWidth: width; overlayHeight: height; lightMode: addNewDownloadPage.lightMode }

    Comp.P_MessageDialog {
        id: showBeforeChooseSavePath

        messageText: qsTr("After selecting the file save path, the previous options can no longer be modified\n\nThey can still be adjusted later\n\n**This prompt can be turned off in the settings\n\nAre you sure you want to proceed?")

        onAccepted: {
            beforeSaving()
            saveAs.open()
        }
    }

    Connections {
        target: backend
        enabled: visible

        function onFiltersAsked(mediaFilters) { saveAs.nameFilters = mediaFilters }

        function onAvailablePathChecked(ok, path) {
            loading.visible = false
            loading.running = false

            if (ok) { r3.p_gridLayout_textField_SavePath.text = path }
            else { toast.show(qsTr("Invalid Save Path: is not writable")) }
        }

        function onNewDownloadAdded(accepted, message) {
            loading.visible = false
            loading.running = false

            if (accepted) { Qt.callLater(function() { windowManager.backToMainWindow() }) }
            else { toast.show(message) }
        }

        function onEditInformationRequest(obj, message) {
            if (b_editMode && qint64_internalId !== 0 && obj.internalId === qint64_internalId) {
                r1.p_gridLayout_textField_URL.text = obj.url.trim()
                r1.p_gridLayout_textField_URL.enabled = false
                r1.p_gridLayout_textField_Title.text = obj.title.trim()
                r1.p_gridLayout_textField_Title.enabled = false

                if (obj.bestAudio) { r2.p_gridLayout_comboBox_Mode.currentIndex = 1 }

                else if (obj.bestVideo) { r2.p_gridLayout_comboBox_Mode.currentIndex = 0 }

                else {
                    r2.p_gridLayout_showItagBtn.visible = true
                    r2.p_gridLayout_comboBox_Mode.currentIndex = 2
                    r2.p_gridLayout_label_AudioItag.visible = true
                    r2.p_gridLayout_comboBox_AudioItag.visible = true
                    r2.p_gridLayout_label_VideoItag.visible = true
                    r2.p_gridLayout_comboBox_VideoItag.visible = true
                    r2.p_gridLayout_label_NonDash.visible = true
                    r2.p_gridLayout_comboBox_NonDash.visible = true

                    s_audioItag = obj.audioCode.trim()
                    s_videoItag = obj.videoCode.trim()
                    s_nonDashItag = obj.nonDashCode.trim()
                }

                Qt.callLater(() => { urlInfoManager.getInfo(obj.url.trim()) })

                r2.p_gridLayout_comboBox_Metadata.currentIndex = (obj.metadata ? 1 : 0)
                // r2.p_gridLayout_comboBox_Subtitles.currentIndex = (obj.subtitles ? 1 : 0)
                r3.p_gridLayout_textField_SavePath.text = obj.savePath

                b_showInfoBeforeSaving = false

                loading.running = true
                loading.visible = true
            }

            else { toast.show(message) }
        }
    }

    Connections {
        target: urlInfoManager
        enabled: visible

        function onUrlValid(url, ok) {
            if (!ok) {
                r1.p_gridLayout_textField_Title.text = qsTr("Failed")
                toast.show(qsTr("URL Invalid"))
                return
            }

            loading.running = true
            loading.visible = true

            urlInfoManager.getInfo(url)
        }

        function onJsonReady(info) {
            loading.running = false
            loading.visible = false

            const non_selected = qsTr("Non Selected")
            const audio_arr = []
            const video_arr = []
            const non_dash_arr = info.non_dash_formats

            for (let aud    of info.mpeg_dash_audio_formats) { audio_arr.push(aud)    }
            for (let vid    of info.mpeg_dash_video_formats) { video_arr.push(vid)    }
            for (let vid_ai of info.ai_upscaled_formats)     { video_arr.push(vid_ai) }

            audio_arr.sort(function(a,b)    { return parseInt(a) - parseInt(b) })
            video_arr.sort(function(a,b)    { return parseInt(a) - parseInt(b) })
            non_dash_arr.sort(function(a,b) { return parseInt(a) - parseInt(b) })

            audio_arr.unshift(non_selected)
            video_arr.unshift(non_selected)
            non_dash_arr.unshift(non_selected)

            r1.p_gridLayout_textField_Title.text = info.title
            r2.p_gridLayout_comboBox_AudioItag.modelData = audio_arr
            r2.p_gridLayout_comboBox_VideoItag.modelData = video_arr
            r2.p_gridLayout_comboBox_NonDash.modelData = non_dash_arr

            if (b_editMode) {
                if (s_audioItag.trim() != "")   {
                    r2.p_gridLayout_comboBox_AudioItag.currentIndex =
                        r2.p_gridLayout_comboBox_AudioItag.find(s_audioItag.trim())
                }

                if (s_videoItag.trim() != "")   {
                    r2.p_gridLayout_comboBox_VideoItag.currentIndex =
                        r2.p_gridLayout_comboBox_VideoItag.find(s_videoItag.trim())
                }

                if (s_nonDashItag.trim() != "") {
                    r2.p_gridLayout_comboBox_NonDash.currentIndex =
                        r2.p_gridLayout_comboBox_NonDash.find(s_nonDashItag.trim())
                }
            }
        }

        function onJsonError(message) {
            loading.running = false
            loading.visible = false

            r1.p_gridLayout_textField_Title.text = qsTr("Failed")
            toast.show(message)
        }
    }

    function checkSelection() { r2.p_gridLayout_comboBox_NonDash.enabled = r2.p_gridLayout_comboBox_AudioItag.currentIndex === 0 && r2.p_gridLayout_comboBox_VideoItag.currentIndex === 0 }

    function checkChangeTypeEditMode() { if (addNewDownloadPage.b_editMode) { r3.p_gridLayout_textField_SavePath.text = "" } }

    function checkRequirements() {
        const hasUrl = r1.p_gridLayout_textField_URL.text.trim() !== ""

        if (hasUrl) {
            const title = r1.p_gridLayout_textField_Title.text.trim()
            const hasTitle = title !== "" && title !== qsTr("Failed")

            if (hasTitle) {
                const modeIdx = r2.p_gridLayout_comboBox_Mode.currentIndex

                if (modeIdx === 0 || modeIdx === 1) {
                    if (b_showInfoBeforeSaving) { showBeforeChooseSavePath.open() }
                    else { beforeSaving(); saveAs.open() }
                }

                else {
                    const audioIdx = r2.p_gridLayout_comboBox_AudioItag.currentIndex
                    const videoIdx = r2.p_gridLayout_comboBox_VideoItag.currentIndex
                    const nonDashIdx = r2.p_gridLayout_comboBox_NonDash.currentIndex

                    if ((audioIdx === 0 && nonDashIdx === 0)
                            || (audioIdx === 0 && videoIdx === 0 && nonDashIdx === 0))
                    {
                        toast.show(qsTr("An audio code is required before choosing the save path"))
                    }

                    else {
                        if (b_showInfoBeforeSaving) { showBeforeChooseSavePath.open() }
                        else { beforeSaving(); saveAs.open() }
                    }
                }
            }

            else { toast.show(qsTr("A title is required before choosing the save path")) }
        }

        else { toast.show(qsTr("A URL is required before choosing the save path")) }
    }

    function beforeSaving() {
        r1.p_gridLayout_textField_URL.enabled = false
        r1.p_gridLayout_textField_Title.enabled = false
        r2.p_gridLayout_comboBox_Mode.enabled = false
        r2.p_gridLayout_comboBox_AudioItag.enabled = false
        r2.p_gridLayout_comboBox_VideoItag.enabled = false
        r2.p_gridLayout_comboBox_NonDash.enabled = false
        r2.p_gridLayout_comboBox_Metadata.enabled = false
        r2.p_gridLayout_showItagBtn.enabled = false

        const modeIdx = r2.p_gridLayout_comboBox_Mode.currentIndex

        if (modeIdx === 0) { backend.askVideoFilters() }

        else if (modeIdx === 1) { backend.askAudioFilters() }

        else {
            const audioIdx = r2.p_gridLayout_comboBox_AudioItag.currentIndex
            const videoIdx = r2.p_gridLayout_comboBox_VideoItag.currentIndex
            const nonDashIdx = r2.p_gridLayout_comboBox_NonDash.currentIndex

            if (nonDashIdx !== 0) backend.askVideoFilters()

            else {
                if (videoIdx === 0) backend.askAudioFilters()

                else { backend.askVideoFilters() }
            }
        }
    }
}
