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

    connect(client_, &Client::signalDataReceived, this, &Game::updateEnemy, Qt::DirectConnection);
    connect(client_, &Client::signalTileNameReceived, this, &Game::updateMap, Qt::DirectConnection);

    connect(logic_handler_, &GameLogicHandler::playerMoved, this, &Game::playerMoved, Qt::DirectConnection);
    connect(logic_handler_, &GameLogicHandler::tileDeleted, this, &Game::tileDeleted, Qt::DirectConnection);

    connect(logic_handler_, &GameLogicHandler::newBulletSignal, gui_, &GameWindow::addEntity);
    connect(logic_handler_, &GameLogicHandler::destroyBullet, gui_, &GameWindow::removeEntity);
    connect(logic_handler_, &GameLogicHandler::destroyPlayer, gui_, &GameWindow::removeEntity);
    connect(gui_, &GameWindow::mousePos, logic_handler_, &GameLogicHandler::updateAimingPoint);
    connect(gui_, &GameWindow::keyPressedSignal, logic_handler_, &GameLogicHandler::updateKeys);
    connect(gui_, &GameWindow::focusedOutSignal, logic_handler_, &GameLogicHandler::resetKeys);
    connect(gui_, &GameWindow::mousePressedSignal, logic_handler_, &GameLogicHandler::updateMouseClick);
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

void Game::updateEnemy(QVariant variant)
{
    Player *enemy = new Player("enemy");
    enemy->fromVariant(variant);
    QString enemy_name = enemy->getName();
//    qDebug() << "primljeni podaci za: " << enemy->getName() << ": " << enemy->getDrawer()->pos();

    // ŠTA JE OVO? Da li mi brišemo neprijatelja svaki put kada se pozove ova funkcija?
    if (enemies_.find(enemy_name) == enemies_.end())
    {
        enemies_[enemy_name] = enemy;
        gui_->addEntity(enemy_name, enemy->getDrawer());
    }
    else
    {
        delete enemy;
        enemies_[enemy_name]->fromVariant(variant);
    }

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

void Game::tileDeleted(QString name)
{
    client_->sendMessage(QVariant(name));
}

