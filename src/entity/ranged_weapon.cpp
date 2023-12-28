#include "ranged_weapon.hpp"

RangedWeapon::RangedWeapon()
    : Weapon()
{
}

RangedWeapon::RangedWeapon(QString name, RarenessType rtype, qreal price, qreal dmg_dealt, qreal shot_cooldown, qint32 capacity, qreal rel_time, qint32 rem_bul, QObject* parent)
    : Weapon(name, rtype, price, dmg_dealt, parent), shot_cooldown_(shot_cooldown), reload_time_(rel_time), capacity_(capacity), remaining_bullets_(rem_bul)
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

//std::list<Bullet> RangedWeapon::getBulletsUsed()
//{
//    return bullets_used_;
//}


qint32 RangedWeapon::getCapacity() const
{
    return capacity_;
}

qint32 RangedWeapon::getRemainingBullets() const
{
    return remaining_bullets_;
}

void RangedWeapon::setRemainingBullets(qint32 remaining_bullets)
{
    remaining_bullets_ = remaining_bullets;
}

