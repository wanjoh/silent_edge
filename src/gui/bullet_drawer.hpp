#pragma once
#include "entity_drawer.hpp"

class BulletDrawer : public EntityDrawer
{
public:
    BulletDrawer(QString name, const QString &pixmap_path = "path_to_pixmap",
                 QGraphicsItem * = nullptr);

private:
    QString name_;
};
