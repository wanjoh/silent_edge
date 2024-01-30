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

    qreal getHp() const;
    qreal getEnergy();
    qreal getDpp();
    qreal getScore() const;
    MeleeWeapon* getMeleeWeapon() const;
    RangedWeapon* getRangedWeapon() const;
    Entity* getReload() const;
    QTimer* getReloadTimer() const;
    QTimer* getSwingTimer() const;

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
