#pragma once

#include <QObject>
#include <QTimer>
#include "../entity/player.hpp"
#include "../entity/bullet.hpp"
#include "../gui/entity_drawer.hpp"
#include "../map/map.hpp"
#include <QMutex>


class GameLogicHandler : public QObject
{
    Q_OBJECT
public:
    static constexpr qreal DEFAULT_PLAYER_VELOCITY = 5.0f;
    static constexpr quint32 TARGET_FPS = 60;
    static constexpr qreal EPSILON = 1e-5;

    GameLogicHandler(QString = "playa", Map* = new Map(), QObject* = nullptr);
    GameLogicHandler(const GameLogicHandler&) = delete;
    ~GameLogicHandler();

    Player* getPlayer();
public slots:
    void resetKeys();
    void updateKeys(quint32, bool);
    void updateMouseClick(Qt::MouseButton, bool);
    void updateMouseScroll(qint32);
    void addBullet(QString, Bullet*);
    void updateAimingPoint(QPointF);
private slots:
    void updateMovement();
    void updateAmmo();
    void updateBullets();
    void checkCollisions(Bullet*);
signals:
    void playerMoved(QVariant);
    void tileDeleted(QString);
    // todo: connect sa klijentom
    void playerShot();
    void newBulletSignal(QString, EntityDrawer*);
    void weaponDrawSignal(QString, EntityDrawer*);
    void destroyBullet(QString);
    void destroyPlayer(QString);
    void bulletUpdating(Bullet*);
private:
    void decreaseHp(Player*,Bullet*);
    void initializeTimers();
    bool updateRotation();
    bool canPlayerMove(QVector<QString> names);

    QTimer movement_timer_;
    QTimer ammo_respawn_timer_;
    QTimer shooting_timer_;
    Player* player_;
    Weapon* current_weapon_;
    QPointF aiming_point_;
    // todo: izbaciti enemies iz game klase
    std::map<QString, Player*> enemies_;
    std::map<QString, std::vector<Bullet*>> bullets_;
    std::map<QString, quint32> player_bullet_count_;
    // ovo ce mozda moci i da bude lista od 10ak bulova jer nemamo mnogo dugmica
    std::unordered_map<quint32, bool> keys_;

    Map* map_object_;
    std::unordered_map<QString, Tile*> map_;
    QMutex mutex_;
};
