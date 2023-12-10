#include "game.hpp"

// pomeriti u gui
#include "qapplication.h"

Game::Game(QString name, QObject *parent)
    :QObject(parent),
    client_(new Client()),
    player_(new Player(name, false)),
    map_(new Map())
{
    gui_ = new GameWindow(map_, player_->getDrawer());
    addAllDynamicTiles();
    connect(client_, &Client::signalDataReceived,
            this, std::bind(&Game::updateEnemy, this, std::placeholders::_1), Qt::DirectConnection);
    connect(client_, &Client::signalTileNameReceived,
            this, std::bind(&Game::updateMap, this, std::placeholders::_1), Qt::DirectConnection);
    connect(gui_, &GameWindow::playerMoved, this, &Game::playerMoved, Qt::DirectConnection);
    connect(gui_, &GameWindow::tileDeleted, this, &Game::tileDeleted, Qt::DirectConnection);
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

void Game::addAllDynamicTiles()
{
    for (auto& spawnpoint : map_->get_spawnpoints())
        gui_->addTile(spawnpoint.first, spawnpoint.second);

    for (auto& ammo_pile : map_->get_ammo_piles())
        gui_->addTile(ammo_pile.first, ammo_pile.second);
}

void Game::updateEnemy(QVariant variant)
{
    Player *enemy = new Player("enemy");
    enemy->fromVariant(variant);
    QString enemy_name = enemy->getName();
    qDebug() << "primljeni podaci za: " << enemy->getName() << ": " << enemy->getDrawer()->pos();

    // ŠTA JE OVO? Da li mi brišemo neprijatelja svaki put kada se pozove ova funkcija?
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

void Game::updateMap(QVariant variant)
{
    QString name = variant.toString();
    map_->remove_name_from_ammo_list(name);
    gui_->deleteTile(name);
}

void Game::playerMoved()
{
    client_->sendMessage(player_->toVariant());
//    qDebug() << "slanje iz gejma, pos : " << player_->getDrawer()->pos();
}

void Game::tileDeleted(QString name)
{
    client_->sendMessage(QVariant(name));
}

