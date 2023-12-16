#include "tile_drawer.hpp"

TileDrawer::TileDrawer(const QString& pixmap_path, QGraphicsItem* parent):
    QGraphicsPixmapItem(QPixmap(pixmap_path), parent)
{

}

void TileDrawer::setImagePath(const QString& imagePath)
{
    QPixmap pixmap(imagePath);
    setPixmap(pixmap);
}
