#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include <QVector>
#include <QTimer>

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

    Q_INVOKABLE void loadBullets();
    Q_INVOKABLE void shoot(bool self);
    Q_INVOKABLE void switchTurn();
    Q_INVOKABLE void resetGame(); // Новый метод для сброса игры

signals:
    void playerHealthChanged();
    void enemyHealthChanged();
    void playerTurnChanged();
    void bulletsChanged();
    void showResult(const QString &text, const QString &color);
    void playerDamaged();
    void enemyDamaged();
    void miss();
    void enemyAction(const QString &action);

private slots:
    void processEnemyTurn();

private:
    int m_playerHealth = 3;
    int m_enemyHealth = 3;
    bool m_playerTurn = true;
    QVector<bool> m_bullets;
    int m_currentChamber = 0;
    QTimer* m_enemyTimer;

    int calculateEnemyDecision();
};

#endif // GAMELOGIC_H