#pragma once

#include "bullet.hpp"
#include "weapon.hpp"

class RangedWeapon : public Weapon
{
public:
    qreal getRateOfFire();
    std::list<Bullet> getBulletsUsed();
private:
    qreal rate_of_fire_;
    std::list<Bullet> bullets_used_;
};
