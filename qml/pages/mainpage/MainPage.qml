import QtQuick
import QtQuick.Layouts
import "../../components" as Comp
import "../../download" as Do

Rectangle {
    id: background

    anchors.fill: parent

    color: "transparent"

    ColumnLayout {
        id: columnLayout

        anchors.fill: parent
        anchors.margins: 24

        spacing: 20

        RowLayout {
            id: buttonLayoutControl

            Layout.fillWidth: true

            spacing: 10

            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton { id: startBtn; text: qsTr("Start"); enabled: true }

            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton { id: stopBtn; text: qsTr("Stop"); enabled: false }

            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton { id: showInfo; text: qsTr("Show Info") }

            Item { Layout.fillWidth: true }
        }

        ColumnLayout {
            id: downloadListAndTerminalView

            spacing: 10

            Do.DownloadList {
                id: downloadList

                Layout.fillWidth: true
                Layout.fillHeight: true

                onEditDownloadRequest:   function(idd) { console.log("Edit item: ", idd)   }
                onRemoveDownloadRequest: function(idd) { console.log("Remove item: ", idd) }
                onStartDownloadRequest:  function(idd) { console.log("Start item: ", idd)  }
                onStopDownloadRequest:   function(idd) { console.log("Stop item: ", idd)   }
            }

            Comp.LiquidGlassTerminalView { id: terminalView; Layout.fillWidth: true; Layout.fillHeight: true }
        }

        RowLayout {
            id: buttonLayoutUI

            Layout.fillWidth: true

            spacing: 10

            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton { id: addDownload; text: qsTr("Add New Download"); implicitWidth: 300 }

            Item { Layout.fillWidth: true }

            Comp.LiquidGlassButton { id: removeAllDownloads; text: qsTr("Remove All Downloads"); implicitWidth: 300 }

            Item { Layout.fillWidth: true }
        }
    }
}
