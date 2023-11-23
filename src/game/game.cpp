#include "game.hpp"

#include "qapplication.h"
#include "qgraphicsscene.h"
#include "qgraphicsview.h"

Game::Game(QObject *parent)
    :QObject(parent),
    client_(new Client())
{
    Q_UNUSED(parent);

    scene_ = new QGraphicsScene();
    scene_->setSceneRect(0, 0, 800, 600);

    view_ = new QGraphicsView();
    view_->setScene(scene_);
    view_->setBackgroundBrush(Qt::gray);

    connect(client_, &Client::signalDataReceived, this, std::bind(&Game::updateEnemy, this, std::placeholders::_1));
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
    startServer();
    client_->connectToServer(GameServer::HOST.toString(), GameServer::PORT);
    player_ = client_->getPlayer();
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
}

void Game::quit()
{
    delete this;

    QApplication::exit();
}

void Game::updateEnemy(Player *enemy)
{
    enemy_->setX(enemy->x());
    enemy_->setY(enemy->y());
    delete enemy;
}
