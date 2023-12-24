#pragma once

#include "../gui/player_drawer.hpp"
#include "entity.hpp"
#include "weapon.hpp"
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

    void setHp(qreal);

    void addWeapon(Weapon*);
    Weapon* currentWeapon();
    Weapon* nextWeapon();
    Weapon* previousWeapon();


private:
    qreal hp_;
    qreal energy_;
    qreal dpp_;
    qreal score_;

    std::vector<Weapon*> weapon_list_;
    qint32 weapon_index_;
};
