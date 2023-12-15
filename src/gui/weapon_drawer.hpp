#pragma once
#include "entity_drawer.hpp"

class WeaponDrawer : public EntityDrawer
{
public:
    static constexpr QRect DEFAULT_WEAPON_RECT = QRect(0, 0, 15, 30);
    WeaponDrawer(Qt::GlobalColor = Qt::gray, const QRect& = DEFAULT_WEAPON_RECT,
                 const QString& = "path_to_pixmap",
                 QGraphicsItem* = nullptr);
};
