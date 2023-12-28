#include "bullet.hpp"


Bullet::Bullet(QString name, qreal damage_dealt, QObject* parent)
    : Entity(name, parent)
{

    drawer_ = new BulletDrawer(name,"../silent-edge/src/images/bullet.png");
    drawer_->setScale(2);

    entityType_ = "bullet";
    damage_dealt_ = damage_dealt;
    name_ = name;
}

Bullet::~Bullet()
{
    qDebug() << "metak " << name_ << "unisten" << drawer_->name();
}


Bullet::BulletType Bullet::getBulletType()
{
    return type_;
}

qreal Bullet::getDamageDealt()
{
    return damage_dealt_;
}


void Bullet::setName(const QString &name)
{
    name_ = name;
}

