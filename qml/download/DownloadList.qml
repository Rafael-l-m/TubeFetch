import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    property bool lightMode

    property alias p_root: downloadList
    property alias p_MouseArea: ma
    property alias p_MouseArea_ListView: downloadListView
    property alias p_MouseArea_ListView_ScrollBar: scBar

    property var selectedInternalId

    id: downloadList

    radius: 8

    color: lightMode ? "#c9eaf3" : Qt.rgba(255,255,255,0.05)

    border.color: lightMode ? "#d0d0d0" : "#686868"

    MouseArea {
        id: ma

        anchors.fill: parent

        onClicked: { downloadListView.currentIndex = -1 }

        ListView {
            id: downloadListView

            model: downloadModel

            currentIndex: -1

            anchors.fill: parent
            anchors.margins: 10

            clip:true

            spacing:8

            delegate: DownloadDelegate {
                listViewRef: downloadListView

                width: downloadListView.width

                b_lightMode: downloadList.lightMode

                enabled: downloadList.enabled

                p_MouseArea.onClicked: {
                    listViewRef.currentIndex = index
                    selectedInternalId = internalId
                }
            }

            ScrollBar.vertical: ScrollBar {
                id: scBar

                policy: ScrollBar.AsNeeded

                width: 10

                background: Rectangle {
                    id: scBar_backgroud_Rectangle

                    radius: 12

                    color: "transparent"
                }
            }
        }
    }
}
