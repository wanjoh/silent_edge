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
    Weapon(QString, RarenessType = Weapon::RarenessType::COMMON, qreal = 0.0, qreal = 10.0, qreal = 3.3, QObject* = nullptr);

    qreal getPrice();
    qreal getDamageDealt();
    qreal getFireRate();
    void setFireRate(qreal);
    RarenessType getRarenessClass();    
    QVariant toVariant() const override;
    void fromVariant(const QVariant&) override;
private:
    RarenessType type_;
    qreal price_;
    qreal damage_dealt_;
    qreal fire_rate_;
};
