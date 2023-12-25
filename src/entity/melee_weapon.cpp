#include "melee_weapon.hpp"

MeleeWeapon::MeleeWeapon()
    : Weapon()
{
}

MeleeWeapon::MeleeWeapon(QString name, RarenessType rtype, qreal price, qreal dmg_dealt, qreal cooldown, QObject* parent)
    : Weapon(name, rtype, price, dmg_dealt, parent), cooldown_(cooldown)
{
}

qreal MeleeWeapon::getCooldown()
{
    return cooldown_;
}
