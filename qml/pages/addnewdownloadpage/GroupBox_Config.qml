import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import "../../components" as Comp

GroupBox {
    property bool lightMode

    property alias p_label_Label: label_Label
    property alias p_gridLayout: gridLayout
    property alias p_gridLayout_label_Mode: label_Mode
    property alias p_gridLayout_comboBox_Mode: comboBox_Mode
    property alias p_gridLayout_label_VideoItag: label_VideoItag
    property alias p_gridLayout_comboBox_VideoItag: comboBox_VideoItag
    property alias p_gridLayout_label_AudioItag: label_AudioItag
    property alias p_gridLayout_comboBox_AudioItag: comboBox_AudioItag
    property alias p_gridLayout_label_NonDash: label_NonDash
    property alias p_gridLayout_comboBox_NonDash: comboBox_NonDash
    property alias p_gridLayout_label_Metadata: label_Metadata
    property alias p_gridLayout_comboBox_Metadata: comboBox_Metadata
    property alias p_gridLayout_label_Subtitles: label_Subtitles
    property alias p_gridLayout_comboBox_Subtitles: comboBox_Subtitles
    property alias p_gridLayout_showItagBtn: showItagBtn
    property alias p_background_Rectangle: background_Rectangle

    id: groupBoxConfig
    font.pixelSize: 12
    Layout.fillWidth: true

    label: Comp.P_GroupBox_Label { id: label_Label; lightMode: groupBoxConfig.lightMode }

    GridLayout {
        id: gridLayout
        anchors.centerIn: parent
        anchors.margins: 20
        columns: 2
        columnSpacing: 25
        rowSpacing: 30

        Comp.P_Label { id: label_Mode; lightMode: groupBoxConfig.lightMode }

        Row {
            spacing: 25

            Comp.P_ComboBox { id: comboBox_Mode; lightMode: groupBoxConfig.lightMode }

            Comp.P_Button   { id: showItagBtn;   lightMode: groupBoxConfig.lightMode }
        }

        Comp.P_Label    { id: label_AudioItag;    lightMode: groupBoxConfig.lightMode }

        Comp.P_ComboBox { id: comboBox_AudioItag; lightMode: groupBoxConfig.lightMode }

        Comp.P_Label    { id: label_VideoItag;    lightMode: groupBoxConfig.lightMode }

        Comp.P_ComboBox { id: comboBox_VideoItag; lightMode: groupBoxConfig.lightMode }

        Comp.P_Label    { id: label_NonDash;      lightMode: groupBoxConfig.lightMode }

        Comp.P_ComboBox { id: comboBox_NonDash;   lightMode: groupBoxConfig.lightMode }

        Comp.P_Label    { id: label_Metadata;     lightMode: groupBoxConfig.lightMode }

        Comp.P_ComboBox { id: comboBox_Metadata;  lightMode: groupBoxConfig.lightMode }

        Comp.P_Label    { id: label_Subtitles;    lightMode: groupBoxConfig.lightMode }

        Comp.P_ComboBox { id: comboBox_Subtitles; lightMode: groupBoxConfig.lightMode }
    }

    background: Comp.P_Background_Rectangle { id: background_Rectangle }
}
