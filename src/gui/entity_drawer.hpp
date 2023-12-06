#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>

class EntityDrawer : public QGraphicsPixmapItem
{
public:
    static constexpr qreal DEFAULT_VELOCITY = 5.0f;
    static constexpr QRect DEFAULT_RECT = QRect(0, 0, 32, 32);
    EntityDrawer(Qt::GlobalColor = Qt::red,
                 const QString& = "../images/samurai2.png",
                 const QRect &rect = DEFAULT_RECT,
                 qreal = DEFAULT_VELOCITY,
                 QGraphicsItem * = nullptr);
    qreal getVelocity();
private:
    qreal velocity_;
};
