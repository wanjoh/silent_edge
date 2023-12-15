#pragma once

#include <QGraphicsPixmapItem>

class OverlayDrawer : public QGraphicsPixmapItem
{
public:
    OverlayDrawer(const QString &pixmap_path = "path_to_pixmap", QGraphicsItem * = nullptr);
private:

};
