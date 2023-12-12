#include "player.hpp"

#include <QKeyEvent>
//TODO: OCISTITI DJUBRE
Player::Player() : Entity()
{

}
Player::Player(QString name, bool enemy, QObject* parent) : Entity(name, parent)
{
    drawer_ = new EntityDrawer(enemy ? Qt::red : Qt::green);
    entityType = "player";
}

QVariant Player::toVariant() const
{
    QVariantMap map;
    map.insert("type_entity", entityType);
    map.insert("name", name_);
    map.insert("position_x", drawer_->x());
    map.insert("position_y", drawer_->y());
    map.insert("rotation", drawer_->rotation());
    map.insert("hp", hp_);
    //qDebug() << "to variant" << map;

    return map;
}

void Player::fromVariant(const QVariant& variant)
{
    const auto map = variant.toMap();
    entityType = map.value("type_entity").toString();
    QString name = map.value("name").toString();
    qreal x = map.value("position_x").toReal();
    qreal y = map.value("position_y").toReal();
    drawer_->setPos(x, y);
    drawer_->setRotation(map.value("rotation").toReal());
    hp_ = map.value("hp").toReal();

    name_ = name;
    //qDebug() << "from variant" << x << " " << y << " " << name;
    //    qDebug() << "from variant " << hp_;
}
