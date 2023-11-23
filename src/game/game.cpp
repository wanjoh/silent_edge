#include "game.hpp"

#include "../client/client.hpp"
#include "../server/server.hpp"
#include "qapplication.h"
#include "qgraphicsscene.h"
#include "qgraphicsview.h"

Game::Game(QObject *parent)
    :QObject(parent)
{
    Q_UNUSED(parent);

    scene_ = new QGraphicsScene();
    scene_->setSceneRect(0, 0, 800, 600);

    view_ = new QGraphicsView();
    view_->setScene(scene_);
    view_->setBackgroundBrush(Qt::gray);
}

Game::~Game()
{

}

void Game::show()
{
    view_->show();
}

void Game::startGame()
{
    Client client;
    client.connectToServer(GameServer::HOST.toString(), GameServer::PORT);
    Player *player = client.getPlayer();
    player->setBrush(Qt::green);
    player->setRect(Player::INITIAL);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();

    Player *enemy = new Player("enemy");
    enemy->setBrush(Qt::red);
    enemy->setRect(Player::INITIAL);

    scene_->addItem(player);
    scene_->addItem(enemy);
}

void Game::startServer()
{
    GameServer server;

    startGame();
}

void Game::quit()
{
    delete this;

    QApplication::exit();
}
