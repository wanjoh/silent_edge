#pragma once

#include "qgraphicsview.h"
#include "qobject.h"

class Game : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Game)
public:
    explicit Game(QObject *parent = nullptr);
    ~Game();
    void startGame();
    void startServer();
    void show();
    void quit();

    QGraphicsView *view_;
    QGraphicsScene *scene_;

private:
};
