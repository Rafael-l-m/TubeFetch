import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import "../../components" as Comp

GroupBox {
    property bool lightMode

    property alias p_label_Label: label_Label
    property alias p_gridLayout: gridLayout
    property alias p_gridLayout_label_URL: label_URL
    property alias p_gridLayout_textField_URL: textField_URL
    property alias p_gridLayout_label_Title: label_Title
    property alias p_gridLayout_textField_Title: textField_Title
    property alias p_timer1: timer1
    property alias p_background_Rectangle: background_Rectangle

    id: groupBoxURL
    font.pixelSize: 12
    Layout.fillWidth: true

    label: Comp.P_GroupBox_Label { id: label_Label; lightMode: groupBoxURL.lightMode }

    GridLayout {
        id: gridLayout
        anchors.centerIn: parent
        anchors.margins: 20
        columns: 2
        columnSpacing: 16
        rowSpacing: 20

        Comp.P_Label     { id: label_URL;       lightMode: groupBoxURL.lightMode }

        Comp.P_TextField { id: textField_URL;   lightMode: groupBoxURL.lightMode }

        Comp.P_Label     { id: label_Title;     lightMode: groupBoxURL.lightMode }

        Comp.P_TextField { id: textField_Title; lightMode: groupBoxURL.lightMode }

        Timer { id: timer1 }
    }

    background: Comp.P_Background_Rectangle { id: background_Rectangle }
}
