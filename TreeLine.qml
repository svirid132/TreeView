import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    color: '#ff99bb'
    property string mainText: display
    property string text: `${node.index.row} ${display}`
    implicitWidth: mouseArea.implicitWidth; implicitHeight: 50
    property var model
    MouseArea {
        id: mouseArea
        implicitWidth: layout.width; implicitHeight: root.implicitHeight
        onClicked: {
            node.toggle()
        }
        RowLayout {
            id: layout
            spacing: 10
            height: parent.implicitHeight
            Label {
                id: text
                text: root.text
                verticalAlignment: Text.AlignVCenter
                color: node.isToggle ? '#330055' : "#33005510"
            }
            Button {
                text: 'изменить'
                onClicked: {
                    model.setData(node.index, root.mainText + '_1')
                }
            }

            Button {
                text: 'добавить'
                onClicked: {
                    model.insertRows(0, 1, node.index)
                }
            }

            Button {
                text: 'удалить'
                onClicked: {
                    model.removeRows(node.index.row, 1, node.parentIndex)
                }
            }
        }
    }
}
