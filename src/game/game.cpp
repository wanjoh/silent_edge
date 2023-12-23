#include "game.hpp"

// pomeriti u gui
#include "qapplication.h"
#include "../server/server_config.hpp"
#include <QJsonDocument>

Game::Game(QString name, QObject *parent)
    : QObject(parent)
    , client_(new Client())
    , map_(new Map())
    , player_(new Player(name, false))
{
    Room *room = map_->addPlayerToARoom(player_);
    gui_ = new GameWindow(map_->getDrawer(), room);
    json_object_["name"] = name;

    connect(client_, &Client::serverTickReceived, this, &Game::serializeData, Qt::DirectConnection);
    connect(client_, &Client::dataReceived, this, &Game::deserializeData, Qt::DirectConnection);
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

void Game::deserializeData(const QByteArray &data)
{
    QJsonParseError parse_error;
    const QJsonDocument json_data = QJsonDocument::fromJson(data, &parse_error);
    if (parse_error.error == QJsonParseError::NoError && json_data.isObject())
    {


    }
    else
    {
        // todo: handle? ili ne? nemamo vremena :(((
    }

}

void Game::serializeData()
{
    json_object_["movement"] = QJsonValue(static_cast<qint64>(gui_->getMovement()));
//    json_object_["mouse_x"] = gui_->getMouseX();
//    json_object_["mouse_y"] = gui_->getMouseY();

    client_->sendMessage(QJsonDocument(json_object_).toJson());
}


