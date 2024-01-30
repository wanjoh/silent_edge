#include "bullet_drawer.hpp"

BulletDrawer::BulletDrawer(const QString &name,const QString &pixmap_path,
                           QGraphicsItem *parent):
    EntityDrawer(name,pixmap_path, parent)
{

}

BulletDrawer::~BulletDrawer()
= default;
