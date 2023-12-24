#include "player.hpp"

#include <QKeyEvent>

Player::Player() : Entity()
{
    weapon_list_.push_back(new Weapon("Katana", Weapon::RarenessType::COMMON, 50.0, 20.0, 1.0));
    weapon_index_ = 0;
}

Player::Player(QString name, bool enemy, qreal hp, int x, int y, QObject* parent) : Entity(name, parent)
{
    weapon_list_.push_back(new Weapon("Katana", Weapon::RarenessType::COMMON, 50.0, 20.0, 1.0));
    weapon_list_.push_back(new Weapon("Pistol", Weapon::RarenessType::COMMON, 500.0, 200.0, 6.6));
    weapon_index_ = 0;

    drawer_ = new PlayerDrawer(name, (enemy ? "../silent-edge/src/images/enemy.png" : "../silent-edge/src/images/samurai.png"));
    drawer_->setPos(x, y);
    entityType_ = "player";
    hp_ = hp;

}

void Player::addWeapon(Weapon *weapon)
{
    weapon_list_.push_back(weapon);
}


Weapon* Player::currentWeapon()
{
    if (!weapon_list_.empty())
    {
        return weapon_list_.at(weapon_index_);
    }
    return nullptr;
}

Weapon* Player::nextWeapon()
{
    if (!weapon_list_.empty())
    {
        weapon_index_ = (weapon_index_ + 1) % weapon_list_.size();
        return weapon_list_.at(weapon_index_);
    }
    return nullptr;
}

Weapon* Player::previousWeapon()
{
    if (!weapon_list_.empty())
    {
        if (weapon_index_ == 0)
        {
            weapon_index_ = weapon_list_.size() - 1;
        }
        else
        {
            --weapon_index_;
        }
        return weapon_list_.at(weapon_index_);
    }
    return nullptr;
}

qreal Player::getHp()
{
    return hp_;
}

void Player::setHp(qreal hp)
{
    hp_ = hp;
}
