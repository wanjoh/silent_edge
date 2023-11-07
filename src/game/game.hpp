#pragma once

#include <QGraphicsView>

class Game : public QGraphicsView
{
public:
    using QGraphicsView::QGraphicsView;
    void startGame();
private:
    QGraphicsScene scene_;
};
