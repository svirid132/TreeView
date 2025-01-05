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

    FileSystemModel {
        id: fileSystemModel
    }

    TreeView {
        id: treeView
        anchors.fill: parent
        model: fileSystemModel
        rootModelIndex: fileSystemModel.rootModelIndex()
        delegate: RowDelegate {
            width: treeView.width
            backgroundColor: {
                if (n_animationRunning) {
                    return 'red'
                }
                return n_hasChildren ? 'yellow' : 'orange'
            }
            level: n_level
            text: {
                const filePath = fileSystemModel.filePath(n_modelIndex)
                n_modelIndex.row !== -1 ? fileSystemModel.filePath(n_modelIndex) + ' ' + fileSystemModel.getSizeInMB(filePath) : ''
            }
            onOpened: {
                if (n_animationRunning) {
                    return
                }
                if (n_isOpened) {
                    n_animationClose()
                    return
                }
                n_animationOpen()
            }
            // onInserted: {
            //     treeModel.insertRows(0, 1, n_modelIndex)
            // }
            // onRemoved: {
            //     treeModel.removeRows(n_modelIndex.row, 1, n_modelIndex.parent)
            // }
        }
    }

    Button {
        x: 400
        text: 'reset model'
        onClicked: {
            fileSystemModel.resetModel()
        }
    }
}
