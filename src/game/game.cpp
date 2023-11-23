#include "game.hpp"

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
    client_.connectToServer(GameServer::HOST.toString(), GameServer::PORT);
    player_ = client_.getPlayer();
    player_->setBrush(Qt::green);
    player_->setRect(Player::INITIAL);
    player_->setFlag(QGraphicsItem::ItemIsFocusable);
    player_->setFocus();

    enemy_ = new Player("enemy");
    enemy_->setBrush(Qt::red);
    enemy_->setRect(Player::INITIAL);

    scene_->addItem(player_);
    scene_->addItem(enemy_);
}

void Game::startServer()
{
    server_ = new GameServer();

    startGame();
}

void Game::quit()
{
    delete this;

    QApplication::exit();
}
