import QtQuick 2.15
import Models 1.0

Flickable {
    id: root
    contentWidth: node.width; contentHeight: node.height
    property alias delegate: node.delegate
    property alias model: node.model
    Node {
        id: node
        width: root.width
    }
    Connections {
        target: root.model
        function onModelReset() {
            node.close()
            node.open()
        }
        function onRowsInserted(parent, first, last) {
            node.insert(parent, first, last)
        }
        function onRowsRemoved(parent, first, last) {
            node.remove(parent, first, last)
        }
    }
    Component.onCompleted: {
        // Раскрываем root Node
        node.open()
    }
}
