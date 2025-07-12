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
            if (status === Image.Ready) console.log("Server screen background loaded")
            else if (status === Image.Error) console.log("Server screen background error:", source)
        }
    }

    Component.onCompleted: {
        networkLogic.startServer("127.0.0.1", 12345)
    }

    Column {
        anchors.centerIn: parent
        spacing: 30

        Text {
            text: "СЕРВЕР СОЗДАН"
            font.pixelSize: 64
            color: "white"
            style: Text.Outline
            styleColor: "black"
        }

        Text {
            text: "IP: 127.0.0.1"
            font.pixelSize: 24
            color: "white"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            text: "Порт: 12345"
            font.pixelSize: 24
            color: "white"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Button {
            text: "ВЫЙТИ"
            width: 300
            height: 80
            font.pixelSize: 24
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                networkLogic.stopServer()
                mainWindow.currentScreen = "networkMenu"
                console.log("Returning to network menu")
            }
        }
    }
}