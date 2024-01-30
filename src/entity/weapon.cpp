#include "weapon.hpp"

#include <utility>

Weapon::Weapon()
    : Entity()
{}

Weapon::Weapon(QString name, RarenessType rtype, qreal price, qreal dmg_dealt, QObject *parent)
    : Entity(std::move(name), parent)
    , type_(rtype)
    , price_(price)
    , damage_dealt_(dmg_dealt)
{

}

qreal Weapon::getPrice() const
{
    return price_;
}

qreal Weapon::getDamageDealt() const
{
    return damage_dealt_;
}


Weapon::RarenessType Weapon::getRarenessClass()
{
    return type_;
}
