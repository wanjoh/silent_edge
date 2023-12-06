#include "entity_drawer.hpp"
#include <QBrush>
#include <QPixmap>

EntityDrawer::EntityDrawer(Qt::GlobalColor color,
                           const QString& pixmap_path,
                           const QRect& rect,
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
