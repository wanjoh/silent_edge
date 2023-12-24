#include "weapon.hpp"

Weapon::Weapon():Entity(){}

Weapon::Weapon(QString name, RarenessType rtype, qreal price, qreal dmg_dealt,  qreal fire_rate, qint32 capacity, qreal rel_time,QObject* parent)
    :Entity(name, parent)
    , type_(rtype)
    , price_(price)
    , damage_dealt_(dmg_dealt)
    , fire_rate_(fire_rate)
    , ammo_capacity_(capacity)
    , reload_time_(rel_time)
{

}

qreal Weapon::getPrice()
{
    return price_;
}

qreal Weapon::getDamageDealt()
{
    return damage_dealt_;
}

qreal Weapon::getFireRate()
{
    return fire_rate_;
}

qreal Weapon::getReloadTime()
{
    return reload_time_;
}

Weapon::RarenessType Weapon::getRarenessClass()
{
    return type_;
}
