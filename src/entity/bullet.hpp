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

    Bullet(QString, QObject* = nullptr);
    ~Bullet() = default;
    BulletType getBulletType();
private:
    BulletType type_;
};
