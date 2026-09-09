import QtQuick
import QtQuick.Controls
import QtQuick.Window

/*
 * Usage:
 *      width: 280
 *
 *      model: [
 *          " ",
 *          " ",
 *          " ",
 *          " "
 *      ]
 *
 *      currentIndex: 2
 *
 *      onActivated: function(index) {}
 *
 *      or
 *
 *      width: 300
 *
 *      model: [
 *          { name: " ", code: " " },
 *          { name: " ", code: " " }
 *      ]
 *
 *      textRole: "name"
 *      valueRole: "code"
 *
 *      currentIndex: 2
 *
 *      onActivated: function(index) {}
 */

Item {
    property var model: []

    property string textRole: ""
    property string valueRole: ""

    property int currentIndex: -1

    readonly property string currentText: getText(currentIndex)
    readonly property var currentValue: getValue(currentIndex)

    property real cornerRadius: 14
    property real popupSpacing: 6
    property real popupMaxHeight: 300
    property real itemHeight: 42
    property real popupPadding: 5

    property color accentColor: Qt.rgba(0.45, 0.72, 1.0, 1.0)
    property bool readOnly: false

    readonly property bool opened: popup.opened
    readonly property bool hovered: mouseArea.containsMouse
    readonly property bool focused: liquidGlassComboBox.activeFocus || popup.activeFocus

    signal activated(int index)
    signal currentIndexChangedByUser(int index, var value)

    id: liquidGlassComboBox

    implicitWidth: 240

    implicitHeight: 52


    // ==============
    // Functions
    // ==============

    function modelCount() {
        if (!liquidGlassComboBox.model) { return 0 }
        if (liquidGlassComboBox.model.length !== undefined) { return liquidGlassComboBox.model.length }
        return 0
    }

    function getModelData(index) {
        var count = modelCount()

        if (index < 0 || index >= count) { return undefined }

        return liquidGlassComboBox.model[index]
    }

    function getText(index) {
        var data = getModelData(index)

        if (data === undefined) { return "" }


        // ============================
        // Object Model + textRole
        // ============================

        if (liquidGlassComboBox.textRole !== "" && data !== null && typeof data === "object") {
            var value = data[liquidGlassComboBox.textRole]
            if (value !== undefined && value !== null) { return String(value) }
        }


        // ====================
        // Primitive Model
        // ====================

        if (typeof data === "string" || typeof data === "number") { return String(data) }


        // ============================
        // Object Without textRole
        // ============================

        if (data !== null && typeof data === "object") {
            if (data.text !== undefined)  { return String(data.text)  }
            if (data.name !== undefined)  { return String(data.name)  }
            if (data.title !== undefined) { return String(data.title) }
        }

        return ""
    }

    function getValue(index) {
        var data = getModelData(index)

        if (data === undefined) { return undefined }

        if (liquidGlassComboBox.valueRole !== "" && data !== null && typeof data === "object") {
            return data[liquidGlassComboBox.valueRole]
        }

        return data
    }

    function clampIndex(index) {
        var count = modelCount()

        if (count <= 0) { return -1 }

        return Math.max(0, Math.min(index, count - 1))
    }

    function selectIndex(index, userAction) {
        var normalized = clampIndex(index)

        if (normalized < 0) { return }

        var changed = liquidGlassComboBox.currentIndex !== normalized

        liquidGlassComboBox.currentIndex = normalized

        if (userAction) {
            liquidGlassComboBox.activated(normalized)
            liquidGlassComboBox.currentIndexChangedByUser(normalized, liquidGlassComboBox.currentValue)
        }
    }

    function openPopup() {
        if (!liquidGlassComboBox.enabled || liquidGlassComboBox.readOnly || modelCount() <= 0) { return }

        popup.open()
    }

    function closePopup() { if (popup.opened) { popup.close() } }

    function togglePopup() { if (popup.opened) { closePopup() } else { openPopup() } }

    function nextItem() {
        var count = modelCount()

        if (count <= 0) { return }

        var next = liquidGlassComboBox.currentIndex + 1

        if (next >= count)  { next = 0 }

        selectIndex(next, true)
    }

    function previousItem() {
        var count = modelCount()

        if (count <= 0) { return }

        var previous = liquidGlassComboBox.currentIndex - 1

        if (previous < 0) { previous = count - 1 }

        selectIndex(previous, true)
    }


    // ====================================
    // Automatic Model Synchronization
    // ====================================

    function synchronizeModel() {
        var count = modelCount()

        if (count <= 0) {
            liquidGlassComboBox.currentIndex = -1
            return
        }

        if (liquidGlassComboBox.currentIndex < 0 || liquidGlassComboBox.currentIndex >= count) {
            liquidGlassComboBox.currentIndex = 0
        }
    }


    // ===============
    // Outer Glow
    // ===============

    Rectangle {
        id: outerGlow

        anchors.fill: field
        anchors.margins: liquidGlassComboBox.opened || liquidGlassComboBox.focused ? -4 : -2

        radius: liquidGlassComboBox.cornerRadius + 4

        color: "transparent"

        border.width: liquidGlassComboBox.opened || liquidGlassComboBox.focused ? 3 : 2
        border.color: !liquidGlassComboBox.enabled
                            ? Qt.rgba(1, 1, 1, 0.025)
                            : liquidGlassComboBox.readOnly
                                ? Qt.rgba(1, 1, 1, 0.035)
                                : (liquidGlassComboBox.opened || liquidGlassComboBox.focused)
                                    ? Qt.rgba(liquidGlassComboBox.accentColor.r, liquidGlassComboBox.accentColor.g, liquidGlassComboBox.accentColor.b, 0.30)
                                    : liquidGlassComboBox.hovered
                                        ? Qt.rgba(1, 1, 1, 0.11)
                                        : Qt.rgba(1, 1, 1, 0.04)

        opacity: !liquidGlassComboBox.enabled
                    ? 0.25
                    : (liquidGlassComboBox.opened || liquidGlassComboBox.focused)
                        ? 1.0
                        : 0.65

        Behavior on border.color { ColorAnimation { duration: 180 } }

        Behavior on opacity { NumberAnimation { duration: 180 } }

        Behavior on anchors.margins {
            NumberAnimation {
                duration: 180
                easing.type: Easing.OutCubic
            }
        }
    }


    // =====================
    // Main Glass Field
    // =====================

    Rectangle {
        id: field

        anchors.fill: parent

        radius: liquidGlassComboBox.cornerRadius

        color: !liquidGlassComboBox.enabled
                    ? Qt.rgba(1, 1, 1, 0.035)
                    : liquidGlassComboBox.readOnly
                        ? Qt.rgba(0.85, 0.90, 1.0, 0.045)
                        : liquidGlassComboBox.opened
                            ? Qt.rgba(1, 1, 1, 0.115)
                            : liquidGlassComboBox.hovered
                                ? Qt.rgba(1, 1, 1, 0.095)
                                : Qt.rgba(1, 1, 1, 0.075)

        border.width: (liquidGlassComboBox.opened || liquidGlassComboBox.focused) ? 1.5 : 1
        border.color: !liquidGlassComboBox.enabled
                            ? Qt.rgba(1, 1, 1, 0.08)
                            : liquidGlassComboBox.readOnly
                                ? Qt.rgba(1, 1, 1, 0.14)
                                : (liquidGlassComboBox.opened || liquidGlassComboBox.focused)
                                    ? Qt.rgba(liquidGlassComboBox.accentColor.r, liquidGlassComboBox.accentColor.g, liquidGlassComboBox.accentColor.b, 0.48)
                                    : liquidGlassComboBox.hovered
                                        ? Qt.rgba(1, 1, 1, 0.32)
                                        : Qt.rgba(1, 1, 1, 0.20)

        gradient: Gradient {
            GradientStop {
                position: 0
                color: liquidGlassComboBox.opened ? Qt.rgba(1, 1, 1, 0.20) : Qt.rgba(1, 1, 1, 0.15)
            }

            GradientStop {
                position: 0.48
                color: Qt.rgba(1, 1, 1, 0.055)
            }

            GradientStop {
                position: 1
                color: liquidGlassComboBox.opened
                            ? Qt.rgba(liquidGlassComboBox.accentColor.r, liquidGlassComboBox.accentColor.g, liquidGlassComboBox.accentColor.b, 0.09)
                            : Qt.rgba(0.70, 0.80, 1.0, 0.035)
            }
        }

        Behavior on color { ColorAnimation { duration: 160 } }

        Behavior on border.color { ColorAnimation { duration: 180 } }


        // ===================
        // Top Reflection
        // ===================

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top

                leftMargin: 10
                rightMargin: 10
                topMargin: 3
            }

            height: parent.height * 0.36

            radius: height / 2

            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: Qt.rgba(1, 1, 1, liquidGlassComboBox.opened ? 0.27 : 0.20)
                }

                GradientStop {
                    position: 1
                    color: Qt.rgba(1, 1, 1, 0.01)
                }
            }

            opacity: liquidGlassComboBox.enabled ? 1 : 0.4
        }


        // ======================
        // Bottom Refraction
        // ======================

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom

                leftMargin: 12
                rightMargin: 12
                bottomMargin: 3
            }

            height: 2

            radius: 1

            color: liquidGlassComboBox.opened
                        ? Qt.rgba(liquidGlassComboBox.accentColor.r, liquidGlassComboBox.accentColor.g, liquidGlassComboBox.accentColor.b, 0.28)
                        : Qt.rgba(0.65, 0.80, 1.0, 0.15)

            Behavior on color { ColorAnimation { duration: 200 } }
        }
    }


    // ====================
    // ReadOnly Marker
    // ====================

    Text {
        id: readonlyMarker

        anchors {
            left: parent.left

            leftMargin: 13

            verticalCenter: parent.verticalCenter
        }

        text: "◈"

        font.pixelSize: 12

        color: Qt.rgba(0.75, 0.84, 1.0, 0.48)

        visible: liquidGlassComboBox.readOnly

        opacity: liquidGlassComboBox.readOnly ? 1 : 0

        Behavior on opacity { NumberAnimation { duration: 160 } }
    }


    // =================
    // Current Text
    // =================

    Text {
        id: currentLabel

        anchors {
            left: parent.left

            leftMargin: liquidGlassComboBox.readOnly ? 34 : 16

            right: arrow.left

            rightMargin: 12

            verticalCenter: parent.verticalCenter
        }

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        text: liquidGlassComboBox.currentText

        elide: Text.ElideRight

        color: !liquidGlassComboBox.enabled
                    ? Qt.rgba(1, 1, 1, 0.28)
                    : liquidGlassComboBox.readOnly
                        ? Qt.rgba(1, 1, 1, 0.62)
                        : liquidGlassComboBox.currentIndex >= 0
                            ? Qt.rgba(1, 1, 1, 0.96)
                            : Qt.rgba(1, 1, 1, 0.40)

        font.pixelSize: 16

        font.weight: liquidGlassComboBox.readOnly ? Font.Medium : Font.Normal

        Behavior on color { ColorAnimation { duration: 160 } }
    }


    // ==========
    // Arrow
    // ==========

    Item {
        id: arrow

        anchors {
            right: parent.right

            rightMargin: 13

            verticalCenter: parent.verticalCenter
        }

        width: 18
        height: 18

        rotation: liquidGlassComboBox.opened ? 180 : 0

        Behavior on rotation {
            NumberAnimation {
                duration: 240
                easing.type: Easing.OutCubic
            }
        }

        Text {
            anchors.centerIn: parent

            text: "⌄"

            color: !liquidGlassComboBox.enabled
                        ? Qt.rgba(1, 1, 1, 0.20)
                        : liquidGlassComboBox.opened
                            ? Qt.rgba(liquidGlassComboBox.accentColor.r, liquidGlassComboBox.accentColor.g, liquidGlassComboBox.accentColor.b, 0.95)
                            : Qt.rgba(1, 1, 1, 0.62)

            font.pixelSize: 18

            font.weight: Font.Medium

            Behavior on color { ColorAnimation { duration: 180 } }
        }
    }


    // ===========================
    // Main Mouse Interaction
    // ===========================

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        hoverEnabled: true

        enabled: liquidGlassComboBox.enabled && !liquidGlassComboBox.readOnly

        cursorShape: Qt.PointingHandCursor

        onClicked: {
            liquidGlassComboBox.forceActiveFocus()
            liquidGlassComboBox.togglePopup()
        }
    }

    focus: true


    // ==========
    // Popup
    // ==========

    Popup {
        id: popup


        // ================================================
        // Popup itself does not use parent's clipping
        // ================================================

        x: 0
        y: liquidGlassComboBox.height + liquidGlassComboBox.popupSpacing

        width: liquidGlassComboBox.width
        height: Math.min(
                    liquidGlassComboBox.popupMaxHeight,
                    Math.max(
                        liquidGlassComboBox.popupPadding * 2,
                        listView.contentHeight
                        + liquidGlassComboBox.popupPadding * 2
                    )
                )

        padding: 0

        modal: false

        focus: true

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside


        // =====================
        // Popup Background
        // =====================

        background: Rectangle {
            id: popupBackground

            radius: liquidGlassComboBox.cornerRadius + 2

            color: Qt.rgba(0.10, 0.13, 0.19, 0.82)

            border.width: 1
            border.color: Qt.rgba(1, 1, 1, 0.20)

            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: Qt.rgba(1, 1, 1, 0.17)
                }

                GradientStop {
                    position: 0.45
                    color: Qt.rgba(1, 1, 1, 0.065)
                }

                GradientStop {
                    position: 1
                    color: Qt.rgba(0.45, 0.60, 0.85, 0.10)
                }
            }


            // =========================
            // Popup Top Reflection
            // =========================

            Rectangle {
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top

                    leftMargin: 12
                    rightMargin: 12
                    topMargin: 4
                }

                height: parent.height * 0.12

                radius: height / 2

                gradient: Gradient {
                    GradientStop {
                        position: 0
                        color: Qt.rgba(1, 1, 1, 0.20)
                    }

                    GradientStop {
                        position: 1
                        color: Qt.rgba(1, 1, 1, 0.01)
                    }
                }
            }
        }


        // ========================================================
        // Popup Entrance Animation
        // ========================================================

        enter: Transition {
            ParallelAnimation {
                NumberAnimation {
                    property: "opacity"

                    from: 0
                    to: 1

                    duration: 180

                    easing.type: Easing.OutCubic
                }

                NumberAnimation {
                    property: "scale"

                    from: 0.94
                    to: 1.0

                    duration: 220

                    easing.type: Easing.OutBack
                }

                NumberAnimation {
                    property: "y"

                    from: liquidGlassComboBox.height + liquidGlassComboBox.popupSpacing - 8
                    to: liquidGlassComboBox.height + liquidGlassComboBox.popupSpacing

                    duration: 220

                    easing.type: Easing.OutCubic
                }
            }
        }


        // ========================================================
        // Popup Exit Animation
        // ========================================================

        exit: Transition {
            ParallelAnimation {
                NumberAnimation {
                    property: "opacity"

                    from: 1
                    to: 0

                    duration: 130
                }

                NumberAnimation {
                    property: "scale"

                    from: 1.0
                    to: 0.96

                    duration: 150

                    easing.type: Easing.InCubic
                }
            }
        }


        // ========================================================
        // List
        // ========================================================

        ListView {
            id: listView

            anchors {
                fill: parent

                leftMargin: liquidGlassComboBox.popupPadding
                rightMargin: liquidGlassComboBox.popupPadding
                topMargin: liquidGlassComboBox.popupPadding
                bottomMargin: liquidGlassComboBox.popupPadding
            }

            Keys.onPressed: function(event) {
                if (!liquidGlassComboBox.enabled) { return }

                if (event.key === Qt.Key_Space || event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                    liquidGlassComboBox.togglePopup()

                    event.accepted = true

                    return
                }

                if (event.key === Qt.Key_Down) {
                    if (!liquidGlassComboBox.opened) { liquidGlassComboBox.openPopup() }

                    liquidGlassComboBox.nextItem()

                    event.accepted = true

                    return
                }

                if (event.key === Qt.Key_Up) {
                    if (!liquidGlassComboBox.opened) { liquidGlassComboBox.openPopup() }

                    liquidGlassComboBox.previousItem()

                    event.accepted = true

                    return
                }


                if (event.key === Qt.Key_Escape) {
                    liquidGlassComboBox.closePopup()

                    event.accepted = true

                    return
                }
            }

            clip: true

            spacing: 3

            model: liquidGlassComboBox.model

            currentIndex: liquidGlassComboBox.currentIndex

            boundsBehavior: Flickable.StopAtBounds

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded

                // ====================================
                // Need ......
                // ====================================
            }

            delegate: Item {

                id: delegateRoot


                width:
                    listView.width


                height:
                    liquidGlassComboBox.itemHeight


                property bool itemHovered:
                    delegateMouse.containsMouse


                property bool itemSelected:
                    index === liquidGlassComboBox.currentIndex


                /*
                 * =================================================
                 * Selection Liquid Layer
                 * =================================================
                 */

                Rectangle {

                    id: selectionBackground


                    anchors.fill:
                        parent


                    radius:
                        liquidGlassComboBox.cornerRadius - 4


                    color:

                        delegateRoot.itemSelected

                        ? Qt.rgba(
                              liquidGlassComboBox.accentColor.r,
                              liquidGlassComboBox.accentColor.g,
                              liquidGlassComboBox.accentColor.b,
                              0.16
                          )

                        : delegateRoot.itemHovered

                          ? Qt.rgba(
                                1,
                                1,
                                1,
                                0.09
                            )

                          : "transparent"


                    border.width:
                        delegateRoot.itemSelected
                        ? 1
                        : 0


                    border.color:
                        Qt.rgba(
                            liquidGlassComboBox.accentColor.r,
                            liquidGlassComboBox.accentColor.g,
                            liquidGlassComboBox.accentColor.b,
                            0.32
                        )


                    Behavior on color {

                        ColorAnimation {
                            duration: 140
                        }
                    }


                    Behavior on border.color {

                        ColorAnimation {
                            duration: 140
                        }
                    }
                }


                /*
                 * =================================================
                 * Selection Highlight
                 * =================================================
                 */

                Rectangle {

                    anchors {

                        left:
                            parent.left

                        top:
                            parent.top

                        bottom:
                            parent.bottom
                    }


                    width:
                        delegateRoot.itemSelected
                        ? 3
                        : 0


                    radius:
                        width / 2


                    color:
                        Qt.rgba(
                            liquidGlassComboBox.accentColor.r,
                            liquidGlassComboBox.accentColor.g,
                            liquidGlassComboBox.accentColor.b,
                            0.75
                        )


                    Behavior on width {

                        NumberAnimation {

                            duration: 180

                            easing.type:
                                Easing.OutCubic
                        }
                    }
                }


                /*
                 * =================================================
                 * Item Text
                 * =================================================
                 */

                Text {

                    anchors {

                        left:
                            parent.left

                        leftMargin: 15

                        right:
                            checkMark.left

                        rightMargin: 8

                        verticalCenter:
                            parent.verticalCenter
                    }


                    text:
                        liquidGlassComboBox.getText(index)


                    elide:
                        Text.ElideRight


                    color:

                        delegateRoot.itemSelected

                        ? Qt.rgba(
                              1,
                              1,
                              1,
                              0.98
                          )

                        : delegateRoot.itemHovered

                          ? Qt.rgba(
                                1,
                                1,
                                1,
                                0.88
                            )

                          : Qt.rgba(
                                1,
                                1,
                                1,
                                0.68
                            )


                    font.pixelSize:
                        15


                    font.weight:

                        delegateRoot.itemSelected

                        ? Font.DemiBold

                        : Font.Medium


                    Behavior on color {

                        ColorAnimation {
                            duration: 120
                        }
                    }
                }


                /*
                 * =================================================
                 * Check Mark
                 * =================================================
                 */

                Text {

                    id: checkMark


                    anchors {

                        right:
                            parent.right

                        rightMargin: 14

                        verticalCenter:
                            parent.verticalCenter
                    }


                    text:
                        "✓"


                    color:
                        Qt.rgba(
                            liquidGlassComboBox.accentColor.r,
                            liquidGlassComboBox.accentColor.g,
                            liquidGlassComboBox.accentColor.b,
                            0.95
                        )


                    font.pixelSize:
                        15


                    font.weight:
                        Font.Bold


                    opacity:
                        delegateRoot.itemSelected
                        ? 1
                        : 0


                    scale:
                        delegateRoot.itemSelected
                        ? 1
                        : 0.6


                    Behavior on opacity {

                        NumberAnimation {
                            duration: 140
                        }
                    }


                    Behavior on scale {

                        NumberAnimation {

                            duration: 180

                            easing.type:
                                Easing.OutBack
                        }
                    }
                }


                /*
                 * =================================================
                 * Delegate Mouse
                 * =================================================
                 */

                MouseArea {

                    id: delegateMouse


                    anchors.fill:
                        parent


                    hoverEnabled: true


                    cursorShape:
                        Qt.PointingHandCursor


                    onClicked: {

                        liquidGlassComboBox.selectIndex(
                            index,
                            true
                        )


                        popup.close()
                    }
                }


                /*
                 * =================================================
                 * Add Animation
                 * =================================================
                 */

                opacity: 0

                scale: 0.96


                Component.onCompleted: {

                    itemAppear.start()
                }


                ParallelAnimation {

                    id: itemAppear


                    NumberAnimation {

                        target:
                            delegateRoot

                        property:
                            "opacity"

                        from: 0
                        to: 1

                        duration: 180

                        easing.type:
                            Easing.OutCubic
                    }


                    NumberAnimation {

                        target:
                            delegateRoot

                        property:
                            "scale"

                        from: 0.96
                        to: 1

                        duration: 220

                        easing.type:
                            Easing.OutBack
                    }
                }
            }


            /*
             * ====================================================
             * Current Index Synchronization
             * ====================================================
             */

            onCurrentIndexChanged: {

                if (
                    currentIndex >= 0 &&
                    currentIndex < count
                ) {

                    positionViewAtIndex(
                        currentIndex,
                        ListView.Contain
                    )
                }
            }
        }


        /*
         * ========================================================
         * Popup Focus
         * ========================================================
         */

        /*Keys.onPressed:
            function(event) {

            if (
                event.key === Qt.Key_Escape
            ) {

                popup.close()

                event.accepted = true

                return
            }


            if (
                event.key === Qt.Key_Down
            ) {

                liquidGlassComboBox.nextItem()

                event.accepted = true

                return
            }


            if (
                event.key === Qt.Key_Up
            ) {

                liquidGlassComboBox.previousItem()

                event.accepted = true

                return
            }


            if (
                event.key === Qt.Key_Return ||
                event.key === Qt.Key_Enter
            ) {

                popup.close()

                event.accepted = true
            }
        }*/


        /*
         * ========================================================
         * Popup Open
         * ========================================================
         */

        onOpened: {

            listView.forceActiveFocus()


            if (
                liquidGlassComboBox.currentIndex >= 0
            ) {

                listView.positionViewAtIndex(
                    liquidGlassComboBox.currentIndex,
                    ListView.Contain
                )
            }
        }


        onClosed: {

            liquidGlassComboBox.forceActiveFocus()
        }
    }


    /*
     * ============================================================
     * Model Changes
     * ============================================================
     */

    onModelChanged: {

        synchronizeModel()
    }


    onTextRoleChanged: {


    }


    /*
     * ============================================================
     * Startup
     * ============================================================
     */

    Component.onCompleted: {

        synchronizeModel()
    }
}
