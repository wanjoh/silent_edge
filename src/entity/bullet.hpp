#pragma once

#include "entity.hpp"

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

    constexpr static quint32 BULLET_HEIGHT = 20;
    constexpr static quint32 BULLET_WIDTH = 10;

    Bullet(QString, QObject* = nullptr);

    ~Bullet() = default;
    BulletType getBulletType();

    qreal getDamageDealt();

    QVariant toVariant() const override;
    void fromVariant(const QVariant&) override;
    void setAim_dir(const QVector2D &newAim_dir);

    QVector2D aim_dir() const;

    void setName(const QString &newName);

private:
    BulletType type_;
    qreal damage_dealt_;
    QVector2D aim_dir_;

};
