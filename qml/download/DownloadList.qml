import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    signal editDownloadRequest(var idd)
    signal removeDownloadRequest(var idd)
    signal startDownloadRequest(var idd)
    signal stopDownloadRequest(var idd)

    id: downloadList

    radius: 14
    color: Qt.rgba(1, 1, 1, 0.075)

    border.width: 1
    border.color: Qt.rgba(1, 1, 1, 0.22)


    // =========================
    // Outer Soft Glow
    // =========================

    Rectangle {
        anchors.fill: parent
        anchors.margins: -3

        radius: downloadList.radius + 3

        color: "transparent"

        border.width: 4
        border.color: Qt.rgba(1, 1, 1, 0.045)

        opacity: 0.9
    }


    // ========================
    // Main Glass Gradient
    // ========================

    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: Qt.rgba(1, 1, 1, 0.17)
        }

        GradientStop {
            position: 0.42
            color: Qt.rgba(1, 1, 1, 0.065)
        }

        GradientStop {
            position: 1.0
            color: Qt.rgba(0.70, 0.80, 1.0, 0.035)
        }
    }


    // =========================
    // Top Glass Reflection
    // =========================

    /* Rectangle {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top

            leftMargin: 10
            rightMargin: 10
            topMargin: 4
        }

        height: parent.height

        radius: parent.radius

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: Qt.rgba(1, 1, 1, 0.20)
            }

            GradientStop {
                position: 1.0
                color: Qt.rgba(1, 1, 1, 0.005)
            }
        }

        opacity: 0.65

        enabled: false
    } */


    // ======================
    // Bottom Refraction
    // ======================

    Rectangle {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom

            leftMargin: 15
            rightMargin: 15
            bottomMargin: 3
        }

        height: 2
        radius: 1

        color: Qt.rgba(0.65, 0.8, 1.0, 0.16)

        enabled: false
    }

    ListView {
        id: downloadListView

        model: downloadModel

        currentIndex: -1

        anchors.fill: parent
        anchors.margins: 10

        clip: true

        spacing: 8

        delegate: DownloadDelegate {
            width: downloadListView.width

            enabled: downloadList.enabled

            onEditRequest:   function(idd) { downloadList.editDownloadRequest(idd)   }
            onRemoveRequest: function(idd) { downloadList.removeDownloadRequest(idd) }
            onStartRequest:  function(idd) { downloadList.startDownloadRequest(idd)  }
            onStopRequest:   function(idd) { downloadList.stopDownloadRequest(idd)   }
        }

        /* ScrollBar.vertical: ScrollBar {
            id: scrollBar

            policy: ScrollBar.AsNeeded

            width: 5

            background: Rectangle {
                radius: 8

                color: "transparent"
            }

            contentItem: Rectangle {
                implicitWidth: 5

                radius: 12

                color: Qt.rgba(1, 1, 1, 0.20)

                visible: scrollBar.size < 1.0
            }
        } */
    }
}
