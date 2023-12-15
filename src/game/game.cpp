#include "game.hpp"

// pomeriti u gui
#include "qapplication.h"

Game::Game(QString name, QObject *parent)
    : QObject(parent)
    , client_(new Client())
    , map_(new Map())
{
    gui_ = new GameWindow(map_);
    // todo: naci nacin kako ce se prebaciti podaci i slati ih samo jednom po server tick-u
    // bilo bi dobro da se iskoristi playerACtions ili bilo kakav bitset za akcije, a jedan qreal za rotaciju
    // ili umesto rotacije mozemo prosledjivati poziciju kursora (svakako sve racunamo na strani servera)
}


void Game::startGame()
{
    startServer();
    client_->connectToServer(GameServer::HOST.toString(), GameServer::PORT);
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

void Game::playerMoved(QVariant variant)
{
    client_->sendMessage(variant);
}

void Game::updateMap(QVariant variant)
{
    QString name = variant.toString();
    QPair<int, int> coords = map_->get_matrix()[name]->get_coords();
    map_->remove_tile(name);
    map_->add_ground_tile_of_type_ammo(name, coords.first, coords.second);
}


