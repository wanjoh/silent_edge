#pragma once

#include "entity.hpp"

class Bullet : public Entity
{
public:
    enum class BulletType
    {
        LIGHT,
        MEDIUM,
        HEAVY
    };

    BulletType getBulletType();
private:
    BulletType type_;
};
