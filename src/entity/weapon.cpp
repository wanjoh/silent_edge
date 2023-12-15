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
