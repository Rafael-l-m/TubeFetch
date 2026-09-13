import QtQuick

/*
 * Usage:
 *      anchors.centerIn: parent
 *
 *      onClicked: function(index) { }
 *      onSelectionChanged: function(index, text) { }
 *
 *      onItemAdded: function(id, index, text) {
 *          segmentedSelection.currentIndex = index
 *      }
 *
 *      onAddRejected: function(text) { }
 *      onItemRemoved: function(id, index, text) { }
 *
 *      Component.onCompleted: {
 *          append("Main")
 *      }
 */

Item {
    readonly property int count: segmentModel.count

    property int currentIndex: 0

    property int dragThreshold: 12

    property real minimumSegmentWidth: 80
    property real minimumSegmentHeight: 58

    property real horizontalPadding: 6
    property real verticalPadding: 5

    property real spacing: 4

    property real minWidthRatio: 0.3
    property real maxWidthRatio: 0.8

    readonly property real minComponentWidth: parent ? parent.width * minWidthRatio : 0
    readonly property real maxComponentWidth: parent ? parent.width * maxWidthRatio : 0
    readonly property real requiredWidth: count <= 1
                                              ? minComponentWidth
                                              : segmentRow.implicitWidth + horizontalPadding * 2

    id: liquidGlassSegmentedSelection

    width:
        Math.max(
            minComponentWidth,
            Math.min(
                maxComponentWidth,
                requiredWidth
            )
        )

    implicitWidth: minComponentWidth
    implicitHeight: Math.max(parent.height * 0.35, minimumSegmentHeight)

    property bool dragging: false
    property bool dragStarted: false
    property real pressX: 0
    property real indicatorStartX: 0
    property int dragStartIndex: 0

    signal clicked(int index)
    signal selectionChanged(int index, string text)
    signal itemAdded(string id, int index, string text)

    signal addRejected(string text)
    signal itemRemoved(string id, int index, string text)


    // ============
    // Helpers
    // ============

    function clamp(value, minValue, maxValue) {
        return Math.max(
            minValue,
            Math.min(
                value,
                maxValue
            )
        )
    }

    function itemAt(index) { return repeater.itemAt(index) }

    function itemX(item) {
        if (!item) { return 0 }
        return segmentRow.x + item.x
    }


    // ===============================================================
    // Estimate the required width of a button based on its text
    //
    // Match the delegate's: label.implicitWidth + 40
    // ===============================================================

    function widthForText(text) {
        textMetrics.text = text

        return Math.max(
            minimumSegmentWidth,
            textMetrics.width + 40
        )
    }


    // ================================================================
    // Calculate the additional width required for one more button
    // ================================================================

    function requiredWidthWithText(text) {
        if (count === 0) { return minComponentWidth }

        if (count === 1) {
            var firstWidth = widthForText(segmentModel.get(0).text)
            var secondWidth = widthForText(text)

            return firstWidth
                + spacing
                + secondWidth
                + horizontalPadding * 2
        }

        return segmentRow.implicitWidth
            + spacing
            + widthForText(text)
            + horizontalPadding * 2
    }

    ListModel { id: segmentModel }


    // ========================================================
    // Used to pre-measure the text width
    //
    // Calculate the required space before append()
    //
    // Skip appending if it would exceed the maximum width
    // ========================================================

    TextMetrics {
        id: textMetrics

        font.pixelSize: 16
        font.weight: Font.Medium
    }


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


    // ============
    // Content
    // ============

    Item {
        id: content

        x: horizontalPadding
        y: verticalPadding

        width:
            Math.max(
                0,
                liquidGlassSegmentedSelection.width
                - horizontalPadding * 2
            )

        height:
            Math.max(
                0,
                liquidGlassSegmentedSelection.height
                - verticalPadding * 2
            )

        clip: true
    }


    // ================
    // Segment Row
    // ================

    Row {
        id: segmentRow

        parent: content

        // =============================================================================================
        // Row automatically sizes itself using its implicitWidth when it contains multiple buttons
        //
        // For a single button, the delegate directly uses content.width
        // =============================================================================================

        x: count === 1
              ? 0
              : Math.max(
                    0,
                    (content.width - width) / 2
                )

        y: 0

        height: content.height

        spacing: liquidGlassSegmentedSelection.spacing

        z: 2

        Repeater {
            id: repeater

            model: segmentModel

            delegate: Item {
                id: segment

                required property string itemId
                required property string text
                required property int index


                // ==============================================================================
                // With a single button, the button takes up the entire content
                //
                // With multiple buttons, button widths are determined by their text content
                // ==============================================================================

                width: liquidGlassSegmentedSelection.count === 1
                            ? content.width
                            : Math.max(
                                liquidGlassSegmentedSelection.minimumSegmentWidth,
                                label.implicitWidth + 40
                            )

                height: content.height


                // ==========
                // Label
                // ==========

                Text {
                    id: label

                    anchors.centerIn: parent

                    text: segment.text

                    color: segment.index === liquidGlassSegmentedSelection.currentIndex
                                ? Qt.rgba(1, 1, 1, 0.98)
                                : Qt.rgba(1, 1, 1, 0.62)

                    font.pixelSize: segment.index === liquidGlassSegmentedSelection.currentIndex ? 16 : 15

                    font.weight: segment.index === liquidGlassSegmentedSelection.currentIndex
                                    ? Font.DemiBold
                                    : Font.Medium

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

        radius: height / 2

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
    // Interaction
    // ================

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

                var itemX = segmentRow.x + item.x

                if (xPos >= itemX && xPos <= itemX + item.width) { return i }
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
            if (!pressed || liquidGlassSegmentedSelection.count <= 1) { return }

            var delta = mouse.x - liquidGlassSegmentedSelection.pressX

            if (!liquidGlassSegmentedSelection.dragStarted
                    && Math.abs(delta) < liquidGlassSegmentedSelection.dragThreshold) { return }

            if (!liquidGlassSegmentedSelection.dragStarted) {
                liquidGlassSegmentedSelection.dragStarted = true
                liquidGlassSegmentedSelection.dragging = true
            }


            // =====================
            // Indicator Follow
            // =====================

            var newX = liquidGlassSegmentedSelection.indicatorStartX + delta
            var minX = segmentRow.x
            var maxX = segmentRow.x + segmentRow.width - indicator.width

            maxX = Math.max(minX, maxX)

            indicator.x = liquidGlassSegmentedSelection.clamp(newX, minX, maxX)
        }

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


    // ==============
    // Selection
    // ==============

    function setCurrentIndex(index) {
        if (count <= 0) {
            currentIndex = 0
            return
        }

        index = clamp(index, 0, count - 1)

        currentIndex = index

        var item = itemAt(index)

        if (!item) { return }

        indicator.width = item.width

        indicator.x = segmentRow.x + item.x

        selectionChanged(index, item.text)
    }

    function snapToCurrent() { setCurrentIndex(currentIndex) }

    function snapToNearest() {
        if (count <= 0) { return }

        var center = indicator.x + indicator.width / 2
        var bestIndex = 0
        var bestDistance = Number.MAX_VALUE

        for (var i = 0; i < count; ++i) {
            var item = itemAt(i)

            if (!item) { continue }

            var itemCenter = segmentRow.x + item.x + item.width / 2
            var distance = Math.abs(center - itemCenter)

            if (distance < bestDistance) {
                bestDistance = distance
                bestIndex = i
            }
        }

        setCurrentIndex(bestIndex)
    }


    // ===========
    // ID API
    // ===========

    function containsId(id) {
        for (var i = 0; i < count; ++i) { if (segmentModel.get(i).itemId === id) { return true } }
        return false
    }


    // ================================================
    // Find index from id; return -1 if not exists
    // ================================================

    function indexOfId(id) {
        for (var i = 0; i < count; ++i) { if (segmentModel.get(i).itemId === id) { return i } }
        return -1
    }


    // ====================================================
    // Find item from id (return model -> data objects)
    // ====================================================

    function itemDataById(id) {
        var index = indexOfId(id)

        if (index < 0) { return null }

        return segmentModel.get(index)
    }


    // =========================================================
    // Match text -> first match; return -1 if do not match
    // =========================================================

    function indexOfText(text) {
        for (var i = 0; i < count; ++i) { if (segmentModel.get(i).text === text) { return i } }
        return -1
    }


    // =========================================================
    // Match id -> first match; return -1 if do not match
    // =========================================================

    function idOfText(text) {
        var index = indexOfText(text)

        if (index < 0) { return "" }

        return segmentModel.get(index).itemId
    }

    property int nextId: 1

    function generateId() {
        var id = "segment_" + nextId

        ++nextId

        while (containsId(id)) {
            id = "segment_" + nextId
            ++nextId
        }

        return id
    }


    // ===========
    // Append
    // ===========


    // ==========================================================================================
    // Add Button
    //
    // Returns the ID of the newly created item
    //
    // Returns an empty string if the operation fails (Reason: overflows parent.width * 0.8)
    // ==========================================================================================

    function append(text, customId) {
        var newWidth = requiredWidthWithText(text)

        if (newWidth > maxComponentWidth + 0.5) {
            addRejected(text)
            return ""
        }

        var id = customId ? customId : generateId()

        if (containsId(id)) {
            console.warn(
                "LiquidGlassSegmentedSelection:",
                "duplicate id:",
                id
            )

            return ""
        }


        // ========================================================
        // Save the ID of the currently selected item
        //
        // The index may no longer be reliable after appending
        //
        // The ID is the stable identifier
        // ========================================================

        var oldCurrentId = ""

        if (count > 0 && currentIndex >= 0 && currentIndex < count) { oldCurrentId = segmentModel.get(currentIndex).itemId }

        segmentModel.append({
            "itemId": id,
            "text": text
        })

        Qt.callLater(function() {

            // =======================================
            // If an item was previously selected
            //
            // Keep the original item selected
            // =======================================

            if (oldCurrentId !== "") {
                var oldIndex = indexOfId(oldCurrentId)
                if (oldIndex >= 0) { currentIndex = oldIndex }
            }

            if (count === 1) { currentIndex = 0 }

            initializeIndicator()

            var newIndex = indexOfId(id)

            itemAdded(id, newIndex, text)
        })

        return id
    }


    // ===========
    // Insert
    // ===========


    // ===================================================
    // Insert an item at the specified index
    //
    // Returns the ID of the newly created item
    //
    // Returns an empty string if the operation fails
    // ===================================================

    function insert(index, text, customId) {
        index = clamp(index, 0, count)

        var newWidth = requiredWidthWithText(text)

        if (newWidth > maxComponentWidth + 0.5) {
            addRejected(text)
            return ""
        }

        var id = customId ? customId : generateId()

        if (containsId(id)) {
            console.warn(
                "LiquidGlassSegmentedSelection:",
                "duplicate id:",
                id
            )

            return ""
        }

        var oldCurrentId = ""

        if (count > 0 && currentIndex >= 0 && currentIndex < count) {
            oldCurrentId = segmentModel.get(currentIndex).itemId
        }

        segmentModel.insert(
            index,
            {
                "itemId": id,
                "text": text
            }
        )

        Qt.callLater(function() {
            if (oldCurrentId !== "") {
                var oldIndex = indexOfId(oldCurrentId)
                if (oldIndex >= 0) { currentIndex = oldIndex }
            }

            if (count === 1) { currentIndex = 0 }

            initializeIndicator()

            var newIndex = indexOfId(id)

            itemAdded(id, newIndex, text)
        })

        return id
    }


    // ====================
    // Remove By Index
    // ====================

    function remove(index) {
        if (index < 0 || index >= count) { return false }

        var removedData = segmentModel.get(index)
        var removedId = removedData.itemId
        var removedText = removedData.text
        var oldCurrentId = ""

        if (count > 0 && currentIndex >= 0 && currentIndex < count) {
            oldCurrentId = segmentModel.get(currentIndex).itemId
        }

        segmentModel.remove(index)

        if (removedId === oldCurrentId) {
            if (count <= 0) { currentIndex = 0 }

            else {
                currentIndex =
                    Math.min(
                        index,
                        count - 1
                    )
            }
        }

        else {
            var newCurrentIndex = indexOfId(oldCurrentId)

            if (newCurrentIndex >= 0) { currentIndex = newCurrentIndex }

            else if (count > 0) {
                currentIndex =
                    Math.min(
                        currentIndex,
                        count - 1
                    )
            }

            else { currentIndex = 0 }
        }

        Qt.callLater(function() {
            initializeIndicator()
            itemRemoved(removedId, index, removedText)
        })

        return true
    }


    // =================
    // Remove By ID
    // =================

    function removeById(id) {
        var index = indexOfId(id)

        if (index < 0) { return false }

        return remove(index)
    }


    // ===================
    // Remove By Text
    // ===================

    function removeByText(text) {
        var index = indexOfText(text)

        if (index < 0) { return false }

        return remove(index)
    }


    // ===================
    // Remove Current
    // ===================

    function removeCurrent() {
        if (count <= 0) { return false }
        return remove(currentIndex)
    }


    // =========================
    // Initialize Indicator
    // =========================

    function initializeIndicator() {
        Qt.callLater(function() {
            if (count <= 0) {
                indicator.width = 0
                indicator.x = 0
                currentIndex = 0
                return
            }

            if (count === 1) {
                currentIndex = 0
                indicator.width = content.width
                indicator.x = 0
                return
            }

            Qt.callLater(function() {
                if (count <= 0) { return }

                if (currentIndex < 0 || currentIndex >= count) {
                    currentIndex =
                        Math.max(
                            0,
                            Math.min(
                                currentIndex,
                                count - 1
                            )
                        )
                }

                setCurrentIndex(currentIndex)
            })
        })
    }


    // ======================
    // Row Width Changed
    // ======================

    Connections {
        target: segmentRow

        function onWidthChanged() {
            if (!liquidGlassSegmentedSelection.dragging) {
                liquidGlassSegmentedSelection.initializeIndicator()
            }
        }
    }


    // =========================
    // Parent Width Changed
    // =========================

    Connections {
        target: liquidGlassSegmentedSelection.parent

        function onWidthChanged() {
            Qt.callLater(function() {
                liquidGlassSegmentedSelection.initializeIndicator()
            })
        }
    }

    Component.onCompleted: { initializeIndicator() }
}
