#include "weapon.hpp"

Weapon::Weapon():Entity(){}

Weapon::Weapon(QString name, RarenessType rtype, qreal price, qreal dmg_dealt,  qreal fire_rate, QObject* parent)
    :Entity(name, parent)
    , type_(rtype)
    , price_(price)
    , damage_dealt_(dmg_dealt)
    , fire_rate_(fire_rate)
{

}

qreal Weapon::getFireRate()
{
    return fire_rate_;
}


void Weapon::setFireRate(qreal fire_rate)
{
    fire_rate_ = fire_rate;
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
