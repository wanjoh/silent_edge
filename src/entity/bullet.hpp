#pragma once

#include "entity.hpp"

#include <QVector2D>

#define BULLET_HEIGHT (20)
#define BULLET_WIDTH (10)


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

    static constexpr QRect BULLET_RECT = QRect(0, 0,BULLET_WIDTH, BULLET_HEIGHT);

    Bullet(QString, QObject* = nullptr);

    ~Bullet() = default;
    BulletType getBulletType();

    qreal getDamageDealt();


    QVariant toVariant() const override;
    void fromVariant(const QVariant&) override;
    void setAim_dir(const QVector2D &newAim_dir);

    QVector2D aim_dir() const;
    QString entityType_;

private:
    BulletType type_;
    qreal damage_dealt_;
    QVector2D aim_dir_;
};
