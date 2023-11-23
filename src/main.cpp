//#include "gui/mainwindow.hpp"
#include "game/game.hpp"
#include "qapplication.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //    MainWindow w;
    //    w.show();

    Game *game = new Game();
    game->show();
    game->startServer();

    return a.exec();
}
