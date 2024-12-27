import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import Models 1.0

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    TreeView {
        id: treeView
        width: 300
        height: window.height
        model: TreeModel {
            id: treeModel
        }
        delegate: RowDelegate {
            width: treeView.width
            backgroundColor: n_hasChildren ? 'yellow' : 'orange'
            level: n_level
            text: n_modelIndex.row !== -1 ? treeModel.data(n_modelIndex, 0) : ''
            onOpened: {
                if (n_isOpened) {
                    n_close()
                    return
                }
                n_open()
            }
            onInserted: {
                treeModel.insertRows(0, 1, n_modelIndex)
            }
            onRemoved: {
                treeModel.removeRows(n_modelIndex.row, 1, n_modelIndex.parent)
            }
        }
    }

    Button {
        x: 400
        text: 'reset model'
        onClicked: {
            treeModel.resetModel()
        }
    }
}
