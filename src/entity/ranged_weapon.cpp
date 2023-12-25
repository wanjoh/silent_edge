#include "ranged_weapon.hpp"

RangedWeapon::RangedWeapon()
    : Weapon()
{
}

RangedWeapon::RangedWeapon(QString name, RarenessType rtype, qreal price, qreal dmg_dealt, qreal fire_rate, qint32 capacity, qreal rel_time, QObject* parent)
    : Weapon(name, rtype, price, dmg_dealt, parent), rate_of_fire_(fire_rate), reload_time_(rel_time)
{

}

qreal RangedWeapon::getRateOfFire()
{
    return rate_of_fire_;
}

qreal RangedWeapon::getReloadTime() const
{
    return reload_time_;
}

//std::list<Bullet> RangedWeapon::getBulletsUsed()
//{
//    return bullets_used_;
//}
