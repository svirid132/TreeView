import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root
    implicitWidth: childrenRect.width
    implicitHeight: childrenRect.height

    required property Component delegate
    required property var model
    property var modelIndex: model.index(-1, -1)
    property int level: -1

    function open() {
        const rowCount = root.model.rowCount(root.modelIndex)
        var nodeChildrenObjs = []
        for (let i = 0; i < rowCount; ++i) {
            var component = Qt.createComponent("Node.qml")
            const modelIndex = root.model.index(i, 0, root.modelIndex)
            var node = component.createObject(null, {delegate: root.delegate, width: root.width, level: root.level + 1, model: root.model, modelIndex: modelIndex})
            nodeChildrenObjs.push(node)
        }
        nodeChildrenColumn.children = nodeChildrenObjs
    }

    function close() {
        const childrenNodes = nodeChildrenColumn.children
        for (let i = 0; i < childrenNodes.length; ++i) {
            childrenNodes[i].destroy()
        }
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
            items.updateModelIndex(i, root)
        }
    }
    function updateChildModelIndex() {
        const items = nodeChildrenColumn.children
        for(let i = 0; i < items.length; ++i) {
            items[i].updateModelIndex(i, root)
        }
    }
    function insert(parentModelIndex, first, last) {
        const items = listToArr(nodeChildrenColumn.children)
        const modelIndex = root.modelIndex
        if (modelIndex === parentModelIndex) {
            itemNodeLoader.update()
            if (items.length === 0) {
                return false
            }
            const len = last - first + 1
            const arr = []
            const spliceArr = []
            for (let ind = first; ind <= last; ++ind) {
                var component = Qt.createComponent("Node.qml")
                const modelIndex = root.model.index(ind, 0, root.modelIndex)
                var node = component.createObject(null, {delegate: root.delegate, width: root.width, level: root.level + 1, model: root.model, modelIndex: modelIndex})
                spliceArr.push(node)
            }
            items.splice(first, 0, ...spliceArr)
            nodeChildrenColumn.children = items
            updateChildModelIndex()
            return true
        } else {
            for (let i = 0; i < items.length; ++i) {
                if (items[i].insert(parentModelIndex, first, last)) {
                    return true
                }
            }
        }
        return false
    }
    function remove(parentModelIndex, first, last) {
        const items = listToArr(nodeChildrenColumn.children)
        if (items.length === 0) {
            return false
        }
        if (root.modelIndex === parentModelIndex) {
            itemNodeLoader.update()
            const len = last - first + 1
            items.splice(first, len)
            nodeChildrenColumn.children = items
            updateChildModelIndex()
            return true
        } else {
            for (let i = 0; i < items.length; ++i) {
                if (items[i].remove(parentModelIndex, first, last)) {
                    return true
                }
            }
        }
        return false
    }

    ColumnLayout {
        id: columnLayout
        width: parent.width
        Loader {
            id: itemNodeLoader
            function update() {
                itemNodeLoader.n_hasChildren = Qt.binding(function() { return root.model.hasChildren(root.modelIndex) })
            }
            property int n_level: root.level
            property var n_modelIndex: root.modelIndex
            property var n_hasChildren: root.model.hasChildren(root.modelIndex)
            property bool n_isOpened: nodeChildrenColumn.children.length !== 0
            signal n_open()
            onN_open: {
                root.open()
            }
            signal n_close()
            onN_close: {
                root.close()
            }
            sourceComponent: root.delegate
            visible: root.modelIndex.row !== -1
        }
        Item {
            implicitWidth: childrenRect.width
            implicitHeight: childrenRect.height
            ColumnLayout {
                id: nodeChildrenColumn
            }
        }
    }
}
