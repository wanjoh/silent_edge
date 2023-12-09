#include "game.hpp"

// pomeriti u gui
#include "qapplication.h"

Game::Game(QString name, QString map_path, QObject *parent)
    :QObject(parent),
    client_(new Client()),
    player_(new Player(name, false)),
    map_(new Map(&map_path))
{
    gui_ = new GameWindow(map_->draw_matrix(&map_path), player_->getDrawer(), map_->get_matrix(&map_path));
    qDebug() << map_->get_matrix(&map_path);
    connect(client_, &Client::signalDataReceived,
            this, std::bind(&Game::updateEnemy, this, std::placeholders::_1), Qt::DirectConnection);
    connect(gui_, &GameWindow::playerMoved, this, &Game::playerMoved, Qt::DirectConnection);
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
        // ovo je jako lose, ali trenutna serijalizacija je jos gora tako da nema veze:)
        delete enemy;
        enemies_[enemy_name]->fromVariant(variant);
    }

}

void Game::playerMoved()
{
    client_->sendMessage(player_->toVariant());
//    qDebug() << "slanje iz gejma, pos : " << player_->getDrawer()->pos();
}

