#include "player.hpp"

#include <QKeyEvent>

Player::Player() : Entity()
{
    melee_weapon_ = new MeleeWeapon("Katana", Weapon::RarenessType::COMMON, 50.0, 20.0, 400.0);
    ranged_weapon_ = new RangedWeapon("Gun", Weapon::RarenessType::COMMON, 200.0, 10.0, 200.0, 10u, 1300.0, 30u);

    reload_ = new Entity("reload");
    EntityDrawer* reload_drawer = new EntityDrawer(reload_->getName(), "../silent-edge/src/images/reload.png");
    reload_->setDrawer(reload_drawer);
    reload_drawer->setScale(0.5);
}

Player::Player(QString name, bool enemy, qreal hp, int x, int y, QObject* parent) : Entity(name, parent)
{

    melee_weapon_ = new MeleeWeapon("Katana", Weapon::RarenessType::COMMON, 50.0, 20.0, 1000.0);
    ranged_weapon_ = new RangedWeapon("Gun", Weapon::RarenessType::COMMON, 200.0, 10.0, 200.0, 10u, 1300.0, 20u);
    drawer_ = new PlayerDrawer(name, enemy ? "../silent-edge/src/images/enemy.png" : "../silent-edge/src/images/samurai.png");

    reload_ = new Entity("reload");
    EntityDrawer* reload_drawer = new EntityDrawer(reload_->getName(), "../silent-edge/src/images/reload.png");
    reload_->setDrawer(reload_drawer);
    reload_drawer->setScale(0.5);

    drawer_->setPos(x, y);
    drawer_->setZValue(6);
    entityType_ = "player";
    hp_ = hp;

    reload_timer_ = new QTimer(this);
    swing_timer_ = new QTimer(this);
    initializeTimers();

    melee_weapon_->getDrawer()->setTransformOriginPoint(melee_weapon_->getDrawer()->pixmap().rect().topLeft());

}

qreal Player::getHp()
{
    return hp_;
}

MeleeWeapon* Player::getMeleeWeapon() const
{
    return melee_weapon_;
}

RangedWeapon* Player::getRangedWeapon() const
{
    return ranged_weapon_;
}

Entity* Player::getReload() const
{
    return reload_;
}

void Player::initializeTimers()
{

    reload_timer_->setInterval(ranged_weapon_->getReloadTime());
    reload_timer_->setSingleShot(true);
    swing_timer_->setInterval(melee_weapon_->getSwingTime());
    swing_timer_->setSingleShot(true);


}


QTimer* Player::getReloadTimer() const
{
    return reload_timer_;
}

QTimer* Player::getSwingTimer() const
{
    return swing_timer_;
}

void Player::setHp(qreal hp)
{
    hp_ = hp;
}
