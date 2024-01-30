#pragma once

#include "../gui/player_drawer.hpp"
#include "entity.hpp"
#include "melee_weapon.hpp"
#include "ranged_weapon.hpp"
#include<QMap>
#include<QTimer>

class Player : public Entity
{
    Q_OBJECT
public:
    constexpr static qint32 VELOCITY = 3;
    Player();
    Player(const QString &, bool = true, qreal hp = 100, int x = 128, int y = 128, QObject* = nullptr);

    auto getHp() const -> qreal;
    auto getEnergy() -> qreal;
    auto getDpp() -> qreal;
    auto getScore() const -> qreal;
    auto getMeleeWeapon() const -> MeleeWeapon *;
    auto getRangedWeapon() const -> RangedWeapon *;
    auto getReload() const -> Entity *;
    auto getReloadTimer() const -> QTimer *;
    auto getSwingTimer() const -> QTimer *;

    void setHp(qreal);
    void setScore(qreal);

private:
    void initializeTimers();

    qreal hp_;
    qreal energy_;
    qreal dpp_;
    qreal score_;

    MeleeWeapon* melee_weapon_;
    RangedWeapon* ranged_weapon_;
    Entity* reload_;

    QTimer* reload_timer_;
    QTimer* swing_timer_;
};
