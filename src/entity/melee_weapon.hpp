#pragma once

#include "weapon.hpp"

class MeleeWeapon : public Weapon
{
public:
    MeleeWeapon();
    MeleeWeapon(QString, RarenessType = RarenessType::COMMON, qreal = 200.0, qreal = 10.0, qreal = 400.0, QObject* = nullptr);
    qreal getCooldown();
private:
    qreal cooldown_;
};
