#include "game.hpp"

// pomeriti u gui
#include "qapplication.h"

Game::Game(QString name, QObject *parent)
    : QObject(parent)
    , client_(new Client())
    , gui_()
    , logic_handler_()
    , map_(new Map())
{
    logic_handler_ = new GameLogicHandler(name, map_);
    gui_ = new GameWindow(map_, logic_handler_->getPlayer()->getDrawer());
    gui_->addEntity(name, logic_handler_->getPlayer()->getDrawer());


    connect(client_, &Client::signalDataReceived, logic_handler_, &GameLogicHandler::recognizeEntityType, Qt::DirectConnection);
    connect(client_, &Client::signalTileNameReceived, this, &Game::updateMap, Qt::DirectConnection);
    
    connect(logic_handler_, &GameLogicHandler::tileDeleted, this, &Game::tileDeleted, Qt::DirectConnection);
    connect(logic_handler_, &GameLogicHandler::update, gui_, &GameWindow::addEntity);
    connect(logic_handler_, &GameLogicHandler::playerMoved, this, &Game::playerMoved, Qt::DirectConnection);
    connect(logic_handler_, &GameLogicHandler::bulletMoved, this, &Game::bulletMoved, Qt::DirectConnection);

    connect(logic_handler_, &GameLogicHandler::newBulletSignal, gui_, &GameWindow::addEntity);
    connect(logic_handler_, &GameLogicHandler::destroyBullet, gui_, &GameWindow::removeEntity);
    connect(logic_handler_, &GameLogicHandler::destroyPlayer, gui_, &GameWindow::removeEntity);
    connect(gui_, &GameWindow::mousePos, logic_handler_, &GameLogicHandler::updateAimingPoint);
    connect(gui_, &GameWindow::keyPressedSignal, logic_handler_, &GameLogicHandler::updateKeys);
    connect(gui_, &GameWindow::focusedOutSignal, logic_handler_, &GameLogicHandler::resetKeys);
    connect(gui_, &GameWindow::mousePressedSignal, logic_handler_, &GameLogicHandler::updateMouseClick);

    connect(logic_handler_, &GameLogicHandler::weapon_changed, gui_, &GameWindow::change_weapon);
    connect(logic_handler_, &GameLogicHandler::update_hp, gui_, &GameWindow::update_hp_overlay);
}

Game::~Game()
{
    for (auto& [_, enemy] : enemies_)
    {
        delete enemy;
    }
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

void Game::bulletMoved(QVariant variant)
{
    client_->sendMessage(variant);
}

void Game::updateMap(QVariant variant)
{
    int id = variant.toInt();
    QPair<int, int> coords = map_->get_matrix()[id]->get_coords();
    map_->remove_tile(id);
    map_->add_ground_tile_of_type_ammo(id, coords.first, coords.second);
}

void Game::tileDeleted(int id)
{
    client_->sendMessage(QVariant(id));
}


