#pragma once

#include <QObject>
#include <QTimer>
#include "../entity/player.hpp"
#include "../entity/bullet.hpp"
#include "../map/map.hpp"
#include <QMutex>
#include <QFile>
#include <QDir>

#include "../gui/results.hpp"

enum class LogicEvents : quint32
{
    CAMERA = 1,
    RESTOCK = 1 << 1,
    TILECHANGE = 1 << 2
};
inline quint32 operator&(quint32 a, LogicEvents b)
{
    return a & static_cast<quint32>(b);
}
inline quint32 operator|(quint32 a, LogicEvents b)
{
    return a | static_cast<quint32>(b);
}
class GameLogicHandler : public QObject
{
    Q_OBJECT
public:
    static constexpr qreal DEFAULT_PLAYER_VELOCITY = 5.0F;
    static constexpr quint32 BULLETS_PER_SECOND = 3;
    static constexpr quint32 BULLET_SPEED = 10;
    static constexpr quint32 AMMO_RESPAWN_TIME = 20000;
    static constexpr quint32 TARGET_FPS = 30;
    static constexpr qreal EPSILON = 1e-5;
    static constexpr qint32 AMMO_BUCKET_CAPACITY = 10;

    GameLogicHandler(Map* = new Map(), QObject* = nullptr);
    GameLogicHandler(const GameLogicHandler&) = delete;
    ~GameLogicHandler();
    void updatePlayerRotation(const QString& name);
    void updatePlayerPosition(const QString& name);
    void updatePlayerStats(const QByteArray&);
    void addBullet(const QString& name);
    void putPlayerIntoRoom(const QString &name);
    void removePlayerFromRoom(const QString &name);
    void updateEntities();
    void updatePlayers();
    void updateBullets();
    QByteArray jsonify_players();
    QByteArray jsonify_bullets();
    QByteArray jsonify_tile(int, const QString &);
private slots:
    void updateAmmo();
    void reload(const QString &);
    void swing(const QString &);
signals:
    void updatePlayersSignal(QByteArray&);
    void updateBulletsSignal(QByteArray&);
    void gameIsOver();

private:
    void addPlayer(Player*);
    void removePlayer(const QString &);
    /*Returns true on bullet collision*/
    bool checkBulletCollisions(Bullet *bullet);
    bool checkPlayerCollision(qreal x, qreal y,const QString &name);
    static void decreaseHp(Player *, Bullet *);
    void initializeTimers();
    bool canEntityMove(QVector<int> ids);
    void updateScores(); //
    void rangList();    //

    QTimer movement_timer_;
    QTimer ammo_respawn_timer_;
    QTimer shooting_cooldown_timer_;
    QTimer reload_timer_;
    QTimer swing_timer_;
    std::map<QString, Player*> players_;
    std::map<int, Bullet*> bullets_;
    std::map<int, bool> bullet_moved_;
    Map* map_;
    std::unordered_map<qint32, Room*> rooms_;
    std::unordered_map<qint32, Tile*> matrix_;
    QMutex mutex_;
    QMutex reload_mutex_;
    std::map<QString, quint32> commands_;
    std::map<QString, quint32> logic_events_;
    std::map<QString, bool> shooting_in_progress_;
    std::map<QString, quint32> player_bullet_count_;
    std::map<QString, bool> melee_in_progress_;
    std::map<QString, bool> reloading_in_progress_;
    std::map<QString, QPair<int, int>> positions_;
    std::map<QString, QPair<qreal, qreal>> mouse_positions_;
    std::map<QString, int> tile_id_;
    Results *results_;
};
