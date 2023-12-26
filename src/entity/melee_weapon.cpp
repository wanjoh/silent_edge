#include "melee_weapon.hpp"
#include <QPropertyAnimation>

MeleeWeapon::MeleeWeapon()
    : Weapon()
{
}

MeleeWeapon::MeleeWeapon(QString name, RarenessType rtype, qreal price, qreal dmg_dealt, qreal swing_time, QObject* parent)
    : Weapon(name, rtype, price, dmg_dealt, parent), swing_time_(swing_time)
{
    drawer_ = new MeleeDrawer(name,"../silent-edge/src/images/katana.png");
}

qreal MeleeWeapon::getSwingTime() const
{
    return swing_time_;
}


QVariant MeleeWeapon::toVariant() const
{
    QVariantMap weaponData = Weapon::toVariant().toMap();
    weaponData.insert("swing_time", swing_time_);

    return weaponData;
}

void MeleeWeapon::fromVariant(const QVariant& variant)
{
    Weapon::fromVariant(variant);

    QVariantMap weaponData = variant.toMap();
    swing_time_ = weaponData["swing_time"].toReal();
}
