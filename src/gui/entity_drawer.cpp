#include "entity_drawer.hpp"

EntityDrawer::EntityDrawer(const QString& pixmap_path,
                           qreal velocity,
                           QGraphicsItem* parent):
    QGraphicsPixmapItem(QPixmap(pixmap_path), parent)
    , velocity_(velocity)
{

}

qreal EntityDrawer::getVelocity()
{
    return velocity_;
}
