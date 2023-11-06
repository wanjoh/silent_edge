#pragma once

#include "weapon.h"

class MeleeWeapon : public Weapon
{
public:
    float get_cooldown();
private:
    float cooldown;
};
