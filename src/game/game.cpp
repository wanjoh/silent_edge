#include "game.hpp"

// pomeriti u gui
#include "qapplication.h"

Game::Game(QString name, QObject *parent)
    : QObject(parent)
    , client_(new Client())
    , map_(new Map())
    , player_(new Player("test", false))
{
    // limun: samo da vidim šta radi a šta ne
    EntityDrawer *player_drawer = player_->getDrawer();
    player_drawer->setPos(2*IMAGE_SIZE, 2*IMAGE_SIZE);
    Room *room_ = map_->add_player_to_a_room(player_drawer);
    gui_ = new GameWindow(map_->get_drawer(), room_);
    gui_->addItem(player_drawer);
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

void Game::playerMoved(QVariant variant)
{
    client_->sendMessage(variant);
}

void Game::updateMap(QVariant variant)
{
    // limun: ne treba ovo ovako, kasnije ću
    int id = variant.toInt();
    map_->remove_from_active(id);
    map_->get_drawer()->change_picture(map_->get_matrix().at(id), "");
}


