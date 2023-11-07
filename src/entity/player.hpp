#pragma once

#include "entity.hpp"
#include "weapon.hpp"

class Player : public Entity
{
public:
    qreal getHp();
    qreal getEnergy();
    qreal getDpp();
    qreal getScore();
    void move(std::pair<qint32, qint32>);
private:
    qreal hp_;
    qreal energy_;
    qreal dpp_;
    qreal score_;
    std::list<Weapon> weapon_list_;
};
