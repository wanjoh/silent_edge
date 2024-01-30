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

auto Weapon::getPrice() const -> qreal
{
    return price_;
}

auto Weapon::getDamageDealt() const -> qreal
{
    return damage_dealt_;
}


auto Weapon::getRarenessClass() -> Weapon::RarenessType
{
    return type_;
}
