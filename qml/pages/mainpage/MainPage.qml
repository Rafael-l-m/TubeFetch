import QtQuick
import QtQuick.Layouts

import "../../components" as Comp

Rectangle {
    id: background

    anchors.fill: parent

    color: "transparent"

    ColumnLayout {
        id: columnLayout

        anchors.fill: parent
        anchors.margins: 24

        spacing: 20

        Comp.LiquidGlassTerminalView {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
