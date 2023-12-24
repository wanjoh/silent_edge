#pragma once

#include <QObject>
#include <QTimer>
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
    static constexpr quint32 BULLET_SPEED = 10;
    static constexpr quint32 AMMO_RESPAWN_TIME = 20000;
    static constexpr quint32 TARGET_FPS = 30;
    static constexpr qreal EPSILON = 1e-5;

    GameLogicHandler(Map* = new Map(), QObject* = nullptr);
    GameLogicHandler(const GameLogicHandler&) = delete;
    ~GameLogicHandler();
    void updateBullets();
    qreal updatePlayerRotation(int x, int y, const QString& name, Player *player);
    void updatePlayerPosition(int x, int y, const QString& name, Player *player);
    void updatePlayerStats(const QByteArray&);
    void addBullet(int x, int y, const QString& name, qreal rotation);
    void putPlayersIntoRooms();
    void updatePlayers();
    QByteArray jsonify(const QString&);
private slots:
    void updateAmmo();
signals:
    void tileChanged(int, const QString& path);
    void updatePlayersSignal(QByteArray& player_info);
    void updateBulletsSignal(QByteArray& bullet_info);
    // videti kako ovo da se iskoristi, nesto mora da se prosledi kao param
    void playerDestroyedSignal();
private:
    void addPlayer(Player*);
    void removePlayer(QString);
    /*Returns true on bullet collision*/
    bool checkCollisions(Bullet*);
    void decreaseHp(Player*, Bullet*);
    void initializeTimers();
    bool canEntityMove(QVector<int> ids);

    QTimer movement_timer_;
    QTimer ammo_respawn_timer_;
    QTimer shooting_timer_;
    std::map<QString, Player*> players_;
    QList<QPair<QString, Bullet*>> bullets_;
    Map* map_;
    std::unordered_map<qint32, Room*> rooms_;
    std::unordered_map<qint32, Tile*> matrix_;
    QMutex mutex_;
    std::map<QString, quint32> commands_;
    std::map<QString, QPair<qreal, qreal>> mouse_positions_;
};
