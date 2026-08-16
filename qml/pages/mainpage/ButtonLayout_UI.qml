import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import "../../components" as Comp

RowLayout {
    property bool lightMode

    property alias p_root: buttonLayout_UI
    property alias p_addBtn: addBtn
    property alias p_removeAllBtn: removeAllBtn

    id: buttonLayout_UI
    Layout.fillWidth: true
    spacing: 10

    Item { Layout.fillWidth: true }

    Comp.P_Button { id: addBtn;       text: qsTr("Add New Download");     implicitWidth: 170; lightMode: buttonLayout_UI.lightMode }

    Item { Layout.fillWidth: true }

    Comp.P_Button { id: removeAllBtn; text: qsTr("Remove All Downloads"); implicitWidth: 170; lightMode: buttonLayout_UI.lightMode }

    Item { Layout.fillWidth: true }
}
