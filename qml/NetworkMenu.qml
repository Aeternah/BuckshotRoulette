import QtQuick 2.15
import QtQuick.Controls 2.15

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
            if (status === Image.Ready) console.log("Network menu background loaded")
            else if (status === Image.Error) console.log("Network menu background error:", source)
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 30

        Button {
            text: "СОЗДАТЬ СЕРВЕР"
            width: 300
            height: 80
            font.pixelSize: 24
            onClicked: {
                mainWindow.currentScreen = "serverScreen"
                console.log("Switching to server screen")
            }
        }

        Button {
            text: "ПОДКЛЮЧИТЬСЯ К СЕРВЕРУ"
            width: 300
            height: 80
            font.pixelSize: 24
            onClicked: {
                mainWindow.currentScreen = "joinServerScreen"
                console.log("Switching to join server screen")
            }
        }

        Button {
            text: "НАЗАД"
            width: 300
            height: 80
            font.pixelSize: 24
            onClicked: {
                mainWindow.currentScreen = "menu"
                console.log("Returning to menu")
            }
        }
    }
}