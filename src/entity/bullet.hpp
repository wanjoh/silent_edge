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

    Bullet(int, QString, qreal damage_dealt = 20, QObject* = nullptr);

    ~Bullet() override;
    auto getBulletType() -> BulletType;
    auto getDamageDealt() const -> qreal;

    void setOwnerName(const QString &);
    auto getOwnerName() -> QString;

private:
    int id_;
    QString owner_name_;
    BulletType type_;
    qreal damage_dealt_;
};
