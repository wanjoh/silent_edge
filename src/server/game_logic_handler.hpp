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
    static constexpr qint32 AMMO_BUCKET_CAPACITY = 10;

    GameLogicHandler(Map* = new Map(), QObject* = nullptr);
    GameLogicHandler(const GameLogicHandler&) = delete;
    ~GameLogicHandler();
    void updatePlayerRotation(int x, int y, const QString& name);
    void updatePlayerPosition(int x, int y, const QString& name);
    void updatePlayerStats(const QByteArray&);
    void addBullet(int x, int y, const QString& name);
    void putPlayerIntoRoom(const QString &name);
    void removePlayerFromRoom(const QString &name);
    void updateAll();
    void updatePlayers();
    void updateBullets();
    QByteArray jsonify(const QString&);
    QByteArray jsonify_tile(int, const QString &);
private slots:
    void updateAmmo();
    void reload(const QString &);
    void swing(const QString &);
signals:
    void tileChangedSignal(QByteArray&);
    void updateAllSignal(QByteArray& player_info, QByteArray& bullet_info);
    // videti kako ovo da se iskoristi, nesto mora da se prosledi kao param
    void playerDestroyedSignal();
    void bulletDestroyedSignal(QString);
    void restockAmmoPilesSignal();
    void sendRefreshCameraSignal(QByteArray&);
    void reloadItemSignal(QString, EntityDrawer*);
    void meleeSwingSignal(QString, EntityDrawer*);
    void removeReload(QString);
    void removeMelee(QString);
    void labelSignal(qint32, qint32, qint32);

private:
    void addPlayer(Player*);
    void removePlayer(QString);
    /*Returns true on bullet collision*/
    bool checkBulletCollisions(Bullet *bullet);
    bool checkPlayerCollision(qreal x, qreal y,const QString &name);
    void decreaseHp(Player*, Bullet*);
    void initializeTimers();
    bool canEntityMove(QVector<int> ids);

    QTimer movement_timer_;
    QTimer ammo_respawn_timer_;
    QTimer shooting_cooldown_timer_;
    QTimer reload_timer_;
    QTimer swing_timer_;
    std::map<QString, Player*> players_;
    // limun: mapa čiji je ključ ime igrača, a sadrži listu metaka
    std::map<int, Bullet*> bullets_;
    Map* map_;
    std::unordered_map<qint32, Room*> rooms_;
    std::unordered_map<qint32, Tile*> matrix_;
    QMutex mutex_;
    QMutex reload_mutex_;
    std::map<QString, quint32> commands_;
    std::map<QString, bool> shooting_in_progress_;
    std::map<QString, quint32> player_bullet_count_;
    std::map<QString, bool> melee_in_progress_;
    std::map<QString, QPair<int, int>> positions_;
    std::map<QString, QPair<qreal, qreal>> mouse_positions_;
    EntityDrawer* reload_drawer_;
};
