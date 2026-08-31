import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import "../pages" as Mp


ApplicationWindow {
    property bool lightMode: Application.styleHints.colorScheme === Qt.Light

    property bool b_editMode: false
    property var qint64_internalId: 0

    id: addNewDownloadWindow

    visible: true

    width: 870
    height: 830

    minimumWidth: 870
    minimumHeight: 830

    maximumWidth: 870
    maximumHeight: 830

    Material.theme: Material.Light
    Material.accent: addNewDownloadWindow.lightMode ? "#0015ff" : "#d81209"

    title: qsTr("Add New Download")

    onClosing: (event) => { windowManager.backToMainWindow() }

    Component.onCompleted: { backend.loadSettings() }

    StackView {
        id: stackViewAddNewDownloadWindow

        anchors.fill: parent

        initialItem: Mp.AddNewDownloadPage {
            lightMode: addNewDownloadWindow.lightMode
            b_editMode: addNewDownloadWindow.b_editMode
            qint64_internalId: addNewDownloadWindow.qint64_internalId
        }
    }

    Connections {
        target: backend
        enabled: addNewDownloadWindow.visible

        function onSettingsLoaded(obj) {
            const lma = obj.lightModeAutomatic
            const lm = obj.lightMode
            const rl = obj.rateLimit
            const mbs = obj.messageBeforeSaving

            if (!lma) { addNewDownloadWindow.lightMode = lm }

            else { addNewDownloadWindow.lightMode = Qt.binding(function() {
                return Application.styleHints.colorScheme === Qt.Light
            })}

            stackViewAddNewDownloadWindow.currentItem.b_showInfoBeforeSaving = mbs
            stackViewAddNewDownloadWindow.currentItem.b_enableRateLimit = rl
        }
    }
}
