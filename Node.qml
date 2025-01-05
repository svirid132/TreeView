import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root
    implicitWidth: childrenRect.width
    implicitHeight: childrenRect.height

    required property Component delegate
    required property var model
    property var modelIndex: model.index(-1, 0)
    property int level: -1

    property string state: ''

    // Connections {
    //     target: root.model
    //     function onModelReset() {
    //         if (level === -1) {
    //             root.close()
    //             root.open()
    //         }
    //     }
    //     function onRowsInserted(parentModelIndex, first, last) {
    //         if (root.modelIndex === parentModelIndex) {
    //             console.log('inserted!!!')
    //             const items = listToArr(nodeChildrenColumn.children)
    //             itemNodeLoader.update()
    //             if (items.length === 0) {
    //                 return
    //             }
    //             const spliceArr = createNodes(first, last)
    //             items.splice(first, 0, ...spliceArr)
    //             nodeChildrenColumn.children = items
    //             updateChildModelIndex()
    //         }
    //     }
    //     function onRowsRemoved(parentModelIndex, first, last) {
    //         console.log('onRowsRemoved', root.modelIndex === parentModelIndex)
    //         if (root.modelIndex === parentModelIndex) {
    //             console.log('rowsRemoved')
    //             const items = listToArr(nodeChildrenColumn.children)
    //             if (items.length === 0) {
    //                 return
    //             }
    //             itemNodeLoader.update()
    //             for (let ind = first; ind <= last; ++ind) {
    //                 items[ind].destroy()
    //             }
    //             const len = last - first + 1
    //             items.splice(first, len)
    //             nodeChildrenColumn.children = items
    //             updateChildModelIndex()
    //         }
    //     }
    // }
    function open(onFinished) {
        const cb = function() {
            const lastInd = root.model.rowCount(root.modelIndex) - 1
            var nodeChildrenObjs = createNodes(0, lastInd)
            nodeChildrenColumn.children = nodeChildrenObjs
            if(onFinished) {
                onFinished()
            }
        }
        if (root.model.canFetchMore(root.modelIndex)) {
            root.model.fetchMore(root.modelIndex, cb )
            return
        }
        cb()
    }
    function animationOpen() {
        open(function() {
            const cb = function() {
                items.animationImplicitHeight(0, nodeChildrenColumn.implicitHeight)
                nodeChildrenColumn.onImplicitHeightChanged.disconnect(cb)
            }
            nodeChildrenColumn.onImplicitHeightChanged.connect(cb)
        })
    }
    function close() {
        const childrenNodes = nodeChildrenColumn.children
        for (let i = 0; i < childrenNodes.length; ++i) {
            childrenNodes[i].destroy()
        }
        nodeChildrenColumn.children = []
    }
    function animationClose() {
        items.animationImplicitHeight(nodeChildrenColumn.implicitHeight, 0, root.close)
    }
    function listToArr(list) {
        const arr = []
        for (let ind = 0; ind < list.length; ++ind) {
            arr.push(list[ind])
        }
        return arr
    }
    function updateModelIndex(row, parent) {
        const modelIndex = root.model.index(row, 0, parent.modelIndex)
        root.modelIndex = modelIndex
        const items = nodeChildrenColumn.children
        for (let i = 0; i < items.length; ++i) {
            items[i].updateChildModelIndex()
        }
    }
    function updateChildModelIndex() {
        const items = nodeChildrenColumn.children
        for(let i = 0; i < items.length; ++i) {
            items[i].updateModelIndex(i, root)
        }
    }
    function createNodes(firstInd, lastInd) {
        const arr = []
        for (let ind = firstInd; ind <= lastInd; ++ind) {
            let component = Qt.createComponent("Node.qml")
            const modelIndex = root.model.index(ind, 0, root.modelIndex)
            let node = component.createObject(null, {delegate: root.delegate, width: root.width, level: root.level + 1, model: root.model, modelIndex: modelIndex})
            arr.push(node)
        }
        return arr
    }
    ColumnLayout {
        id: columnLayout
        width: parent.width
        spacing: 0
        Loader {
            id: itemNodeLoader
            function update() {
                itemNodeLoader.n_hasChildren = Qt.binding(function() { return root.model.hasChildren(root.modelIndex) })
            }
            property int n_level: root.level
            property var n_modelIndex: root.modelIndex
            property var n_hasChildren: root.model.hasChildren(root.modelIndex)
            property bool n_isOpened: nodeChildrenColumn.children.length !== 0
            property alias n_animationRunning: animation.running
            function n_open() {
                root.open()
            }
            function n_close() {
                root.close()
            }
            function n_animationOpen() {
                root.animationOpen()
            }
            function n_animationClose() {
                root.animationClose()
            }
            sourceComponent: root.delegate
            visible: root.modelIndex.row !== -1
        }
        Item {
            id: items
            implicitWidth: childrenRect.width
            function linkImplicitHeight() {
                items.implicitHeight = Qt.binding(function() { return nodeChildrenColumn.height })
            }
            function animationImplicitHeight(from, to, onFinished) {
                const cb = function() {
                    if (onFinished) {
                        onFinished()
                    }
                    items.linkImplicitHeight()
                    animation.onFinished.disconnect(cb)
                }
                animation.onFinished.connect(cb)
                animation.from = from
                animation.to = to
                animation.start()
            }
            PropertyAnimation {
                id: animation
                target: items
                property: "implicitHeight"
                duration: 500
            }
            ColumnLayout {
                id: nodeChildrenColumn
                spacing: 0
            }
            Component.onCompleted: {
                linkImplicitHeight()
            }
        }
    }
}
