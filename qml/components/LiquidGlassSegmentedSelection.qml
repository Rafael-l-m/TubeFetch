import QtQuick

/*
 * Usage:
 *     enabled: true
 *     currentIndex: 1
 *
 *     Component.onCompleted: {
 *         append(" ")
 *         append(" ")
 *         append(" ")
 *         append(" ")
 *     }
 *
 *     onClicked: function(index) {}
 *     onSelectionChanged: function (index, text) {}
 */

Item {
    readonly property int count: segmentModel.count

    property real minimumSegmentWidth: 80
    property real horizontalPadding: 6
    property real verticalPadding: 5
    property real spacing: 4

    property int currentIndex: 0
    property int dragThreshold: 12

    signal clicked(int index)
    signal selectionChanged(int index, string text)

    id: liquidGlassSegmentedSelection

    implicitWidth: segmentRow.width + liquidGlassSegmentedSelection.horizontalPadding * 2
    implicitHeight: 58

    opacity: enabled ? 1 : 0.4

    Behavior on opacity { NumberAnimation { duration: 200 } }


    // ======================
    // Interaction State
    // ======================

    property bool dragging: false

    property bool dragStarted: false

    property real pressX: 0

    property real indicatorStartX: 0

    property int dragStartIndex: 0


    // ============
    // Helpers
    // ============

    function itemAt(index) { return repeater.itemAt(index) }

    function clamp(value, minValue, maxValue) {
        return Math.max(
                    minValue,
                        Math.min(
                            value,
                            maxValue
                        )
                )
    }


    // ==========
    // Model
    // ==========

    ListModel { id: segmentModel }


    // ===============
    // Background
    // ===============

    Rectangle {
        id: background

        anchors.fill: parent

        radius: height / 2

        color: Qt.rgba(1, 1, 1, 0.075)

        border.width: 1

        border.color: Qt.rgba(1, 1, 1, 0.20)

        gradient: Gradient {
            GradientStop {
                position: 0
                color: Qt.rgba(1, 1, 1, 0.17)
            }

            GradientStop {
                position: 0.45
                color: Qt.rgba(1, 1, 1, 0.07)
            }

            GradientStop {
                position: 1
                color: Qt.rgba(0.70, 0.82, 1, 0.045)
            }
        }


        // ===================
        // Top Reflection
        // ===================

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top

                leftMargin: 12
                rightMargin: 12
                topMargin: 4
            }

            height: parent.height * 0.35
            radius: height / 2

            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: Qt.rgba(1, 1, 1, 0.22)
                }

                GradientStop {
                    position: 1
                    color: Qt.rgba(1, 1, 1, 0.01)
                }
            }
        }
    }

    Item {
        id: content

        x: liquidGlassSegmentedSelection.horizontalPadding
        y: liquidGlassSegmentedSelection.verticalPadding

        width: segmentRow.width
        height: liquidGlassSegmentedSelection.height - liquidGlassSegmentedSelection.verticalPadding * 2

        clip: true
    }


    // ==============
    // Indicator
    // ==============

    Rectangle {
        id: indicator

        parent: content

        x: 0
        y: 0

        width: 0
        height: content.height

        radius:height / 2

        z: 1

        color: Qt.rgba(1, 1, 1, 0.14)

        border.width: 1

        border.color: Qt.rgba(1, 1, 1, 0.34)

        gradient: Gradient {
            GradientStop {
                position: 0
                color: Qt.rgba(1, 1, 1, 0.26)
            }

            GradientStop {
                position: 0.5
                color: Qt.rgba(1, 1, 1, 0.13)
            }

            GradientStop {
                position: 1
                color: Qt.rgba(0.70, 0.85, 1, 0.10)
            }
        }

        Behavior on x {
            enabled: !liquidGlassSegmentedSelection.dragging

            NumberAnimation {
                duration: 320
                easing.type: Easing.OutCubic
            }
        }

        Behavior on width {
            enabled: !liquidGlassSegmentedSelection.dragging

            NumberAnimation {
                duration: 280
                easing.type: Easing.OutCubic
            }
        }
    }


    // ================
    // Segment Row
    // ================

    Row {
        id: segmentRow

        parent: content

        x: 0
        y: 0

        height: content.height

        spacing: liquidGlassSegmentedSelection.spacing

        z: 2

        Repeater {
            id: repeater

            model: segmentModel

            delegate: Item {
                id: segment

                required property int index
                required property string text

                width: Math.max(liquidGlassSegmentedSelection.minimumSegmentWidth, label.implicitWidth + 40)
                height: content.height

                Text {
                    id: label

                    anchors.centerIn: parent

                    text: segment.text

                    color: segment.index === liquidGlassSegmentedSelection.currentIndex
                                ? Qt.rgba(1, 1, 1, 0.98)
                                : Qt.rgba(1, 1, 1, 0.62)

                    font.pixelSize: segment.index === liquidGlassSegmentedSelection.currentIndex ? 16 : 15
                    font.weight: segment.index === liquidGlassSegmentedSelection.currentIndex ? Font.DemiBold : Font.Medium

                    Behavior on color { ColorAnimation { duration: 160 } }
                }


                // ==================
                // Add Animation
                // ==================

                opacity: 0

                scale: 0.75

                Component.onCompleted: { appear.start() }

                ParallelAnimation {
                    id: appear

                    NumberAnimation {
                        target: segment

                        property: "opacity"

                        to: 1

                        duration: 220
                    }

                    NumberAnimation {
                        target: segment

                        property: "scale"

                        to: 1

                        duration: 280

                        easing.type: Easing.OutBack
                    }
                }
            }
        }
    }


    // =======================
    // Interaction
    //
    // Only One MouseArea
    // =======================

    MouseArea {
        id: interaction

        parent: content

        anchors.fill: parent

        z: 10

        hoverEnabled: true

        cursorShape: Qt.PointingHandCursor


        // =================
        // Find Segment
        // =================

        function indexAt(xPos) {
            for (var i = 0; i < liquidGlassSegmentedSelection.count; ++i) {
                var item = liquidGlassSegmentedSelection.itemAt(i)

                if (!item) { continue }

                if (xPos >= item.x && xPos <= item.x + item.width) { return i }
            }

            return -1
        }

        onPressed: function(mouse) {
            liquidGlassSegmentedSelection.pressX = mouse.x

            liquidGlassSegmentedSelection.dragStarted = false

            liquidGlassSegmentedSelection.dragging = false

            liquidGlassSegmentedSelection.dragStartIndex = liquidGlassSegmentedSelection.currentIndex

            liquidGlassSegmentedSelection.indicatorStartX = indicator.x
        }

        onPositionChanged: function(mouse) {
            if (!pressed) { return }

            var delta = mouse.x - liquidGlassSegmentedSelection.pressX


            // ================================================================
            // Still treated as a click if it doesn’t exceed the threshold
            // ================================================================

            if (!liquidGlassSegmentedSelection.dragStarted && Math.abs(delta) < liquidGlassSegmentedSelection.dragThreshold) { return }


            // =======================
            // Real Start: "Drag"
            // =======================

            if (!liquidGlassSegmentedSelection.dragStarted) {
                liquidGlassSegmentedSelection.dragStarted = true
                liquidGlassSegmentedSelection.dragging = true
            }


            // =====================
            // Indicator Follow
            // =====================

            var newX = liquidGlassSegmentedSelection.indicatorStartX + delta
            var maxX = Math.max(0, content.width - indicator.width)

            indicator.x = liquidGlassSegmentedSelection.clamp(newX, 0, maxX)
        }


        // ============
        // Release
        // ============

        onReleased: function(mouse) {


            // ==========
            // CLICK
            // ==========

            if (!liquidGlassSegmentedSelection.dragStarted) {
                var index = indexAt(mouse.x)

                if (index >= 0) {
                    liquidGlassSegmentedSelection.setCurrentIndex(index)
                    liquidGlassSegmentedSelection.clicked(index)
                }

                return
            }


            // =========
            // DRAG
            // =========

            liquidGlassSegmentedSelection.dragging = false
            liquidGlassSegmentedSelection.snapToNearest()
        }

        onCanceled: {
            liquidGlassSegmentedSelection.dragging = false
            liquidGlassSegmentedSelection.snapToCurrent()
        }
    }


    // ================
    // Set Current
    // ================

    function setCurrentIndex(index) {
        if (count <= 0) { return }

        index = clamp(index, 0, count - 1)

        currentIndex = index

        var item = itemAt(index)

        if (!item) { return }


        indicator.width = item.width

        indicator.x = item.x

        selectionChanged(index, item.text)
    }


    // =================
    // Snap Current
    // =================

    function snapToCurrent() { setCurrentIndex(currentIndex) }


    // ====================
    // Snap To Nearest
    // ====================

    function snapToNearest() {
        if (count <= 0) { return }

        var center = indicator.x + indicator.width / 2

        var bestIndex = 0
        var bestDistance = Number.MAX_VALUE

        for (var i = 0; i < count; ++i) {
            var item = itemAt(i)

            if (!item) { continue }

            var itemCenter = item.x + item.width / 2

            var distance = Math.abs(center - itemCenter)

            if (distance < bestDistance) {
                bestDistance = distance
                bestIndex = i
            }
        }

        setCurrentIndex(bestIndex)
    }


    // ==============
    // Model API
    // ==============

    function append(text) {
        segmentModel.append({ "text": text })

        Qt.callLater(function() { initializeIndicator() })
    }

    function insert(index, text) {
        index = clamp(index, 0, count)

        segmentModel.insert(index, { "text": text })

        if (index <= currentIndex && count > 1) { ++currentIndex }

        Qt.callLater(function() { initializeIndicator() })
    }

    function remove(index) {
        if (index < 0 || index >= count) { return }

        segmentModel.remove(index)

        if (index < currentIndex) { --currentIndex }

        if (currentIndex >= count) { currentIndex = Math.max(0, count - 1) }

        Qt.callLater(function() { initializeIndicator() })
    }


    // ==========================================
    // Initialize: show indicator when start
    // ==========================================

    function initializeIndicator() {

        Qt.callLater(function() {
            if (count <= 0) {
                indicator.width = 0
                return
            }

            Qt.callLater(function() { setCurrentIndex(currentIndex) })
        })
    }


    // ====================
    // Initial Startup
    // ====================

    Component.onCompleted: { initializeIndicator() }


    // ==========
    // Debug
    // ==========

    Connections {
        target: segmentRow

        function onWidthChanged() { if (!liquidGlassSegmentedSelection.dragging) { liquidGlassSegmentedSelection.initializeIndicator() } }
    }
}
