#pragma once

#include <QGraphicsRectItem>
#include "../../src/serialization/serializable.hpp"

class Entity : public QGraphicsRectItem, public Serializable
{
public:
    using QGraphicsRectItem::QGraphicsRectItem;
    void updatePixmap(QString);
    QVariant toVariant() const override;
    void fromVariant(const QVariant &) override;
    ~Entity();
private:
    QString pixmap_path_;
};
