#pragma once

#include "../gui/player_drawer.hpp"
#include "entity.hpp"
#include "melee_weapon.hpp"
#include "ranged_weapon.hpp"
#include<QMap>

class Player : public Entity
{
    Q_OBJECT
public:
    // ovo ce leteti odavde
    constexpr static qint32 VELOCITY = 3;
    Player();
    Player(QString, bool = true, qreal hp = 100, int x = 128, int y = 128, QObject* = nullptr);

    qreal getHp();
    qreal getEnergy();
    qreal getDpp();
    qreal getScore();
    MeleeWeapon* getMeleeWeapon() const;
    RangedWeapon* getRangedWeapon() const;

    void setHp(qreal);
    void setScore(qreal);

private:
    qreal hp_;
    qreal energy_;
    qreal dpp_;
    qreal score_;

    MeleeWeapon* meleeWeapon_;
    RangedWeapon* rangedWeapon_;
};
