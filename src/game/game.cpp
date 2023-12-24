#include "game.hpp"

// pomeriti u gui
#include "qapplication.h"
#include "../server/server_config.hpp"
#include <QJsonDocument>
#include <QJsonArray>

Game::Game(QString name, QObject *parent)
    : QObject(parent)
    , client_(new Client())
    , map_(new Map())
    , player_(new Player(name, false))
{
    Room *room = map_->addPlayerToARoom(*player_);
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

    qDebug() << player_->getName() << ": " << player_->getDrawer()->x() << " " << player_->getDrawer()->y();
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
    std::map<QString, Player*> players;

    QJsonParseError parse_error;
    const QJsonDocument json_data = QJsonDocument::fromJson(data, &parse_error);

    if (parse_error.error == QJsonParseError::NoError && json_data.isArray())
    {
        QJsonArray array = json_data.array();

        for(const auto& value : array) {
            QJsonObject playerObject = value.toObject();

            QString this_name = playerObject["name"].toString();
            qreal x = playerObject["position_x"].toDouble();
            qreal y = playerObject["position_y"].toDouble();
            qreal rotation = playerObject["rotation"].toDouble();
            qreal hp = playerObject["hp"].toDouble();

            if(player_->getName() == this_name) {
                player_->getDrawer()->setPos(x, y);
                player_->getDrawer()->setRotation(rotation);
                player_->setHp(hp);
            }
            else if(enemies_.contains(this_name))
            {
                enemies_[this_name]->getDrawer()->setPos(x, y);
                enemies_[this_name]->getDrawer()->setRotation(rotation);
                enemies_[this_name]->setHp(hp);
            }

            qDebug() << player_->getName() << ": " << player_->getDrawer()->x() << " " << player_->getDrawer()->y();
        }
    }
    else
    {
        // todo: handle? ili ne? nemamo vremena :(((
    }
}

void Game::serializeData()
{
    json_object_["movement"] = QJsonValue(static_cast<qint64>(gui_->getMovement()));
    json_object_["mouse_x"] = gui_->getMouseX();
    json_object_["mouse_y"] = gui_->getMouseY();

    client_->sendMessage(QJsonDocument(json_object_).toJson());
}


