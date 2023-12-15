#pragma once

#include <QObject>
#include <QTimer>
#include <QVariantMap>
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
    void addBullet(QString);
    void updateAimingPoint(QPointF);
    //    void bulletUpdate(QVariant);
    void handleEnemy(QVariant);
    void handleBullet(QVariant);
    void recognizeEntityType(QVariant);

private slots:
    void updateMovement();
    void updateAmmo();
    void updateBullets();
    void swingMelee();
    // privremeni fix: vraca true ako je metak unisten
    bool checkCollisions(Bullet*);
signals:
    void playerMoved(QVariant);
    void tileDeleted(int);
    // todo: connect sa klijentom
    void playerShot();
    void newBulletSignal(QString, EntityDrawer*);
    void destroyBullet(QString);
    void destroyPlayer(QString);
    void bulletMoved(QVariant);
    void update(QString, EntityDrawer*);
    void weapon_changed(int);
    void update_hp(qreal hp);
private:
    void decreaseHp(Player*, Bullet*);
    void initializeTimers();
    bool updateRotation();
    bool canEntityMove(QVector<int> &names);

    QTimer movement_timer_;
    QTimer ammo_respawn_timer_;
    QTimer shooting_timer_;
    Player* player_;
    QPointF aiming_point_;
    // todo: izbaciti enemies iz game klase
    std::map<QString, Player*> enemies_;
    std::map<QString, std::vector<Bullet*>> bullets_;
    std::map<QString, quint32> player_bullet_count_;
    // ovo ce mozda moci i da bude lista od 10ak bulova jer nemamo mnogo dugmica
    std::unordered_map<quint32, bool> keys_;

    Map* map_object_;
    std::unordered_map<int, Tile*> map_;
    QMutex mutex_;
};
