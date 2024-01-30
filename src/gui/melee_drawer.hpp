#pragma once
#include "entity_drawer.hpp"

class MeleeDrawer : public EntityDrawer
{
public:
    MeleeDrawer(const QString& name, const QString &pixmap_path = "path_to_pixmap", QGraphicsItem* parent = nullptr);

};
