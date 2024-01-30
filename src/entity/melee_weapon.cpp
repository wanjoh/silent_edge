#include "melee_weapon.hpp"
#include <QPropertyAnimation>

MeleeWeapon::MeleeWeapon()
    : Weapon()
{}

MeleeWeapon::MeleeWeapon(const QString& name,
                         RarenessType rtype,
                         qreal price,
                         qreal dmg_dealt,
                         qreal swing_time,
                         QObject *parent)
    : Weapon(name, rtype, price, dmg_dealt, parent)
    , swing_time_(swing_time)
{
    drawer_ = new MeleeDrawer(name,"../silent-edge/src/images/katana.png");
    drawer_->setScale(2);
}

auto MeleeWeapon::getSwingTime() const -> qreal
{
    return swing_time_;
}
