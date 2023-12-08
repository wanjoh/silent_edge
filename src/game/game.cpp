#include "game.hpp"

// pomeriti u gui
#include "qapplication.h"

Game::Game(QString name, QObject *parent)
    : QObject(parent)
    , client_(new Client())
    , gui_(new GameWindow())
    , player_(new Player(name, false))
{
    logic_handler_ = new GameLogicHandler(player_);
    gui_->addEntity(player_->getName(), player_->getDrawer());

    connect(client_, &Client::signalDataReceived, this, &Game::updateEnemy, Qt::DirectConnection);

    connect(logic_handler_, &GameLogicHandler::playerMoved, this, &Game::playerMoved, Qt::DirectConnection);

    connect(gui_, &GameWindow::keyPressedSignal, logic_handler_, &GameLogicHandler::updateKeys);
    connect(gui_, &GameWindow::focusedOutSignal, logic_handler_, &GameLogicHandler::resetKeys);
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
    qDebug() << "primljeni podaci za: " << enemy->getName() << ": " << enemy->getDrawer()->pos();

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

void Game::playerMoved()
{
    client_->sendMessage(player_->toVariant());
//    qDebug() << "slanje iz gejma, pos : " << player_->getDrawer()->pos();
}

