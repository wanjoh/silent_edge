#pragma once

#include "entity.hpp"
#include "weapon.hpp"

class Player : public Entity
{
public:
    // ovo ce leteti odavde
    constexpr static qint32 VELOCITY = 5;
    constexpr static QRect INITIAL = QRect(0, 0, 50, 50);
//    using Entity::Entity;
    Player() {};

    qreal getHp();
    qreal getEnergy();
    qreal getDpp();
    qreal getScore();
    void move(std::pair<qint32, qint32>);
    // ovo ce verovatno ici u kontroler?
    void keyPressEvent(QKeyEvent* event);

private:
    // nakon sto dodamo klase za  crtanje eniteta, ovo cemo skloniti
    qreal hp_;
    qreal energy_;
    qreal dpp_;
    qreal score_;
    std::list<Weapon> weapon_list_;
};
