#pragma once

#include "entity.hpp"
#include "../gui/bullet_drawer.hpp"
#include <QVector2D>



class Bullet : public Entity
{
    Q_OBJECT
public:
    enum class BulletType
    {
        LIGHT,
        MEDIUM,
        HEAVY
    };


    Bullet(QString, QObject* = nullptr);

    ~Bullet() = default;
    BulletType getBulletType();

    qreal getDamageDealt();

    BulletDrawer *getDrawer() override;
    QVariant toVariant() const override;
    void fromVariant(const QVariant&) override;
    void setAim_dir(const QVector2D &newAim_dir);

    QVector2D aim_dir() const;

protected:
    BulletDrawer* drawer_;

private:
    BulletType type_;
    qreal damage_dealt_;
    QVector2D aim_dir_;

};
