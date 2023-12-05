#include "gui/mainwindow.hpp"
#include "game/game.hpp"
#include "qapplication.h"
#include <QThread>
#include <QRandomGenerator>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QRandomGenerator q(QDateTime::currentMSecsSinceEpoch());
    QString name = QString("playa") + QString::number(q.generate());
    qDebug() << "ime: " << name;
    Game *game = new Game(name);
    game->startGame();
    game->moveToThread(new QThread);
    return a.exec();
}
