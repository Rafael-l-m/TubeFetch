import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import "../../components" as Comp


GroupBox {
    property bool lightMode

    property alias p_GridLayout: gridLayout
    property alias p_GridLayout_LabelMode: labelMode
    property alias p_GridLayout_LabelAudioCodec: labelAudioCodec
    property alias p_GridLayout_LabelVideoCodec: labelVideoCodec
    property alias p_GridLayout_LabelNonDashCodec: labelNonDashCodec
    property alias p_GridLayout_LabelMetadata: labelMetadata
    property alias p_GridLayout_LabelSubtitles: labelSubtitles
    property alias p_GridLayout_Row_ChooseComboBox: comboBoxMode
    property alias p_GridLayout_ChooseComboBoxAudioCodec: comboBoxAudioCodec
    property alias p_GridLayout_ChooseComboBoxVideoCodec: comboBoxVideoCodec
    property alias p_GridLayout_ChooseComboBoxNonDash: comboBoxNonDash
    property alias p_GridLayout_ChooseComboBoxMetadata: comboBoxMetadata
    property alias p_GridLayout_ChooseComboBoxSubtitles: comboBoxSubtitles
    property alias p_GridLayout_Row_PushButton: showItagBtn
    property alias p_label_Label: titleLab
    property alias p_background_Rectangle: backgroundGroupBoxConfig

    id: groupBoxConfig

    font.pixelSize: 12

    Layout.fillWidth: true

    GridLayout {
        id: gridLayout

        anchors.centerIn: parent
        anchors.margins: 20

        columns: 2
        columnSpacing: 25

        rowSpacing: 30

        Label {
            id: labelMode

            font.pixelSize: 12
            font.bold: true

            color: groupBoxConfig.lightMode ? "#000000" : "#ffffff"

            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        Row {
            spacing: 25

            Comp.ChooseComboBox { id: comboBoxMode; lightMode: groupBoxConfig.lightMode }

            Comp.PushButton   { id: showItagBtn;     lightMode: groupBoxConfig.lightMode }
        }

        Label {
            id: labelAudioCodec

            font.pixelSize: 12
            font.bold: true

            color: groupBoxConfig.lightMode ? "#000000" : "#ffffff"

            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        Comp.ChooseComboBox { id: comboBoxAudioCodec; lightMode: groupBoxConfig.lightMode }

        Label {
            id: labelVideoCodec

            font.pixelSize: 12
            font.bold: true

            color: groupBoxConfig.lightMode ? "#000000" : "#ffffff"

            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        Comp.ChooseComboBox { id: comboBoxVideoCodec; lightMode: groupBoxConfig.lightMode }

        Label {
            id: labelNonDashCodec

            font.pixelSize: 12
            font.bold: true

            color: groupBoxConfig.lightMode ? "#000000" : "#ffffff"

            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        Comp.ChooseComboBox { id: comboBoxNonDash; lightMode: groupBoxConfig.lightMode }

        Label {
            id: labelMetadata

            font.pixelSize: 12
            font.bold: true

            color: groupBoxConfig.lightMode ? "#000000" : "#ffffff"

            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        Comp.ChooseComboBox { id: comboBoxMetadata; lightMode: groupBoxConfig.lightMode }

        Label {
            id: labelSubtitles

            font.pixelSize: 12
            font.bold: true

            color: groupBoxConfig.lightMode ? "#000000" : "#ffffff"

            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        Comp.ChooseComboBox { id: comboBoxSubtitles; lightMode: groupBoxConfig.lightMode }
    }

    label: Label {
        id: titleLab

        text: parent.title

        anchors.left: parent.left
        anchors.leftMargin: 12

        color: groupBoxConfig.lightMode ? "#000000" : "#ffffff"

        font.pixelSize: 12
        font.bold: true
    }

    background: Rectangle {
        id: backgroundGroupBoxConfig

        border.color: "#dedede"
        border.width: 1

        radius: 6

        color: "transparent"
    }
}
