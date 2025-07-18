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
    NetworkLogic { id: networkLogic }

    Loader {
        anchors.fill: parent
        source: currentScreen === "menu" ? "Menu.qml" : 
                currentScreen === "networkMenu" ? "NetworkMenu.qml" : 
                currentScreen === "serverScreen" ? "ServerScreen.qml" : 
                currentScreen === "joinServerScreen" ? "JoinServerScreen.qml" : 
                "Game.qml"
        onStatusChanged: {
            if (status === Loader.Ready) {
                console.log("Loader loaded:", currentScreen)
            } else if (status === Loader.Error) {
                console.log("Loader error for:", currentScreen, "- Source:", source)
                resultDialog.text = "Ошибка загрузки: " + currentScreen
                resultDialog.textColor = "red"
                resultDialog.open()
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
                gameLogic.resetGame()
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

    Connections {
        target: networkLogic
        function onServerStarted(ip, port) {
            resultDialog.text = "Сервер запущен: " + ip + ":" + port
            resultDialog.textColor = "green"
            resultDialog.open()
        }
        function onServerError(error) {
            resultDialog.text = "Ошибка сервера: " + error
            resultDialog.textColor = "red"
            resultDialog.open()
            mainWindow.currentScreen = "networkMenu"
        }
        function onClientConnected() {
            resultDialog.text = "Подключено к серверу!"
            resultDialog.textColor = "green"
            resultDialog.open()
            mainWindow.currentScreen = "game"
        }
        function onClientError(error) {
            resultDialog.text = "Ошибка подключения: " + error
            resultDialog.textColor = "red"
            resultDialog.open()
            mainWindow.currentScreen = "networkMenu"
        }
        function onMessageReceived(message) {
            console.log("Message from server:", message)
        }
    }
}