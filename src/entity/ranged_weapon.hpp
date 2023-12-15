#pragma once

#include "bullet.hpp"
#include "weapon.hpp"

class RangedWeapon : public Weapon
{
public:
    RangedWeapon();
    RangedWeapon(QString, RarenessType = RarenessType::COMMON, qreal = 200.0, qreal = 10.0, qreal = 3.3, qint32 = 30, qreal = 700.0, QObject* = nullptr);
    qreal getRateOfFire();
    qreal getReloadTime() const;
    QVariant toVariant() const override;
    void fromVariant(const QVariant&) override;
    //std::list<Bullet> getBulletsUsed();
private:
    qreal rate_of_fire_;
    //std::list<Bullet> bullets_used_;
    qreal reload_time_;
};
