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

QVariant MeleeWeapon::toVariant() const
{
    QVariantMap weaponData = Weapon::toVariant().toMap();
    weaponData.insert("cooldown", cooldown_);

    return weaponData;
}

void MeleeWeapon::fromVariant(const QVariant& variant)
{
    Weapon::fromVariant(variant);

    QVariantMap weaponData = variant.toMap();
    cooldown_ = weaponData["cooldown"].toReal();
}
