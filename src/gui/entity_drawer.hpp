#pragma once

#include <QGraphicsPixmapItem>

class EntityDrawer : public QGraphicsPixmapItem
{
public:
    EntityDrawer(const QString &pixmap_path = "path_to_pixmap",
                 QGraphicsItem * = nullptr);
};
