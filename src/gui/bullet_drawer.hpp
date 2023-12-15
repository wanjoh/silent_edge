#pragma once

#include "entity_drawer.hpp"

class BulletDrawer : public EntityDrawer
{
public:
    static constexpr QRect DEFAULT_BULLET_RECT = QRect(0, 0, 10, 20);
    BulletDrawer(Qt::GlobalColor = Qt::red, const QRect& = DEFAULT_BULLET_RECT,
                 const QString& = "path_to_pixmap",
                 QGraphicsItem* = nullptr);

};
