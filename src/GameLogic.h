#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include <QTimer>
#include <QVector>

class GameLogic : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int playerHealth READ playerHealth NOTIFY playerHealthChanged)
    Q_PROPERTY(int enemyHealth READ enemyHealth NOTIFY enemyHealthChanged)
    Q_PROPERTY(bool playerTurn READ playerTurn NOTIFY playerTurnChanged)
    Q_PROPERTY(QVector<bool> bullets READ bullets NOTIFY bulletsChanged)

public:
    explicit GameLogic(QObject *parent = nullptr);

    int playerHealth() const;
    int enemyHealth() const;
    bool playerTurn() const;
    QVector<bool> bullets() const;

public slots:
    void initializeBullets();
    void shuffleBullets();
    void shoot(bool self);
    void switchTurn();
    void processEnemyTurn();
    void resetGame();
    void startRound();

signals:
    void playerHealthChanged();
    void enemyHealthChanged();
    void playerTurnChanged();
    void bulletsChanged();
    void playerDamaged();
    void enemyDamaged();
    void miss();
    void showResult(const QString &text, const QColor &color);
    void enemyAction(const QString &action);
    void showBulletsPreview();

private:
    int m_playerHealth = 3;
    int m_enemyHealth = 3;
    bool m_playerTurn;
    QVector<bool> m_bullets;
    int m_currentChamber = 0;
    QTimer *m_enemyTimer;
    QTimer *m_enemyActionTimer;
    bool m_enemyPlannedShootSelf;
    bool m_enemyActing = false;
    int calculateEnemyDecision();
    bool m_bulletsInitialized = false;
};

#endif // GAMELOGIC_H