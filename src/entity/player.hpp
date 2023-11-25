#pragma once

#include "entity.hpp"
#include "weapon.hpp"
#include<QMap>
#include<QTimer>

class Player : public Entity
{
    Q_OBJECT
public:
    // ovo ce leteti odavde
    constexpr static qint32 VELOCITY = 3;
    constexpr static QRect INITIAL = QRect(0, 0, 50, 50);
    Player() {};

    Player(QString);

    Player(QString, qreal, qreal);

    qreal getHp();
    qreal getEnergy();
    qreal getDpp();
    qreal getScore();
    void setName(QString name);
    void move(Player*);
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void initializeTimer();
    QVariant toVariant() const override;
    void fromVariant(const QVariant&) override;
    QString getName();
public slots:
    void updateMovement();
signals:
    void positionChangedSignal();
private:
    QString name_;
    qreal hp_;
    qreal energy_;
    qreal dpp_;
    qreal score_;
    QMap<int, bool> keys_;
    QTimer movementTimer_;

//    std::list<Weapon> weapon_list_;
};
