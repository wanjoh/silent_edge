#pragma once
#include "entity_drawer.hpp"

class PlayerDrawer : public EntityDrawer
{
public:
    PlayerDrawer(QString name,const QString &pixmap_path = "path_to_pixmap",
                 QGraphicsItem * = nullptr);
};
