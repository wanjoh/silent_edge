#pragma once

#include "weapon.hpp"

class MeleeWeapon : public Weapon
{
public:
    qreal getCooldown();
private:
    qreal cooldown_;
};
