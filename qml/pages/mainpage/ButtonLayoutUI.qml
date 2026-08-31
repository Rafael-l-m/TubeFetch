import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import "../../components" as Comp


RowLayout {
    property bool lightMode

    property alias p_PushButtonAdd: addBtn
    property alias p_PushButtonRemoveAll: removeAllBtn

    id: buttonLayoutUI

    Layout.fillWidth: true

    spacing: 10

    Item { Layout.fillWidth: true }

    Comp.PushButton { id: addBtn;       text: qsTr("Add New Download");     implicitWidth: 200; lightMode: buttonLayoutUI.lightMode }

    Item { Layout.fillWidth: true }

    Comp.PushButton { id: removeAllBtn; text: qsTr("Remove All Downloads"); implicitWidth: 200; lightMode: buttonLayoutUI.lightMode }

    Item { Layout.fillWidth: true }
}
