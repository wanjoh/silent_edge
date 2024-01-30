#include "gui/mainwindow.hpp"
#include "game/game.hpp"
#include "qapplication.h"
#include <QThread>
#include <QRandomGenerator>
#include <QTime>

auto main(int argc, char *argv[]) -> int
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /*QRandomGenerator q(QDateTime::currentMSecsSinceEpoch());
    QString name = QString("playa") + QString::number(q.generate());
    qDebug() << "ime: " << name;
    Game *game = new Game(name);
    game->startGame();
    game->moveToThread(new QThread);*/
    return QApplication::exec();
}
