import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root
    implicitHeight: childrenRect.height
    property color backgroundColor:'yellow'
    property int level: -1
    property string text
    signal opened()
    signal inserted()
    signal removed()
    MouseArea {
        id: mouseArea
        width: parent.width
        implicitHeight: childrenRect.height
        onClicked: root.opened()
        RowLayout {
            width: parent.width
            spacing: 0
            Item {
                width: root.level * 20
                height: 1
            }
            Control {
                Layout.fillWidth: true
                padding: 10
                contentItem: Text {
                    clip: true
                    text: root.level + ' ' + root.text
                }
                background: Rectangle {
                    color: root.backgroundColor
                }
            }
            Button {
                implicitWidth: 30
                implicitHeight: 30
                text: '-'
                onClicked: {
                    root.removed()
                }
            }
            Button {
                implicitWidth: 30
                implicitHeight: 30
                text: '+'
                onClicked: {
                    root.inserted()
                }
            }
        }
    }
}
