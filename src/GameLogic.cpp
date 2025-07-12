#include "GameLogic.h"
#include <QRandomGenerator>
#include <QDebug>

GameLogic::GameLogic(QObject *parent) : QObject(parent),
    m_enemyTimer(new QTimer(this))
{
    connect(m_enemyTimer, &QTimer::timeout, this, &GameLogic::processEnemyTurn);
    m_playerTurn = true; // Начинаем всегда с игрока
}

int GameLogic::playerHealth() const { return m_playerHealth; }
int GameLogic::enemyHealth() const { return m_enemyHealth; }
bool GameLogic::playerTurn() const { return m_playerTurn; }
QVector<bool> GameLogic::bullets() const { return m_bullets; }

void GameLogic::loadBullets()
{
    m_bullets.clear();
    int shellCount = QRandomGenerator::global()->bounded(2, 9); // 2-8 пуль
    for (int i = 0; i < shellCount; ++i) {
        m_bullets.append(QRandomGenerator::global()->bounded(2) == 0); // 50/50 live/blank
    }
    m_playerTurn = true; // Устанавливаем ход игрока
    m_currentChamber = 0;
    qDebug() << "Bullets loaded:" << m_bullets.size();
    emit bulletsChanged();
}

void GameLogic::shoot(bool self)
{
    if (m_bullets.isEmpty()) {
        emit showResult("Патроны закончились!", "red");
        return;
    }

    bool bang = m_bullets[m_currentChamber];
    if (bang) {
        if (self) {
            m_playerHealth--;
            emit playerHealthChanged();
            emit playerDamaged();
        } else {
            m_enemyHealth--;
            emit enemyHealthChanged();
            emit enemyDamaged();
        }
    } else {
        emit miss();
    }

    if (!bang && self) {
        // Холостой выстрел в себя — игрок остается на ходу
    } else {
        switchTurn();
    }

    m_currentChamber = (m_currentChamber + 1) % m_bullets.size();
    if (m_currentChamber == 0) {
        m_bullets.clear();
        emit bulletsChanged();
    }

    if (m_playerHealth <= 0) {
        emit showResult("Вы проиграли!", "red");
    } else if (m_enemyHealth <= 0) {
        emit showResult("Вы победили!", "green");
    }
}

void GameLogic::switchTurn()
{
    m_playerTurn = !m_playerTurn;
    emit playerTurnChanged();
    if (!m_playerTurn) {
        m_enemyTimer->start(5000); // 5 секунд для хода врага
    }
}

void GameLogic::processEnemyTurn()
{
    m_enemyTimer->stop();
    bool shootSelf = calculateEnemyDecision();
    QString action = shootSelf ? "Стреляет в себя" : "Стреляет в тебя";
    emit enemyAction(action);
}

int GameLogic::calculateEnemyDecision()
{
    if (m_enemyHealth == 1) {
        return false; // Стреляет в игрока, если мало HP
    }
    return QRandomGenerator::global()->bounded(2) == 0; // 50/50
}

void GameLogic::resetGame()
{
    m_playerHealth = 3;
    m_enemyHealth = 3;
    m_playerTurn = true;
    m_bullets.clear();
    m_currentChamber = 0;
    m_enemyTimer->stop();
    emit playerHealthChanged();
    emit enemyHealthChanged();
    emit playerTurnChanged();
    emit bulletsChanged();
    qDebug() << "Game reset: playerHealth=" << m_playerHealth << ", enemyHealth=" << m_enemyHealth;
}