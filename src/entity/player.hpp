#pragma once

#include "entity.hpp"
#include "weapon.hpp"

class Player : public Entity
{
    Q_OBJECT
public:
    // ovo ce leteti odavde
    constexpr static qint32 VELOCITY = 5;
    constexpr static QRect INITIAL = QRect(0, 0, 50, 50);
    Player() {};

    Player(QString);

    Player(QString, qreal, qreal);

    qreal getHp();
    qreal getEnergy();
    qreal getDpp();
    qreal getScore();
    void setName(QString name);
    void move(std::pair<qint32, qint32>);
    void keyPressEvent(QKeyEvent* event) override;
    QVariant toVariant() const override;
    void fromVariant(const QVariant&) override;
    QString getName();
signals:
    void positionChangedSignal();
private:
    QString name_;
    qreal hp_;
    qreal energy_;
    qreal dpp_;
    qreal score_;
//    std::list<Weapon> weapon_list_;
};
