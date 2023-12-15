#pragma once

#include "../gui/player_drawer.hpp"
#include "entity.hpp"
#include "weapon.hpp"
#include<QMap>

class Player : public Entity
{
    Q_OBJECT
public:
    // ovo ce leteti odavde
    constexpr static qint32 VELOCITY = 3;
    Player();
    Player(QString, bool = true, int x = 64, int y = 64, QObject* = nullptr);

    qreal getHp();
    qreal getEnergy();
    qreal getDpp();
    qreal getScore();
    void setHp(qreal);
    QVariant toVariant() const override;
    void fromVariant(const QVariant&) override;

private:
    qreal hp_;
    qreal energy_;
    qreal dpp_;
    qreal score_;

    std::vector<Weapon*> weapon_list_;
};
