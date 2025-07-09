import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia

Rectangle {
    id: root
    width: 1280
    height: 720
    color: "#111"

    Image {
        id: background
        anchors.fill: parent
        source: "qrc:/images/game_bg.png"
        fillMode: Image.PreserveAspectCrop
        opacity: 0.7
        z: -1
        onStatusChanged: {
            if (status === Image.Ready) console.log("Game background loaded")
            else if (status === Image.Error) console.log("Game background error")
        }
    }

    Rectangle {
        id: muzzleFlash
        anchors.fill: parent
        color: "white"
        opacity: 0.0
        visible: false

        SequentialAnimation {
            id: flashAnim
            running: false
            onStarted: console.log("Muzzle flash started")
            PropertyAnimation { target: muzzleFlash; property: "opacity"; to: 0.7; duration: 50 }
            PauseAnimation { duration: 50 }
            PropertyAnimation { target: muzzleFlash; property: "opacity"; to: 0.0; duration: 200 }
            ScriptAction { script: { muzzleFlash.visible = false; console.log("Muzzle flash finished") } }
        }
    }

    Image {
        id: player
        source: "qrc:/images/player.jpg"
        width: 200
        height: 200
        anchors.left: parent.left
        anchors.leftMargin: 100
        anchors.verticalCenter: parent.verticalCenter
        onStatusChanged: {
            if (status === Image.Ready) console.log("Player image loaded")
            else if (status === Image.Error) console.log("Player image error")
        }

        SequentialAnimation on y {
            id: playerHitAnim
            loops: 2
            running: false
            PropertyAnimation { from: y; to: y - 10; duration: 80 }
            PropertyAnimation { from: y - 10; to: y; duration: 80 }
        }
    }

    Image {
        id: enemy
        source: "qrc:/images/enemy.png"
        width: 200
        height: 200
        anchors.right: parent.right
        anchors.rightMargin: 100
        anchors.verticalCenter: parent.verticalCenter

        SequentialAnimation on y {
            id: enemyHitAnim
            loops: 2
            running: false
            PropertyAnimation { from: y; to: y - 10; duration: 80 }
            PropertyAnimation { from: y - 10; to: y; duration: 80 }
        }
    }

    Image {
        id: shotgun
        source: "qrc:/images/shotgun.png"
        width: 300
        height: 300
        anchors.centerIn: parent
    }

    Column {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 16

        Button {
            id: shootButton
            text: "Выстрелить"
            width: 240
            height: 60
            enabled: gameLogic.bullets.length > 0 && gameLogic.playerTurn
            font.pixelSize: 20
            onClicked: {
                prepareForShotSound.play()
                flashAnim.restart()
                muzzleFlash.visible = true
                gameLogic.shoot(false) // Стреляем во врага
            }
        }

        Button {
            id: selfShootButton
            text: "Выстрел в себя"
            width: 240
            height: 60
            enabled: gameLogic.bullets.length > 0 && gameLogic.playerTurn
            font.pixelSize: 20
            onClicked: {
                prepareForShotSound.play()
                flashAnim.restart()
                muzzleFlash.visible = true
                gameLogic.shoot(true) // Стреляем в себя
            }
        }
    }

    Row {
        id: playerHearts
        spacing: 10
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 30

        Repeater {
            model: gameLogic.playerHealth
            Image {
                source: "qrc:/images/heart1.png"
                width: 40
                height: 40
            }
        }
    }

    Row {
        id: enemyHearts
        spacing: 10
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 30

        Repeater {
            model: gameLogic.enemyHealth
            Image {
                source: "qrc:/images/heart2.png"
                width: 40
                height: 40
            }
        }
    }

    Text {
        id: turnText
        text: gameLogic.playerTurn ? "Ваш ход" : "Ход врага: " + (enemyActionText.text || "Думает...")
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        color: "white"
        font.pixelSize: 28
    }

    Text {
        id: enemyActionText
        text: ""
        anchors.top: turnText.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
        color: "white"
        font.pixelSize: 20
        visible: !gameLogic.playerTurn
    }

    ProgressBar {
        id: enemyTurnProgress
        anchors.top: enemyActionText.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
        width: 200
        height: 20
        visible: !gameLogic.playerTurn
        value: 0.0
        from: 0.0
        to: 1.0
    }

    Dialog {
        id: resultDialog
        modal: true
        focus: true
        anchors.centerIn: parent
        width: 400
        height: 200
        visible: false

        property string text: "Игра окончена"
        property color textColor: "white"

        background: Rectangle {
            color: "#222"
            radius: 12
            border.color: "white"
            border.width: 2
        }

        contentItem: Column {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: resultDialog.text
                color: resultDialog.textColor
                font.pixelSize: 32
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                text: "В МЕНЮ"
                width: 160
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: mainWindow.currentScreen = "menu"
            }
        }
    }

    Dialog {
        id: preloadDialog
        modal: true
        focus: true
        anchors.centerIn: parent
        width: 600
        height: 250 // Увеличим высоту для текста
        visible: false

        background: Rectangle {
            color: "#222"
            radius: 12
            border.color: "white"
            border.width: 2
        }

        contentItem: Column {
            anchors.centerIn: parent
            spacing: 20

            Row {
                id: bulletRow
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 20

                Repeater {
                    model: gameLogic.bullets
                    Image {
                        source: modelData ? "qrc:/images/bullet1.png" : "qrc:/images/bullet2.png"
                        width: 80
                        height: 80
                    }
                }
            }

            Button {
                id: continueButton
                text: "Продолжить"
                width: 160
                anchors.horizontalCenter: parent.horizontalCenter // Центрируем кнопку
                onClicked: preloadDialog.close()
            }
        }

        onOpened: {
            prepareForShotSound.play()
            timer.start()
        }

        Timer {
            id: timer
            interval: 3000
            onTriggered: preloadDialog.close()
        }
    }

    SoundEffect {
        id: shotSound
        source: "qrc:/sounds/shot.wav"
    }

    SoundEffect {
        id: missSound
        source: "qrc:/sounds/miss.wav"
    }

    SoundEffect {
        id: prepareForShotSound
        source: "qrc:/sounds/prepare_for_shot.wav"
    }

    Connections {
        target: gameLogic
        function onPlayerDamaged() { playerHitAnim.restart(); shotSound.play() }
        function onEnemyDamaged() { enemyHitAnim.restart(); shotSound.play() }
        function onShowResult(text, color) {
            resultDialog.text = text
            resultDialog.textColor = color
            resultDialog.open()
        }
        function onMiss() { missSound.play(); gameLogic.switchTurn() }
        function onBulletsChanged() {
            if (gameLogic.bullets.length > 0) {
                preloadDialog.open() // Открываем при любом изменении с патронами
            } else {
                console.log("No bullets left, reloading...")
                gameLogic.loadBullets() // Автоматическая перезагрузка
            }
        }
        function onEnemyAction(action) {
            enemyActionText.text = action
            enemyTurnProgress.value = 0.0
            progressTimer.start()
            if (action === "Стреляет в себя") {
                gameLogic.shoot(true)
            } else if (action === "Стреляет в тебя") {
                gameLogic.shoot(false)
            }
        }
    }

    Timer {
        id: progressTimer
        interval: 50
        repeat: true
        running: false
        onTriggered: {
            enemyTurnProgress.value += 0.01
            if (enemyTurnProgress.value >= 1.0) {
                progressTimer.stop()
                gameLogic.switchTurn()
            }
        }
    }
}