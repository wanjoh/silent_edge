#pragma once
#include "entity_drawer.hpp"

class PlayerDrawer : public EntityDrawer
{
public:
    PlayerDrawer(Qt::GlobalColor = Qt::red, const QRect& = DEFAULT_RECT,
                 const QString& = "path_to_pixmap",
                 QGraphicsItem* = nullptr);
};
