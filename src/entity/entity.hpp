#pragma once

#include <QGraphicsRectItem>

class Entity : public QGraphicsRectItem
{
public:
    using QGraphicsRectItem::QGraphicsRectItem;
    void updatePixmap(QString path);
private:
    QString pixmap_path_;
};
