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

WeaponDrawer *Weapon::getDrawer()
{
    return drawer_;
}



QVariant Weapon::toVariant() const
{
    QVariantMap weaponData;
    weaponData.insert("type", static_cast<int>(type_));
    weaponData.insert("price", price_);
    weaponData.insert("damage_dealt", damage_dealt_);

    return weaponData;
}

void Weapon::fromVariant(const QVariant& variant)
{
    QVariantMap weaponData = variant.toMap();

    type_ = static_cast<RarenessType>(weaponData["type"].toInt());
    price_ = weaponData["price"].toReal();
    damage_dealt_ = weaponData["damage_dealt"].toReal();
}
