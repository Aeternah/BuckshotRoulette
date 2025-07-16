#include "GameLogic.h"
#include <QRandomGenerator>
#include <QDebug>
#include <algorithm>
#include <QColor>

GameLogic::GameLogic(QObject *parent)
    : QObject(parent),
      m_enemyTimer(new QTimer(this)),
      m_enemyActionTimer(new QTimer(this)),
      m_bulletsInitialized(false)
{
    connect(m_enemyTimer, &QTimer::timeout, this, &GameLogic::processEnemyTurn);
    connect(m_enemyActionTimer, &QTimer::timeout, [this]() {
        m_enemyActionTimer->stop(); // на всякий случай
        shoot(m_enemyPlannedShootSelf); // Стреляем через ~паузы~
        m_enemyActing = false; // теперь враг свободен
    });

    m_playerTurn = true;
}

int GameLogic::playerHealth() const { return m_playerHealth; }
int GameLogic::enemyHealth() const { return m_enemyHealth; }
bool GameLogic::playerTurn() const { return m_playerTurn; }
QVector<bool> GameLogic::bullets() const { return m_bullets; }

void GameLogic::loadBullets()
{
    if (m_bulletsInitialized && !m_bullets.isEmpty()) return;

    m_bullets.clear();
    int shellCount = QRandomGenerator::global()->bounded(2, 7);
    int liveCount = shellCount / 2 + (QRandomGenerator::global()->bounded(2) == 0 ? 1 : 0);

    for (int i = 0; i < liveCount; ++i) m_bullets.append(true);
    for (int i = 0; i < shellCount - liveCount; ++i) m_bullets.append(false);

    std::random_shuffle(m_bullets.begin(), m_bullets.end());
    m_currentChamber = 0;
    m_bulletsInitialized = true;

    qDebug() << "Bullets loaded:" << m_bullets;
    emit showBulletsPreview();
    emit bulletsChanged();
}

void GameLogic::shoot(bool self)
{
    if (m_bullets.isEmpty()) {
        qDebug() << "No bullets left, forcing reload";
        m_bulletsInitialized = false;
        loadBullets();
        return;
    }

    bool isLive = m_bullets.takeFirst();

    if (isLive) {
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

    if (m_playerHealth <= 0) {
        emit showResult("Вы проиграли!", QColor("red"));
    } else if (m_enemyHealth <= 0) {
        emit showResult("Вы победили!", QColor("green"));
    } else {
        switchTurn();
    }

    emit bulletsChanged();
}

void GameLogic::switchTurn()
{
    m_playerTurn = !m_playerTurn;
    emit playerTurnChanged();

    if (!m_playerTurn && !m_bullets.isEmpty()) {
        if (!m_enemyActing) {
            m_enemyActing = true;
            m_enemyTimer->start(1500); // враг "думает"
        }
    }
}


void GameLogic::processEnemyTurn()
{
    m_enemyTimer->stop();

    if (m_bullets.isEmpty()) {
        qDebug() << "No bullets for enemy turn, reloading";
        m_bulletsInitialized = false;
        m_enemyActing = false;
        loadBullets();
        return;
    }

    if (m_enemyActing) {
        m_enemyPlannedShootSelf = calculateEnemyDecision();
        QString action = m_enemyPlannedShootSelf ? "Стреляет в себя" : "Стреляет в тебя";
        emit enemyAction(action);
        m_enemyActionTimer->start(800); // подождём немного
    }
}


int GameLogic::calculateEnemyDecision()
{
    int liveBullets = 0;
    for (bool b : m_bullets) {
        if (b) liveBullets++;
    }

    if (m_enemyHealth == 1) return false;
    if (liveBullets == 1) return false;
    if (liveBullets > m_bullets.size() / 2)
        return QRandomGenerator::global()->bounded(100) < 70;

    return QRandomGenerator::global()->bounded(2) == 0;
}

void GameLogic::resetGame()
{
    m_playerHealth = 3;
    m_enemyHealth = 3;
    m_playerTurn = true;
    m_bullets.clear();
    m_currentChamber = 0;
    m_enemyTimer->stop();
    m_enemyActionTimer->stop();
    m_bulletsInitialized = false;

    qDebug() << "Game reset";
    emit playerHealthChanged();
    emit enemyHealthChanged();
    emit playerTurnChanged();
    emit bulletsChanged();
}

void GameLogic::startRound()
{
    m_playerTurn = true;
    emit playerTurnChanged();
    emit bulletsChanged();
}
