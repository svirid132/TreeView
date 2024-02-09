import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Views 1.0
import Models 1.0

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Item {
        anchors.fill: parent
        TreeModel {
            id: treeModel
        }

        VegaTreeView {
            spacing: 30
            anchors.fill: parent
            model: treeModel
            delegate: TreeLine {
                model: treeModel
            }
        }

        Button {
            text: 'resetModel'
            anchors.bottom: parent.bottom
            onClicked: {
                treeModel.resetModel()
            }
        }
    }

}
