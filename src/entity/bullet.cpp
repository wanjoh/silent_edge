#include "bullet.hpp"
#include "../gui/bullet_drawer.hpp"

Bullet::Bullet(QString name, QObject* parent)
    : Entity(name, parent)
{

    drawer_ = new BulletDrawer(Qt::black,BulletDrawer::DEFAULT_BULLET_RECT);

}



Bullet::BulletType Bullet::getBulletType()
{
    return type_;
}

qreal Bullet::getDamageDealt()
{
    return damage_dealt_;
}

BulletDrawer *Bullet::getDrawer()
{
    return drawer_;
}

QVariant Bullet::toVariant() const
{
    QVariantMap map;
    map["type"] = static_cast<int>(type_);
    map["damage_dealt"] = damage_dealt_;
    map["aim_dir_x"] = aim_dir_.x();
    map["aim_dir_y"] = aim_dir_.y();
    return map;
}

void Bullet::fromVariant(const QVariant &variant)
{
    QVariantMap map = variant.toMap();
    type_ = static_cast<BulletType>(map.value("type").toInt());
    damage_dealt_ = map.value("damage_dealt").toDouble();
    aim_dir_ = QVector2D(map.value("aim_dir_x").toFloat(), map.value("aim_dir_y").toFloat());
}

void Bullet::setAim_dir(const QVector2D &aim_dir)
{
    aim_dir_ = aim_dir;
}

QVector2D Bullet::aim_dir() const
{
    return aim_dir_;
}



