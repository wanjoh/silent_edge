#pragma once

#include "../gui/bullet_drawer.hpp"

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

    Bullet(int, QString, qreal damage_dealt = 5, QObject* = nullptr);

    ~Bullet();
    BulletType getBulletType();
    qreal getDamageDealt();

    void setOwnerName(const QString &);
    QString getOwnerName();

private:
    int id_;
    QString owner_name_;
    BulletType type_;
    qreal damage_dealt_;
};
