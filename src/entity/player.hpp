#pragma once

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
    Player(QString, bool = true, QObject* = nullptr);

    qreal getHp();
    qreal getEnergy();
    qreal getDpp();
    qreal getScore();
    QVariant toVariant() const override;
    void fromVariant(const QVariant&) override;
    QString entityType;
private:
    qreal hp_;
    qreal energy_;
    qreal dpp_;
    qreal score_;

    std::vector<Weapon*> weapon_list_;
};
