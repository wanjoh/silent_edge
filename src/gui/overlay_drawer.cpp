#include "overlay_drawer.hpp"

OverlayDrawer::OverlayDrawer(const QString& pixmap_path, QGraphicsItem* parent):
    QGraphicsPixmapItem(QPixmap(pixmap_path), parent)
{

}
