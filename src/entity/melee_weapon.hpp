#pragma once

#include "../gui/melee_drawer.hpp"
#include "weapon.hpp"

class MeleeWeapon : public Weapon
{
public:
    MeleeWeapon();

    MeleeWeapon(const QString&, RarenessType = RarenessType::COMMON, qreal = 200.0, qreal = 10.0, qreal = 1000.0, QObject* = nullptr);
    auto getSwingTime() const -> qreal;

private:
    qreal swing_time_;
};
