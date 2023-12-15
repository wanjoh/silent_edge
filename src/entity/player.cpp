#include "player.hpp"

#include <QKeyEvent>
//TODO: OCISTITI DJUBRE
Player::Player() : Entity()
{

}
Player::Player(QString name, bool enemy, int x, int y, QObject* parent) : Entity(name, parent)
{
    drawer_ = new PlayerDrawer(enemy ? "../silent-edge/src/images/enemy.png" : "../silent-edge/src/images/samurai.png");
    drawer_->setPos(x, y);
    drawer_->setZValue(2);
}

QVariant Player::toVariant() const
{
    QVariantMap map;
    map.insert("name", name_);
    map.insert("position_x", drawer_->x());
    map.insert("position_y", drawer_->y());
    map.insert("rotation", drawer_->rotation());
//    map.insert("hp", hp_);
    //qDebug() << "to variant" << map;

    return map;
}

void Player::fromVariant(const QVariant& variant)
{
    const auto map = variant.toMap();
    QString name = map.value("name").toString();
    qreal x = map.value("position_x").toReal();
    qreal y = map.value("position_y").toReal();
    drawer_->setPos(x, y);
    drawer_->setRotation(map.value("rotation").toReal());
    name_ = name;
    //qDebug() << "from variant" << x << " " << y << " " << name;
}

qreal Player::getHp()
{
    return hp_;
}

void Player::setHp(qreal hp)
{
    hp_ = hp;
}
