import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import "../../components" as Comp

RowLayout {
    property bool lightMode

    property alias p_root: buttonLayout_Control
    property alias p_startBtn: startBtn
    property alias p_stopBtn: stopBtn
    property alias p_editBtn: editBtn
    property alias p_removeBtn: removeBtn

    id: buttonLayout_Control
    Layout.fillWidth: true
    spacing: 10

    Item { Layout.fillWidth: true }

    Comp.P_Button { id: startBtn;  text: qsTr("Start");  enabled: true;  lightMode: buttonLayout_Control.lightMode }

    Item { Layout.fillWidth: true }

    Comp.P_Button { id: stopBtn;   text: qsTr("Stop");   enabled: false; lightMode: buttonLayout_Control.lightMode }

    Item { Layout.fillWidth: true }

    Comp.P_Button { id: editBtn;   text: qsTr("Edit");   enabled: false; lightMode: buttonLayout_Control.lightMode }

    Item { Layout.fillWidth: true }

    Comp.P_Button { id: removeBtn; text: qsTr("Remove"); enabled: false; lightMode: buttonLayout_Control.lightMode }

    Item { Layout.fillWidth: true }
}
