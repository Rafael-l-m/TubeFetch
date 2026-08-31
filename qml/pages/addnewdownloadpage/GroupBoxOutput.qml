import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import "../../components" as Comp


GroupBox {
    property bool lightMode

    property alias p_GridLayout: gridLayout
    property alias p_GridLayout_Label: labelSavePath
    property alias p_GridLayout_InputField: textFieldSavePath
    property alias p_GridLayout_PushButton: saveBtn
    property alias p_label_Label: titleLab
    property alias p_background_Background: backgroundGroupBoxOutput

    id: groupBoxOutput

    font.pixelSize: 12

    Layout.fillWidth: true

    GridLayout {
        id: gridLayout

        anchors.centerIn: parent
        anchors.margins: 20

        columns: 3
        columnSpacing: 16

        rowSpacing: 20

        Label {
            id: labelSavePath

            font.pixelSize: 12
            font.bold: true

            color: groupBoxOutput.lightMode ? "#000000" : "#ffffff"

            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        Comp.InputField {
            id: textFieldSavePath

            implicitWidth: 400
            implicitHeight: 40

            readOnly: true

            lightMode: groupBoxOutput.lightMode
        }

        Comp.PushButton { id: saveBtn; lightMode: groupBoxOutput.lightMode }
    }

    label: Label {
        id: titleLab

        text: parent.title

        anchors.left: parent.left
        anchors.leftMargin: 12

        color: groupBoxOutput.lightMode ? "#000000" : "#ffffff"

        font.pixelSize: 12
        font.bold: true
    }

    background: Rectangle {
        id: backgroundGroupBoxOutput

        border.color: "#dedede"
        border.width: 1

        radius: 6

        color: "transparent"
    }
}
