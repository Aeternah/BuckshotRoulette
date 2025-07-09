import QtQuick 2.15

Item {
    id: root
    width: 250
    height: 150

    property int currentChamber: 0
    property bool active: true

    Image {
        anchors.fill: parent
        source: "qrc:/images/shotgun.png"
        fillMode: Image.PreserveAspectFit
        opacity: active ? 1.0 : 0.5

        transform: Rotation {
            id: shotgunRotation
            origin.x: width / 2
            origin.y: height / 2
            angle: 0
            Behavior on angle {
                NumberAnimation { duration: 200 }
            }
        }
    }

    Item {
        anchors.centerIn: parent
        width: parent.width * 0.5
        height: width

        Repeater {
            model: 6
            delegate: Rectangle {
                width: 15
                height: 15
                radius: 7.5
                color: index === root.currentChamber ? "red" : "transparent"
                border.color: "white"
                border.width: 1
                x: parent.width / 2 + Math.cos(index * Math.PI / 3) * parent.width * 0.2 - width / 2
                y: parent.height / 2 + Math.sin(index * Math.PI / 3) * parent.height * 0.2 - height / 2
            }
        }
    }
}