#include "ranged_weapon.hpp"

RangedWeapon::RangedWeapon()
    : Weapon()
{
}

RangedWeapon::RangedWeapon(QString name, RarenessType rtype, qreal price, qreal dmg_dealt, qreal shot_cooldown, qint32 capacity, qreal rel_time, QObject* parent)
    : Weapon(name, rtype, price, dmg_dealt, parent), shot_cooldown_(shot_cooldown), reload_time_(rel_time), capacity_(capacity)
{

}

qreal RangedWeapon::getShotCooldown() const
{
    return shot_cooldown_;
}

qreal RangedWeapon::getReloadTime() const
{
    return reload_time_;
}

QVariant RangedWeapon::toVariant() const
{
    QVariantMap weaponData = Weapon::toVariant().toMap();
    weaponData.insert("shot_cooldown", shot_cooldown_);
    weaponData.insert("reload_time", reload_time_);

    return weaponData;
}

qint32 RangedWeapon::getCapacity() const
{
    return capacity_;
}

void RangedWeapon::fromVariant(const QVariant& variant)
{
    Weapon::fromVariant(variant);

    QVariantMap weaponData = variant.toMap();
    shot_cooldown_ = weaponData["shot_cooldown"].toReal();
    reload_time_ = weaponData["reload_time"].toReal();
}

//std::list<Bullet> RangedWeapon::getBulletsUsed()
//{
//    return bullets_used_;
//}
