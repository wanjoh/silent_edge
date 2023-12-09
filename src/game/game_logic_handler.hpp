#pragma once

#include <QObject>
#include <QTimer>
#include "../entity/player.hpp"
#include "../entity/bullet.hpp"

class GameLogicHandler : public QObject
{
    Q_OBJECT
public:
    static constexpr qreal DEFAULT_PLAYER_VELOCITY = 5.0f;
    static constexpr quint32 TARGET_FPS = 60;
    static constexpr qreal EPSILON = 1e-5;

    GameLogicHandler(QString = "playa", QObject* = nullptr);
    GameLogicHandler(const GameLogicHandler&) = delete;
    ~GameLogicHandler();

    Player* getPlayer();
public slots:
    void resetKeys();
    void updateKeys(quint32, bool);
    void addBullet(QString, Bullet*);
    void updateAimingPoint(QPointF);
private slots:
    void updateMovement();
    void updateBullets();
signals:
    void playerMoved(QVariant);
    // todo: connect sa klijentom
    void playerShot();
    void newBulletSignal(QString, EntityDrawer*);
    void destroyBullet(QString);
private:
    void initializeTimers();
    bool updateRotation();

    QTimer movement_timer_;
    Player* player_;
    QPointF aiming_point_;
    // todo: izbaciti enemies iz game klase
    std::map<QString, Player*> enemies_;
    std::map<QString, std::vector<Bullet*>> bullets_;
    std::map<QString, quint32> player_bullet_count_;
    // ovo ce mozda moci i da bude lista od 10ak bulova jer nemamo mnogo dugmica
    std::unordered_map<quint32, bool> keys_;
};

