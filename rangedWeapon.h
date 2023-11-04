#pragma once

#include "bullet.h"
#include "weapon.h"

class RangedWeapon : public Weapon
{
public:
    float get_rate_of_fire();
    std::list<Bullet> get_bullets_used();
private:
    float rate_of_fire;
    std::list<Bullet> bullets_used;
};
