#pragma once

#include <QGraphicsView>

class Game : public QGraphicsView
{
public:
    using QGraphicsView::QGraphicsView;
//    Game();
    void startGame();
private:
    QGraphicsScene scene_;
};
