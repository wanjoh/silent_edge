#include "player.hpp"


#include <QKeyEvent>
//TODO: OCISTITI DJUBRE
Player::Player() : Entity()
{
    weapon_list_.push_back(new Weapon("Katana", Weapon::RarenessType::COMMON, 50.0, 20.0, 1.0));
    weapon_index_ = 0;
}
Player::Player(QString name, bool enemy, int x, int y, QObject* parent) : Entity(name, parent)
{

    drawer_ = new EntityDrawer(enemy ? "../silent-edge/src/images/enemy.png" : "../silent-edge/src/images/samurai.png");
    drawer_->setPos(x, y);
    drawer_->setZValue(2);

    weapon_list_.push_back(new Weapon("Katana", Weapon::RarenessType::COMMON, 50.0, 20.0, 1.0));
    weapon_list_.push_back(new Weapon("Pistol", Weapon::RarenessType::COMMON, 500.0, 200.0, 6.6));
    weapon_index_ = 0;
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
