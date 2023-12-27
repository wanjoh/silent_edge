#include "entity_drawer.hpp"

EntityDrawer::EntityDrawer(const QString& name, const QString& pixmap_path,
                           QGraphicsItem* parent):
    QGraphicsPixmapItem(QPixmap(pixmap_path), parent),
    name_(name)
{
    setTransformOriginPoint(pixmap().rect().center());
}

QString EntityDrawer::name() const
{
    return name_;
}

void EntityDrawer::setName(const QString &name)
{
    name_ = name;
}
