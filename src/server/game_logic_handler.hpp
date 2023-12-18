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
    qreal updatePlayerRotation(int x, int y, const QString& name, Player *player);
    void updatePlayerPosition(int x, int y, const QString& name, Player *player);
    void updatePlayerStats(QByteArray &);
    void addBullet(const QString& name, qreal rotation);
    void put_players_into_rooms();
    void updatePlayers();
    void clear_commands();
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
    bool canEntityMove(QVector<int> ids);
    qreal dot_product(const QPair<qreal, qreal>& v1, const QPair<qreal, qreal>& v2);
    qreal mag(const QPair<qreal, qreal>& v);

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
    std::map<QString, int> commands_;
    std::map<QString, QPair<qreal, qreal>> mouse_positions_;
};
