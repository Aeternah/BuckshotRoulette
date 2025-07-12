import QtQuick 2.15
import QtQuick.Controls 2.15
import Game 1.0

Rectangle {
    id: root
    anchors.fill: parent
    color: "transparent"

    Image {
        anchors.fill: parent
        source: "qrc:/images/menu_bg.png"
        fillMode: Image.PreserveAspectCrop
        opacity: 0.7
        z: -1
        onStatusChanged: {
            if (status === Image.Ready) console.log("Join server screen background loaded")
            else if (status === Image.Error) console.log("Join server screen background error:", source)
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 30

        Text {
            text: "ПОДКЛЮЧЕНИЕ К СЕРВЕРУ"
            font.pixelSize: 64
            color: "white"
            style: Text.Outline
            styleColor: "black"
        }

        TextField {
            id: ipField
            width: 300
            height: 50
            font.pixelSize: 24
            placeholderText: "Введите IP (например, 127.0.0.1)"
            text: "127.0.0.1"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        TextField {
            id: portField
            width: 300
            height: 50
            font.pixelSize: 24
            placeholderText: "Введите порт (например, 12345)"
            text: "12345"
            validator: IntValidator { bottom: 1; top: 65535 }
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Button {
            text: "ПОДКЛЮЧИТЬСЯ"
            width: 300
            height: 80
            font.pixelSize: 24
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                networkLogic.connectToServer(ipField.text, parseInt(portField.text))
                console.log("Connecting to server at", ipField.text, ":", portField.text)
            }
        }

        Button {
            text: "НАЗАД"
            width: 300
            height: 80
            font.pixelSize: 24
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                networkLogic.disconnectClient()
                mainWindow.currentScreen = "networkMenu"
                console.log("Returning to network menu")
            }
        }
    }
}