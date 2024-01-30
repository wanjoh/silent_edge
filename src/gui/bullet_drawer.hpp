#pragma once
#include "entity_drawer.hpp"

class BulletDrawer : public EntityDrawer
{
public:
    BulletDrawer(const QString &name, const QString &pixmap_path = "path_to_pixmap",
                 QGraphicsItem * = nullptr);
    ~BulletDrawer();
private:
    QString name_;
};
