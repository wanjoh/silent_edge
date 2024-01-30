#pragma once

#include <QGraphicsPixmapItem>

class EntityDrawer : public QGraphicsPixmapItem
{
public:
    EntityDrawer(const QString &name,
                 const QString &pixmap_path = "path_to_pixmap",
                 QGraphicsItem * = nullptr);

    void setName(const QString &name);
    QString getName();

private:
    QString name_;
};
