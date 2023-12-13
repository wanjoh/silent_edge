#include "entity_drawer.hpp"

EntityDrawer::EntityDrawer(const QString& pixmap_path,
                           QGraphicsItem* parent):
    QGraphicsPixmapItem(QPixmap(pixmap_path), parent)
{

}
