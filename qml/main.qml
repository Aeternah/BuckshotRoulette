import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Game 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1280
    height: 720
    minimumWidth: 800
    minimumHeight: 600
    title: "Игра с дробовиком"
    color: "#121212"

    property string currentScreen: "menu"

    GameLogic { id: gameLogic }

    Loader {
        anchors.fill: parent
        source: currentScreen === "menu" ? "qrc:/qml/Menu.qml" : "qrc:/qml/Game.qml"
        onStatusChanged: {
            if (status === Loader.Ready) {
                console.log("Loader loaded:", currentScreen)
                if (currentScreen === "game") {
                    gameLogic.loadBullets() // Инициализируем патроны при загрузке игры
                }
            } else if (status === Loader.Error) {
                console.log("Loader error for:", currentScreen)
            }
        }
        onLoaded: console.log("Loaded item:", item)
    }

    Dialog {
        id: resultDialog
        modal: true
        anchors.centerIn: parent
        width: 300
        height: 150

        property string text: ""
        property color textColor: "white"

        background: Rectangle {
            color: "#333"
            radius: 10
        }

        Label {
            anchors.centerIn: parent
            text: resultDialog.text
            color: resultDialog.textColor
            font.pixelSize: 24
        }

        onClosed: {
            if (gameLogic.playerHealth <= 0 || gameLogic.enemyHealth <= 0) {
                mainWindow.currentScreen = "menu"
                gameLogic.loadBullets() // Перезагрузка при проигрыше
            }
        }
    }

    Connections {
        target: gameLogic
        function onShowResult(text, color) {
            resultDialog.text = text
            resultDialog.textColor = color
            resultDialog.open()
        }
    }
}