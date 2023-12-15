#include "player_drawer.hpp"

PlayerDrawer::PlayerDrawer(Qt::GlobalColor color, const QRect& rect,
                           const QString& pixmapPath,
                           QGraphicsItem* parent) :
    EntityDrawer(color, rect, pixmapPath, parent)
{

}
