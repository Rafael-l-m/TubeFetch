import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Imagine
import QtQuick.Layouts

import "../../components" as Comp


Flickable {
    property bool lightMode

    property alias p_ColumnLayout: mainLayout
    property alias p_ColumnLayout_GridLayout: gridLayout
    property alias p_ColumnLayout_GridLayout_LabelScheme: schemeLabel
    property alias p_ColumnLayout_GridLayout_SegmentedControl: lightModeSegmentedControl
    property alias p_ColumnLayout_GridLayout_LabelSelfCheck: selfCheckLabel
    property alias p_ColumnLayout_GridLayout_SwitchSelfCheck: switchSelfCheck
    property alias p_ColumnLayout_GridLayout_LabelRateLimit: rateLimitLabel
    property alias p_ColumnLayout_GridLayout_SwitchRateLimit: switchRateLimit

    id: generalSettings

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
                id: schemeLabel

                text: qsTr("Scheme:")

                font.pixelSize: 12
                font.bold: true

                color: generalSettings.lightMode ? "#000000" : "#ffffff"

                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            Comp.SegmentedControl {
                id: lightModeSegmentedControl

                lightMode: generalSettings.lightMode

                options: [qsTr("Light"), qsTr("Dark"), qsTr("Auto")]

                currentIndex: 2

                onCurrentIndexChanged: {
                    if (currentIndex === 2) { backend.updateLightMode(true, true) }

                    else { backend.updateLightMode(false, currentIndex === 0) }
                }
            }

            Label {
                id: selfCheckLabel

                text: qsTr("Initiate self‑check:")

                font.pixelSize: 12
                font.bold: true

                color: generalSettings.lightMode ? "#000000" : "#ffffff"

                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            Switch {
                id: switchSelfCheck

                checked: true

                text: switchSelfCheck.checked ? qsTr("True") : qsTr("False")

                font.bold: true
                font.pixelSize: 12

                onCheckedChanged: { backend.updateSelfCheck(switchSelfCheck.checked) }
            }

            Label {
                id: rateLimitLabel

                text: qsTr("Enable rate limit:")

                font.pixelSize: 12
                font.bold: true

                color: generalSettings.lightMode ? "#000000" : "#ffffff"

                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            Switch {
                id: switchRateLimit

                checked: true

                text: switchRateLimit.checked ? qsTr("True") : qsTr("False")

                font.bold: true
                font.pixelSize: 12

                onCheckedChanged: { backend.updateRateLimit(switchRateLimit.checked) }
            }
        }
    }

    Connections {
        target: backend
        enabled: generalSettings.visible

        function onSettingsLoaded(obj) {
            const lma = obj.lightModeAutomatic
            const lm = obj.lightMode
            const sc = obj.selfCheck
            const rl = obj.rateLimit

            if (lma) { lightModeSegmentedControl.currentIndex = 2 }

            else { lightModeSegmentedControl.currentIndex = lm ? 0 : 1 }

            switchSelfCheck.checked = sc
            switchRateLimit.checked = rl
        }

        function onLightModeUpdated(ok) { backend.loadSettings() }
        function onSelfCheckUpdated(ok) { backend.loadSettings() }
        function onRateLimitUpdated(ok) { backend.loadSettings() }
    }
}
