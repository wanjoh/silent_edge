#pragma once

#include "entity.hpp"

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

    qreal getPrice();
    qreal getDamageDealt();
    RarenessType getRarenessClass();
private:
    RarenessType type_;
    qreal price_;
    qreal damage_dealt_;
};
