#include "bullet.hpp"


Bullet::Bullet(QString name,qreal damage_dealt, QObject* parent)
    : Entity(name, parent)
{
    drawer_ = new BulletDrawer(name,"../silent-edge/src/images/bullet.png");
    entityType_ = "bullet";
    damage_dealt_ = damage_dealt;
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
    QVariantMap map;
    map.insert("type_entity", entityType_);
    map.insert("name", name_);
    map.insert("damage_dealt", damage_dealt_);
    map.insert("aim_x", aim_dir_.x());
    map.insert("aim_y", aim_dir_.y());
    map.insert("position_x", drawer_->x());
    map.insert("position_y", drawer_->y());

//    qDebug() << "toVariant drawer" << drawer_->x() << " " << drawer_->y();
//    qDebug() << "toVariant aim_dir " << aim_dir_.x() << " " << aim_dir_.y();

    return map;
}

void Bullet::fromVariant(const QVariant & variant)
{
    const auto map = variant.toMap();
    entityType_ = map.value("type_entity").toString();
    name_ = map.value("name").toString();
    damage_dealt_ = map.value("damage_dealt").toReal();

    qreal ax = map.value("aim_x").toReal();
    qreal ay = map.value("aim_y").toReal();
    aim_dir_.setX(ax);
    aim_dir_.setY(ay);

    qreal x = map.value("position_x").toReal();
    qreal y = map.value("position_y").toReal();
    drawer_->setPos(x, y);

//    qDebug() << "fromVariant " << drawer_->x() << " " << drawer_->y();
//    qDebug() << aim_dir_.x() << " " << aim_dir_.y();
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

