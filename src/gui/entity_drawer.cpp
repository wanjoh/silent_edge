#include "entity_drawer.hpp"

EntityDrawer::EntityDrawer(QString name,const QString& pixmap_path,
                           QGraphicsItem* parent):
    QGraphicsPixmapItem(QPixmap(pixmap_path), parent)
{
    name_ = name;
    setTransformOriginPoint(pixmap().width() / 2, pixmap().height() / 2);
}

QString EntityDrawer::name() const
{
    return name_;
}

void EntityDrawer::setName(const QString &name)
{
    name_ = name;
}
