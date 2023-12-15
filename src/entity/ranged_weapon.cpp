#include "ranged_weapon.hpp"

RangedWeapon::RangedWeapon()
    : Weapon()
{
}

RangedWeapon::RangedWeapon(QString name, RarenessType rtype, qreal price, qreal dmg_dealt, qreal fire_rate, qint32 capacity, qreal rel_time, QObject* parent)
    : Weapon(name, rtype, price, dmg_dealt, parent), rate_of_fire_(fire_rate), reload_time_(rel_time), capacity_(capacity)
{

}

qreal RangedWeapon::getRateOfFire() const
{
    return rate_of_fire_;
}

qreal RangedWeapon::getReloadTime() const
{
    return reload_time_;
}

QVariant RangedWeapon::toVariant() const
{
    QVariantMap weaponData = Weapon::toVariant().toMap();
    weaponData.insert("rate_of_fire", rate_of_fire_);
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
    rate_of_fire_ = weaponData["rate_of_fire"].toReal();
    reload_time_ = weaponData["reload_time"].toReal();
}

//std::list<Bullet> RangedWeapon::getBulletsUsed()
//{
//    return bullets_used_;
//}
