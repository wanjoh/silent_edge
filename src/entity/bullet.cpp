#include "bullet.hpp"

#include <utility>


Bullet::Bullet(int id, QString owner_name, qreal damage_dealt, QObject* parent)
    : Entity(QString::number(id), parent)
{
    drawer_ = new BulletDrawer(QString::number(id), "../silent-edge/src/images/bullet.png");
    drawer_->setScale(1.5);

    entityType_ = "bullet";
    damage_dealt_ = damage_dealt;
    id_ = id;
    owner_name_ = std::move(owner_name);
}

Bullet::~Bullet()
{
    delete drawer_;
    qDebug() << "metak " << name_ << "unisten";
}


auto Bullet::getBulletType() -> Bullet::BulletType
{
    return type_;
}

auto Bullet::getDamageDealt() const -> qreal
{
    return damage_dealt_;
}

void Bullet::setOwnerName(const QString &new_owner_name)
{
    owner_name_ = new_owner_name;
}

auto Bullet::getOwnerName() -> QString
{
    return owner_name_;
}
