#pragma once

#include <QGraphicsRectItem>

class EntityDrawer : public QGraphicsRectItem
{
public:
    // izbaciti ovo kad ubacimo pixmape i izmeniti klasu
    static constexpr QRect DEFAULT_RECT = QRect(0, 0, 50, 50);
    static constexpr qreal DEFAULT_VELOCITY = 5.0f;
    EntityDrawer(Qt::GlobalColor = Qt::red, const QRect& = DEFAULT_RECT,
                 const QString& = "path_to_pixmap", qreal = DEFAULT_VELOCITY,
                 QGraphicsItem* = nullptr);
    qreal getVelocity();
private:
    QPixmap* pixmap_;
    qreal velocity_;
};
