#include "gui/mainwindow.hpp"
#include "game/game.hpp"
#include "qapplication.h"
#include <QKeyEvent>
#include <QThread>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
        MainWindow w;
        w.show();

    Game *game = new Game();
    game->startGame();
    game->moveToThread(new QThread);
    return a.exec();
}
