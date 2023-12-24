#pragma once

#include "entity.hpp"

#include <QVector2D>

class Weapon : public Entity
{
public:

    enum class RarenessType
    {
        COMMON,
        UNCOMMON,
        RARE,
        EPIC,
        LEGENDARY
    };


    Weapon();
    Weapon(QString, RarenessType = Weapon::RarenessType::COMMON, qreal = 200.0, qreal = 10.0, qreal = 3.3, qint32 = 30, qreal = 700.0, QObject* = nullptr);

    qreal getPrice();
    qreal getDamageDealt();
    qreal getFireRate();
    qreal getReloadTime();
    RarenessType getRarenessClass();


private:
    RarenessType type_;
    qreal price_;
    qreal damage_dealt_;
    qreal fire_rate_;
    qint32 ammo_capacity_;
    qreal reload_time_;
};
