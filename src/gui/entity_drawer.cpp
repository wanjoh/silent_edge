#include "entity_drawer.hpp"

EntityDrawer::EntityDrawer(QString name,const QString& pixmap_path,
                           QGraphicsItem* parent):
    QGraphicsPixmapItem(QPixmap(pixmap_path), parent)
{
    name_ = name;
}

QString EntityDrawer::name() const
{
    return name_;
}
