#include "bullet.hpp"

Bullet::Bullet(QString name, QObject* parent)
    : Entity(name, parent)
{
    drawer_ = new BulletDrawer("../silent-edge/src/images/bullet.png");
}

Bullet::BulletType Bullet::getBulletType()
{
    return type_;
}

qreal Bullet::getDamageDealt()
{
    return damage_dealt_;
}

QVariant Bullet::toVariant() const
{

}

void Bullet::fromVariant(const QVariant &)
{

}

void Bullet::setAim_dir(const QVector2D &aim_dir)
{
    aim_dir_ = aim_dir;
}

QVector2D Bullet::aim_dir() const
{
    return aim_dir_;
}

void Bullet::setName(const QString &name)
{
    name_ = name;
}



