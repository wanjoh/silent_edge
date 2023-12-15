#pragma once

#include <QGraphicsPixmapItem>

class EntityDrawer : public QGraphicsPixmapItem
{
public:
    EntityDrawer(QString name,const QString &pixmap_path = "path_to_pixmap",
                 QGraphicsItem * = nullptr);

    QString name() const;

private:
    QString name_;
};
