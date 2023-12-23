#pragma once

#include "weapon.hpp"

class RangedWeapon : public Weapon
{
public:
    RangedWeapon();
    RangedWeapon(QString, RarenessType = RarenessType::COMMON, qreal = 200.0, qreal = 10.0, qreal = 150.0, qint32 = 30, qreal = 700.0, qint32 = 200, QObject* = nullptr);
    qreal getShotCooldown() const;
    qreal getReloadTime() const;
    qint32 getCapacity() const;
    qint32 getRemainingBullets() const;
    void setRemainingBullets(qint32);
    QVariant toVariant() const override;
    void fromVariant(const QVariant&) override;

private:
    qreal shot_cooldown_;
    qreal reload_time_;
    qint32 capacity_;
    qint32 remaining_bullets_;
};
