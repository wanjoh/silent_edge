#include "bullet.hpp"

Bullet::Bullet(QString name, QObject* parent)
    : Entity(name, parent)
{

    drawer_ = new EntityDrawer(Qt::black,BULLET_RECT);
    entityType_ = "bullet";
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
    map.insert("damage_dealt", damage_dealt_);

    //    map.insert("aim_dir", aim_dir_);

    //    qDebug() << aim_dir_.x() << " " << aim_dir_.y();
    map.insert("aim_x", aim_dir_.x());
    map.insert("aim_y", aim_dir_.y());

    //    qDebug() << "toVariant " << drawer_->x() << " " << drawer_->y();
    map.insert("position_x", drawer_->x());
    map.insert("position_y", drawer_->y());

    return map;
}

void Bullet::fromVariant(const QVariant & variant)
{
    const auto map = variant.toMap();
    entityType_ = map.value("type_entity").toString();
    damage_dealt_ = map.value("damage_dealt").toReal();

    //    aim_dir_ = map.value("aim_dir").value<QVector2D>();

    //    QVector2D vec = QVector2D(x, y);
    //    setAim_dir(vec);

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


