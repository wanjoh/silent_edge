#pragma once

#include <QGraphicsRectItem>
#include "../../src/serialization/serializable.hpp"

class Entity : public QGraphicsRectItem, public Serializable, public QObject
{
public:
    using QGraphicsRectItem::QGraphicsRectItem;
    void updatePixmap(QString);
    ~Entity();
private:
    QString pixmap_path_;
};
