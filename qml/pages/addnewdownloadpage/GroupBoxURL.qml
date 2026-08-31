import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import "../../components" as Comp


GroupBox {
    property bool lightMode

    property alias p_GridLayout: gridLayout
    property alias p_GridLayout_LabelURL: labelURL
    property alias p_GridLayout_LabelTitle: labelTitle
    property alias p_GridLayout_InputFieldURL: textFieldURL
    property alias p_GridLayout_InputFieldTitle: textFieldTitle
    property alias p_Timer: textInputTimer
    property alias p_label_Label: titleLab
    property alias p_background_Rectangle: backgroundGroupBoxURL

    id: groupBoxURL

    font.pixelSize: 12

    Layout.fillWidth: true

    GridLayout {
        id: gridLayout

        anchors.centerIn: parent
        anchors.margins: 20

        columns: 2
        columnSpacing: 16

        rowSpacing: 20

        Label {
            id: labelURL

            font.pixelSize: 12
            font.bold: true

            color: groupBoxURL.lightMode ? "#000000" : "#ffffff"

            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        Comp.InputField { id: textFieldURL; lightMode: groupBoxURL.lightMode }

        Label {
            id: labelTitle

            font.pixelSize: 12
            font.bold: true

            color: groupBoxURL.lightMode ? "#000000" : "#ffffff"

            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        Comp.InputField { id: textFieldTitle; lightMode: groupBoxURL.lightMode }

    }

    Timer { id: textInputTimer; interval: 1500; repeat: false }

    label: Label {
        id: titleLab

        text: parent.title

        anchors.left: parent.left
        anchors.leftMargin: 12

        color: groupBoxURL.lightMode ? "#000000" : "#ffffff"

        font.pixelSize: 12
        font.bold: true
    }

    background: Rectangle {
        id: backgroundGroupBoxURL

        border.color: "#dedede"
        border.width: 1

        radius: 6

        color: "transparent"
    }
}
