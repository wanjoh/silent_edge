#include "game.hpp"

// pomeriti u gui
#include "qapplication.h"
#include "../server/server_config.hpp"

Game::Game(QString name, QObject *parent)
    : QObject(parent)
    , client_(new Client())
    , map_(new Map())
    , player_(new Player(name, false))
{
    Room room = *new Room();
    gui_ = new GameWindow(map_->getDrawer(), &room);

    connect(client_, &Client::serverTickReceived, client_,
            std::bind(&Client::sendMessage, client_, player_->getName(), gui_->getMovement()));
}

Game::~Game()
{

}

void Game::startGame()
{
    startServer();
    client_->connectToServer(ServerConfig::getHost().toString(), ServerConfig::PORT);
    gui_->show(GameWindow::GamePhase::FIGHT_PHASE);
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


