#include "game.hpp"

// pomeriti u gui
#include "qapplication.h"
#include "../server/server_config.hpp"

Game::Game(QString name, QObject *parent)
    : QObject(parent)
    , client_(new Client())
    , map_(new Map())
    , player_(new Player("name", false))
{
    Room room = *new Room();
    gui_ = new GameWindow(map_->getDrawer(), &room);

    // todo: naci nacin kako ce se prebaciti podaci i slati ih samo jednom po server tick-u
    // bilo bi dobro da se iskoristi playerACtions ili bilo kakav bitset za akcije, a jedan qreal za rotaciju
    // ili umesto rotacije mozemo prosledjivati poziciju kursora (svakako sve racunamo na strani servera)

    // limun: okej
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


