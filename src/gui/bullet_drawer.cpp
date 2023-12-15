#include "bullet_drawer.hpp"

BulletDrawer::BulletDrawer(Qt::GlobalColor color, const QRect& rect,
                           const QString& pixmapPath,
                           QGraphicsItem* parent) :
    EntityDrawer(color, rect, pixmapPath, parent)
{

}
