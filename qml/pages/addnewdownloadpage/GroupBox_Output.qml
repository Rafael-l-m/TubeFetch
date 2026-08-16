import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import "../../components" as Comp

GroupBox {
    property bool lightMode

    property alias p_label_Label: label_Label
    property alias p_gridLayout: gridLayout
    property alias p_gridLayout_label_SavePath: label_SavePath
    property alias p_gridLayout_textField_SavePath: textField_SavePath
    property alias p_gridLayout_saveBtn: saveBtn
    property alias p_background_Rectangle: background_Rectangle

    id: root
    font.pixelSize: 12
    Layout.fillWidth: true

    label: Comp.P_GroupBox_Label { id: label_Label; lightMode: root.lightMode }

    GridLayout {
        id: gridLayout
        anchors.centerIn: parent
        anchors.margins: 20
        columns: 3
        columnSpacing: 16
        rowSpacing: 20

        Comp.P_Label { id: label_SavePath; lightMode: root.lightMode }

        Comp.P_TextField {
            id: textField_SavePath
            implicitWidth: 400
            implicitHeight: 40
            enabled: false
            color: "#ffffff"
            lightMode: root.lightMode
        }

        Comp.P_Button { id: saveBtn; lightMode: root.lightMode }
    }

    background: Comp.P_Background_Rectangle { id: background_Rectangle }
}
