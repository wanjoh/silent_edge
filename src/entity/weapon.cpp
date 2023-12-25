#include "weapon.hpp"

Weapon::Weapon():Entity(){}

Weapon::Weapon(QString name, RarenessType rtype, qreal price, qreal dmg_dealt,  QObject* parent)
    :Entity(name, parent)
    , type_(rtype)
    , price_(price)
    , damage_dealt_(dmg_dealt)
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


Weapon::RarenessType Weapon::getRarenessClass()
{
    return type_;
}
