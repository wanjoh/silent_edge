#include "bullet.hpp"


Bullet::Bullet(QString name, qreal damage_dealt, QObject* parent)
    : Entity(name, parent)
{
    drawer_ = new BulletDrawer(name, "../silent-edge/src/images/bullet.png");
    entityType_ = "bullet";
    damage_dealt_ = damage_dealt;
    name_ = name;
}

Bullet::BulletType Bullet::getBulletType()
{
    return type_;
}

qreal Bullet::getDamageDealt()
{
    return damage_dealt_;
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

