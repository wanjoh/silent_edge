#pragma once

#include "entity.h"

class Bullet : public Entity
{
public:
    enum class bullet_type {
        LIGHT,
        MEDIUM,
        HEAVY
    };

    std::string get_bullet_type();
private:
    bullet_type type;
};
