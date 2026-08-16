import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import "../pages" as Mp

ApplicationWindow {
    property bool lightMode: Application.styleHints.colorScheme === Qt.Light
    property bool b_editMode: false
    property var qint64_internalId: 0

    id: window

    visible: true

    width: 870
    height: 830

    maximumWidth: window.width
    minimumWidth: window.width

    maximumHeight: window.height
    minimumHeight: window.height

    Material.theme: Material.Light
    Material.accent: lightMode ? "#0015ff" : "#d81209"

    title: qsTr("Add New Download")

    onClosing: (event) => { windowManager.backToMainWindow() }

    StackView {
        id: stackView2
        anchors.fill: parent
        initialItem: Mp.AddNewDownloadPage {
            lightMode: window.lightMode
            b_editMode: window.b_editMode
            qint64_internalId: window.qint64_internalId
        }
    }
}
