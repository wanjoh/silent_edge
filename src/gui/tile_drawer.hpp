#pragma once

#include <QGraphicsPixmapItem>

class TileDrawer : public QGraphicsPixmapItem
{
public:
    TileDrawer(const QString &pixmap_path = "path_to_pixmap", QGraphicsItem * = nullptr);
private:

};
