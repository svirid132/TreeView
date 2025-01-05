import QtQuick 2.15
import Models 1.0

Flickable {
    id: root
    contentWidth: node.width; contentHeight: node.implicitHeight
    property alias delegate: node.delegate
    property alias model: node.model
    property alias rootModelIndex: node.modelIndex
    Node {
        id: node
        width: root.width
        clip: true
    }
    Component.onCompleted: {
        // Раскрываем root Node
        node.open()
    }
}
