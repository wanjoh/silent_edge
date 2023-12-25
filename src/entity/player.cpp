#include "player.hpp"

#include <QKeyEvent>

Player::Player() : Entity()
{
    meleeWeapon_ = new MeleeWeapon("Katana", Weapon::RarenessType::COMMON, 50.0, 20.0, 400.0);
    rangedWeapon_ = new RangedWeapon("Gun", Weapon::RarenessType::COMMON, 200.0, 10.0, 5.3, 30, 300.0);
}

Player::Player(QString name, bool enemy, qreal hp, int x, int y, QObject* parent) : Entity(name, parent)
{
    meleeWeapon_ = new MeleeWeapon("Katana", Weapon::RarenessType::COMMON, 50.0, 20.0, 1.0);
    rangedWeapon_ = new RangedWeapon("Gun", Weapon::RarenessType::COMMON, 200.0, 10.0, 3.3, 30, 700.0);
    drawer_ = new PlayerDrawer(name, (enemy ? "../silent-edge/src/images/enemy.png" : "../silent-edge/src/images/samurai.png"));

    drawer_->setPos(x, y);
    entityType_ = "player";
    hp_ = hp;

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
