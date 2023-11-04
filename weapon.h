#pragma once

#include "entity.h"

class Weapon : public Entity
{
public:
    enum class rareness_type {
        COMMON,
        UNCOMMON,
        RARE,
        EPIC,
        LEGENDARY
    };

    float get_price();
    float get_damage_dealt();
    std::string get_rareness_class();
private:
    rareness_type type;
    float price;
    float damage_dealt;
};
