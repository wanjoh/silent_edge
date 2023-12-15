#include "game.hpp"

// pomeriti u gui
#include "qapplication.h"

Game::Game(QString name, QObject *parent)
    : QObject(parent)
    , client_(new Client())
    , gui_(new GameWindow())
    , logic_handler_(new GameLogicHandler(name))
{
    gui_->addEntity(name, logic_handler_->getPlayer()->getDrawer());


    connect(client_, &Client::signalDataReceived, this, &Game::updateEnemy, Qt::DirectConnection);

    connect(logic_handler_, &GameLogicHandler::playerMoved, this, &Game::playerMoved, Qt::DirectConnection);

    connect(logic_handler_, &GameLogicHandler::newBulletSignal, gui_, &GameWindow::addEntity);
    connect(logic_handler_, &GameLogicHandler::weaponDrawSignal, gui_, &GameWindow::addEntity);
    connect(logic_handler_, &GameLogicHandler::destroyBullet, gui_, &GameWindow::removeEntity);
    connect(gui_, &GameWindow::mousePos, logic_handler_, &GameLogicHandler::updateAimingPoint);
    connect(gui_, &GameWindow::keyPressedSignal, logic_handler_, &GameLogicHandler::updateKeys);
    connect(gui_, &GameWindow::focusedOutSignal, logic_handler_, &GameLogicHandler::resetKeys);
    connect(gui_, &GameWindow::mousePressedSignal, logic_handler_, &GameLogicHandler::updateMouseClick);
    connect(gui_, &GameWindow::wheelScrollSignal, logic_handler_, &GameLogicHandler::updateMouseScroll);
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
//    qDebug() << "slanje iz gejma, pos : " << player_->getDrawer()->pos();
}

