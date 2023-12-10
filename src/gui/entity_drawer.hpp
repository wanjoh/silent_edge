#pragma once

#include <QGraphicsPixmapItem>

class EntityDrawer : public QGraphicsPixmapItem
{
public:
    static constexpr qreal DEFAULT_VELOCITY = 5.0f;

    EntityDrawer(const QString &pixmap_path = "path_to_pixmap",
                 qreal = DEFAULT_VELOCITY,
                 QGraphicsItem * = nullptr);
    qreal getVelocity();
private:
    qreal velocity_;
};
