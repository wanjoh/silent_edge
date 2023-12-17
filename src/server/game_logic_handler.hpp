#pragma once

#include <QObject>
#include <QTimer>
#include <QVariantMap>
#include "../entity/player.hpp"
#include "../entity/bullet.hpp"
#include "../map/map.hpp"
#include <QMutex>

class GameLogicHandler : public QObject
{
    Q_OBJECT
public:
    static constexpr qreal DEFAULT_PLAYER_VELOCITY = 5.0f;
    static constexpr quint32 BULLETS_PER_SECOND = 3;
    static constexpr quint32 AMMO_RESPAWN_TIME = 20000;
    static constexpr quint32 TARGET_FPS = 30;
    static constexpr qreal EPSILON = 1e-5;

    GameLogicHandler(Map* = new Map(), QObject* = nullptr);
    GameLogicHandler(const GameLogicHandler&) = delete;
    ~GameLogicHandler();
    void updateBullets();
    void updateMovement();
    void updatePlayers();
    void updateRotation();

    void put_players_into_rooms();

public slots:
    void addBullet(QString, Bullet*);
private slots:
    void updateAmmo();
signals:
    // todo: dodati slanje specijalnih tile-ova (npr municija)
    // limun: rešio sam nešto što ovo rešava
    void tile_changed(int, const QString& path);
private:
    void destroyBullet(Bullet*);
    void checkCollisions(Bullet*);
    void decreaseHp(Player*, Bullet*);
    void initializeTimers();
    void updatePlayer(QByteArray);
    bool canEntityMove(QVector<int> ids);

    QTimer movement_timer_;
    QTimer ammo_respawn_timer_;
    QTimer shooting_timer_;
    std::map<QString, Player*> players_;
    // todo: naci pogodniju strukturu podataka, trenutno je par [vlasnik metka, metak]
    // limun: što je to loše, svejedno se server stara o svim mecima
    QVector<QPair<QString, Bullet*>> bullets_;
    Map* map_;
    std::unordered_map<int, Room*> rooms_;
    std::unordered_map<int, Tile*> matrix_;
    QMutex mutex_;
    std::map<QString, int> commands;
};
