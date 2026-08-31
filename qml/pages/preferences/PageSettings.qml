import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Imagine
import QtQuick.Layouts

import "../../components" as Comp


Flickable {
    property bool lightMode

    id: pageSettings

    property alias p_ColumnLayout: mainLayout
    property alias p_ColumnLayout_GridLayout: gridLayout
    property alias p_ColumnLayout_GridLayout_LabelMessage: messageLabel
    property alias p_ColumnLayout_GridLayout_Switch: switchMessageBeforeSaving

    contentWidth: width
    contentHeight: mainLayout.implicitHeight

    Component.onCompleted: { backend.loadSettings() }

    ColumnLayout {
        id: mainLayout

        width: parent.width

        anchors.top: parent.top
        anchors.topMargin: 40

        spacing: 30

        GridLayout {
            id: gridLayout

            Layout.alignment: Qt.AlignHCenter

            columns: 2
            columnSpacing: 20

            rowSpacing: 30

            Label {
                id: messageLabel

                text: qsTr("Message before saving:")

                font.pixelSize: 12
                font.bold: true

                color: pageSettings.lightMode ? "#000000" : "#ffffff"

                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            Switch {
                id: switchMessageBeforeSaving

                checked: true

                text: switchMessageBeforeSaving.checked ? qsTr("Show") : qsTr("Not show")

                font.bold: true
                font.pixelSize: 12

                onCheckedChanged: { backend.updateMessageBeforeSaving(switchMessageBeforeSaving.checked) }
            }
        }
    }

    Connections {
        target: backend
        enabled: pageSettings.visible

        function onSettingsLoaded(obj) { switchMessageBeforeSaving.checked = obj.messageBeforeSaving }
        function onMessageBeforeSavingUpdated(ok) { backend.loadSettings() }
    }
}
