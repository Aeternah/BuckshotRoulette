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
            if (status === Image.Ready) console.log("Menu background loaded")
            else if (status === Image.Error) console.log("Menu background error:", source)
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 30

        Text {
            text: "РУССКАЯ РУЛЕТКА С ДРОБОВИКОМ"
            font.pixelSize: 64
            color: "white"
            style: Text.Outline
            styleColor: "black"
        }

        Button {
            text: "НАЧАТЬ ИГРУ"
            width: 300
            height: 80
            font.pixelSize: 24
            onClicked: {
                gameLogic.loadBullets()
                mainWindow.currentScreen = "game"
                console.log("Switching to game screen")
            }
        }

        Button {
            text: "ВЫХОД"
            width: 300
            height: 80
            font.pixelSize: 24
            onClicked: Qt.quit()
        }
    }
}