#pragma once

#include <QGraphicsPixmapItem>

class EntityDrawer : public QGraphicsPixmapItem
{
public:
    EntityDrawer(QString name,const QString &pixmap_path = "path_to_pixmap",
                 QGraphicsItem * = nullptr);

    QString name() const;

    void setName(const QString &name);

private:
    QString name_;
};
