#include "player.hpp"

#include <QKeyEvent>
//TODO: OCISTITI DJUBRE
Player::Player() : Entity()
{

}
Player::Player(QString name, bool enemy, QObject* parent) : Entity(name, parent)
{
    drawer_ = new EntityDrawer(enemy ? Qt::red : Qt::green);
}

QVariant Player::toVariant() const
{
    QVariantMap map;
    map.insert("name", name_);
    map.insert("position_x", drawer_->x());
    map.insert("position_y", drawer_->y());
    //qDebug() << "to variant" << map;
    //map.insert("hp", hp_);

    return map;
}

void Player::fromVariant(const QVariant& variant)
{
    const auto map = variant.toMap();
    QString name = map.value("name").toString();
    qreal x = map.value("position_x").toReal();
    qreal y = map.value("position_y").toReal();
    drawer_->setPos(x, y);
    // ovde ce ici i rotacija i sve ostalo
    name_ = name;
    //qDebug() << "from variant" << x << " " << y << " " << name;
}
