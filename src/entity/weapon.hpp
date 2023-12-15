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
    Weapon(QString, RarenessType = Weapon::RarenessType::COMMON, qreal = 200.0, qreal = 10.0, QObject* = nullptr);

    qreal getPrice();
    qreal getDamageDealt();
    RarenessType getRarenessClass();
    QVariant toVariant() const override;
    void fromVariant(const QVariant&) override;


private:
    RarenessType type_;
    qreal price_;
    qreal damage_dealt_;

};
