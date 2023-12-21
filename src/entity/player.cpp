#include "player.hpp"


#include <QKeyEvent>
//TODO: OCISTITI DJUBRE
Player::Player() : Entity()
{
    meleeWeapon_ = new MeleeWeapon("Katana", Weapon::RarenessType::COMMON, 50.0, 20.0, 400.0);
    rangedWeapon_ = new RangedWeapon("Gun", Weapon::RarenessType::COMMON, 200.0, 10.0, 200.0, 30, 1300.0);
}
Player::Player(QString name, bool enemy,qreal hp, int x, int y, QObject* parent) : Entity(name, parent)
{
    meleeWeapon_ = new MeleeWeapon("Katana", Weapon::RarenessType::COMMON, 50.0, 20.0, 1.0);
    rangedWeapon_ = new RangedWeapon("Gun", Weapon::RarenessType::COMMON, 200.0, 10.0, 200.0, 30, 1300.0);
    drawer_ = new PlayerDrawer(name,enemy ? "../silent-edge/src/images/enemy.png" : "../silent-edge/src/images/samurai.png");
    drawer_->setPos(x, y);
    drawer_->setZValue(2);
    entityType_ = "player";
    hp_ = hp;

}



QVariant Player::toVariant() const
{
    QVariantMap map;
    map.insert("type_entity", entityType_);
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
    entityType_ = map.value("type_entity").toString();
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

qreal Player::getHp()
{
    return hp_;
}

MeleeWeapon* Player::getMeleeWeapon() const
{
    return meleeWeapon_;
}

RangedWeapon* Player::getRangedWeapon() const
{
    return rangedWeapon_;
}

void Player::setHp(qreal hp)
{
    hp_ = hp;
}
