#include "entity_drawer.hpp"
#include <QBrush>
#include <QPixmap>

EntityDrawer::EntityDrawer(Qt::GlobalColor color, const QRect& rect,
                           const QString& pixmap_path, qreal velocity,
                           QGraphicsItem* parent):
    QGraphicsRectItem(parent)
    //, pixmap_(new QPixmap(pixmap_path))
    , velocity_(velocity)
{
    setRect(rect);
    setBrush(color);
}

qreal EntityDrawer::getVelocity()
{
    return velocity_;
}
