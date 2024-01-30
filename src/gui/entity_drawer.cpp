#include "entity_drawer.hpp"

#include <utility>

EntityDrawer::EntityDrawer(const QString &name, const QString &pixmap_path, QGraphicsItem *parent)
    : QGraphicsPixmapItem(QPixmap(pixmap_path), parent)
    , name_(std::move(name))
{
    setTransformOriginPoint(pixmap().rect().center());
}

QString EntityDrawer::getName()
{
    return name_;
}

void EntityDrawer::setName(const QString &name)
{
    name_ = name;
}
