#include "weapon_drawer.hpp"

WeaponDrawer::WeaponDrawer(Qt::GlobalColor color, const QRect& rect,
                           const QString& pixmapPath,
                           QGraphicsItem* parent) :
    EntityDrawer(color, rect, pixmapPath, parent)
{

}
