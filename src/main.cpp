//#include "gui/mainwindow.hpp"
#include "entity/player.hpp"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    // ovo sve ispod sigurno nece ici u main
    Player player;
    player.setBrush(Qt::green);
    player.setRect(Player::INITIAL);
    player.setFlag(QGraphicsItem::ItemIsFocusable);
    player.setFocus();


    // Player enemy(deserialize(server_enemy_info))
    Player enemy;
    enemy.setBrush(Qt::red);
    enemy.setRect(Player::INITIAL);

    QGraphicsScene scene;
    scene.setSceneRect(0, 0, 600, 400);
    scene.addItem(&player);
    scene.addItem(&enemy);

    QGraphicsView view;
    view.setScene(&scene);
    view.setBackgroundBrush(Qt::gray);
    view.show();

    return a.exec();
}
