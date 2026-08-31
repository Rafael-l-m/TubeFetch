import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import "../../components" as Comp


RowLayout {
    property bool lightMode

    property alias p_PushButtonStart: startBtn
    property alias p_PushButtonStop: stopBtn
    property alias p_PushButtonEdit: editBtn
    property alias p_PushButtonRemove: removeBtn

    id: buttonLayoutControl

    Layout.fillWidth: true

    spacing: 10

    Item { Layout.fillWidth: true }

    Comp.PushButton { id: startBtn;  text: qsTr("Start");  enabled: true;  lightMode: buttonLayoutControl.lightMode }

    Item { Layout.fillWidth: true }

    Comp.PushButton { id: stopBtn;   text: qsTr("Stop");   enabled: false; lightMode: buttonLayoutControl.lightMode }

    Item { Layout.fillWidth: true }

    Comp.PushButton { id: editBtn;   text: qsTr("Edit");   enabled: false; lightMode: buttonLayoutControl.lightMode }

    Item { Layout.fillWidth: true }

    Comp.PushButton { id: removeBtn; text: qsTr("Remove"); enabled: false; lightMode: buttonLayoutControl.lightMode }

    Item { Layout.fillWidth: true }
}
