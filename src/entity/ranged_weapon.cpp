#include "ranged_weapon.hpp"

#include <utility>

RangedWeapon::RangedWeapon()
    : Weapon()
{}

RangedWeapon::RangedWeapon(QString name,
                           RarenessType rtype,
                           qreal price,
                           qreal dmg_dealt,
                           qreal shot_cooldown,
                           qint32 capacity,
                           qreal rel_time,
                           qint32 rem_bul,
                           QObject *parent)
    : Weapon(std::move(name), rtype, price, dmg_dealt, parent)
    , shot_cooldown_(shot_cooldown)
    , reload_time_(rel_time)
    , capacity_(capacity)
    , remaining_bullets_(rem_bul)
{

}

auto RangedWeapon::getShotCooldown() const -> qreal
{
    return shot_cooldown_;
}

auto RangedWeapon::getReloadTime() const -> qreal
{
    return reload_time_;
}

//std::list<Bullet> RangedWeapon::getBulletsUsed()
//{
//    return bullets_used_;
//}


auto RangedWeapon::getCapacity() const -> qint32
{
    return capacity_;
}

auto RangedWeapon::getRemainingBullets() const -> qint32
{
    return remaining_bullets_;
}

void RangedWeapon::setRemainingBullets(qint32 remaining_bullets)
{
    remaining_bullets_ = remaining_bullets;
}

