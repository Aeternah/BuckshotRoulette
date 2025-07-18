#include "GameLogic.h"
#include <QRandomGenerator>
#include <QDebug>
#include <algorithm>
#include <random>
#include <QColor>

GameLogic::GameLogic(QObject *parent)
    : QObject(parent),
      m_enemyTimer(new QTimer(this)),
      m_enemyActionTimer(new QTimer(this)),
      m_bulletsInitialized(false)
{
    connect(m_enemyActionTimer, &QTimer::timeout, [this]() {
        m_enemyActionTimer->stop();
        shoot(m_enemyPlannedShootSelf);
    });

    m_playerTurn = true;
}

int GameLogic::playerHealth() const { return m_playerHealth; }
int GameLogic::enemyHealth() const { return m_enemyHealth; }
bool GameLogic::playerTurn() const { return m_playerTurn; }
QVector<bool> GameLogic::bullets() const { return m_bullets; }

void GameLogic::initializeBullets()
{
    if (m_bulletsInitialized && !m_bullets.isEmpty()) return;

    m_bullets.clear();
    int shellCount = QRandomGenerator::global()->bounded(2, 7);
    int liveCount = shellCount / 2 + (QRandomGenerator::global()->bounded(2) == 0 ? 1 : 0);

    for (int i = 0; i < liveCount; ++i) m_bullets.append(true);
    for (int i = 0; i < shellCount - liveCount; ++i) m_bullets.append(false);

    m_currentChamber = 0;
    m_bulletsInitialized = true;

    qDebug() << "Bullets initialized (not shuffled):" << m_bullets;
    emit showBulletsPreview();
    emit bulletsChanged();
}

void GameLogic::shuffleBullets()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(m_bullets.begin(), m_bullets.end(), g);
    qDebug() << "Bullets shuffled:" << m_bullets;
    emit bulletsChanged();
}

void GameLogic::shoot(bool self)
{
    if (m_bullets.isEmpty()) {
        qDebug() << "No bullets left, forcing reload";
        m_bulletsInitialized = false;
        m_enemyActing = false;
        initializeBullets();
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
        m_enemyActing = false;
        return;
    } else if (m_enemyHealth <= 0) {
        emit showResult("Вы победили!", QColor("green"));
        m_enemyActing = false;
        return;
    }

    // Логика смены хода
    if (m_playerTurn) {
        // Игрок стреляет
        if (!self || isLive) {
            // Выстрел в противника или в себя боевым патроном -> ход боту
            m_enemyActing = false; // Сбрасываем перед сменой хода
            switchTurn();
        }
        // Выстрел в себя холостым -> ход остаётся у игрока
    } else {
        // Бот стреляет
        if (self || isLive) {
            // Выстрел в игрока или в себя боевым патроном -> ход игроку
            m_enemyActing = false; // Сбрасываем перед сменой хода
            switchTurn();
        } else {
            // Выстрел в себя холостым -> ход остаётся у бота
            m_enemyActing = false; // Сбрасываем, чтобы начать новый ход
            if (!m_bullets.isEmpty()) {
                m_enemyActing = true;
                processEnemyTurn(); // Запускаем новый ход бота
            } else {
                // Если патроны закончились, начинаем новый раунд
                m_bulletsInitialized = false;
                initializeBullets();
            }
        }
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
            processEnemyTurn();
        }
    }
}

void GameLogic::processEnemyTurn()
{
    if (m_bullets.isEmpty()) {
        qDebug() << "No bullets for enemy turn, reloading";
        m_bulletsInitialized = false;
        m_enemyActing = false;
        initializeBullets();
        return;
    }

    if (m_enemyActing) {
        m_enemyPlannedShootSelf = calculateEnemyDecision();
        QString action = m_enemyPlannedShootSelf ? "Стреляет в тебя" : "Стреляет в себя";
        emit enemyAction(action);
        m_enemyActionTimer->start(3000); // Задержка 3 секунды
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
    m_enemyActing = false;

    qDebug() << "Game reset";
    emit playerHealthChanged();
    emit enemyHealthChanged();
    emit playerTurnChanged();
    emit bulletsChanged();
}

void GameLogic::startRound()
{
    m_playerTurn = true;
    m_enemyActing = false;
    emit playerTurnChanged();
    emit bulletsChanged();
}