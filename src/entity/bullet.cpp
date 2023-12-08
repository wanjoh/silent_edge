#include "bullet.hpp"

Bullet::Bullet(QString name, QObject* parent)
    : Entity(name, parent)
{

}

Bullet::BulletType Bullet::getBulletType()
{
    return type_;
}
