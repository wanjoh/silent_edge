#include "entity_drawer.hpp"
#include <QBrush>
#include <QPixmap>

EntityDrawer::EntityDrawer(Qt::GlobalColor color, const QRect& rect,
                           const QString& pixmap_path,
                           QGraphicsItem* parent):
    QGraphicsRectItem(parent)
    //, pixmap_(new QPixmap(pixmap_path))
{
    setRect(rect);
    setBrush(color);
    setTransformOriginPoint(this->rect().center());
}
