#include "game.hpp"

// pomeriti u gui
#include "qapplication.h"
#include "../server/server_config.hpp"
#include <QJsonDocument>
#include <QJsonArray>

Game::Game(const QString& name,Client* client, QObject *parent)
    : QObject(parent)
    , client_(client)
    , map_(new Map())
    , player_(new Player(name, false))
{
    Room *room = map_->findRoomForPlayer(*player_);
    gui_ = new GameWindow(room);
    gui_->addItem(map_->getDrawer()->get_group());
    json_object_["name"] = name;
    gui_->addEntity(name, player_->getDrawer());
    gui_->addEntity(player_->getMeleeWeapon()->getDrawer()->name(), player_->getMeleeWeapon()->getDrawer());
    gui_->addEntity(player_->getReload()->getDrawer()->name(), player_->getReload()->getDrawer());

    connect(client_, &Client::serverTickReceived, this, &Game::serializeData, Qt::DirectConnection);
    connect(client_, &Client::dataReceived, this, &Game::deserializeData, Qt::DirectConnection);
}

Game::~Game()
{

}

void Game::startGame(const QString& server_ip)
{
   client_->connectToServer(server_ip, ServerConfig::PORT);

   gui_->show(GameWindow::GamePhase::FIGHT_PHASE);

}

void Game::startServer(const QString& server_ip)
{
    server_ = new GameServer(server_ip);
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

    if (parse_error.error == QJsonParseError::NoError)
    {
        if(json_data.isArray()) {
            QJsonArray array = json_data.array();
            QJsonObject object = array.first().toObject();

            if(object["type"].toString() == "player") {
                for(const auto& value : array) {
                    QJsonObject playerObject = value.toObject();

                    QString this_name = playerObject["name"].toString();
                    qreal x = playerObject["position_x"].toDouble();
                    qreal y = playerObject["position_y"].toDouble();
                    qreal rotation = playerObject["rotation"].toDouble();
                    qreal hp = playerObject["hp"].toDouble();
                    bool is_swinging = playerObject["swinging"].toBool();
                    bool is_reloading = playerObject["reloading"].toBool();
                    bool is_shooting = playerObject["shooting"].toBool();
                    qint32 bullet_count = playerObject["bullet_count"].toInt();
                    qint32 remaining_bullets = playerObject["remaining_bullets"].toInt();
                    quint32 logic_events = playerObject["logic_events"].toInt();
                    bool camera_change = (logic_events & LogicEvents::CAMERA) != 0u;
                    bool restock = (logic_events & LogicEvents::RESTOCK) != 0u;
                    bool tilechange = (logic_events & LogicEvents::TILECHANGE) != 0u;
                    int tile_id = playerObject["tile_id"].toInt();

                    if(player_->getName() == this_name) {
                        player_->getDrawer()->setPos(x, y);
                        player_->getDrawer()->setRotation(rotation);
                        player_->getMeleeWeapon()->getDrawer()->setPos(x+IMAGE_SIZE/2, y+IMAGE_SIZE/2);
                        player_->getMeleeWeapon()->getDrawer()->setRotation(rotation);
                        player_->getReload()->getDrawer()->setPos(x, y);
                        player_->setHp(hp);
                        gui_->update_hp_overlay(hp);

                        if (is_shooting) {
                            gui_->updateBulletsLabel(player_->getRangedWeapon()->getCapacity()
                                                         - bullet_count,
                                                     player_->getRangedWeapon()->getCapacity(),
                                                     remaining_bullets);
                        }
                        if (!player_->getReloadTimer()->isActive()) {
                            gui_->updateBulletsLabel(player_->getRangedWeapon()->getCapacity()
                                                         - bullet_count,
                                                     player_->getRangedWeapon()->getCapacity(),
                                                     remaining_bullets);
                        }

                        if (is_swinging) {
                            player_->getMeleeWeapon()->getDrawer()->setZValue(5);
                        } else {
                            player_->getMeleeWeapon()->getDrawer()->setZValue(-1);
                        }

                        if (is_reloading) {
                            player_->getReload()->getDrawer()->setZValue(7);
                        } else {
                            player_->getReload()->getDrawer()->setZValue(-1);
                        }

                        if(camera_change) {
                            player_->getDrawer()->setPos(x, y);
                            Room *room = map_->findRoomForPlayer(*player_);
                            gui_->changeRoom(room);
                            gui_->teleportPlayer(player_->getName(), player_->getDrawer()->x(), player_->getDrawer()->y());
                        }
                        if (restock) {
                            map_->restockAmmoPiles();
                        }
                    }
                    else
                    {
                        if(!enemies_.contains(this_name)) {
                            enemies_[this_name] = new Player(this_name);
                            gui_->addEntity(this_name, enemies_[this_name]->getDrawer());
                            gui_->addEntity(enemies_[this_name]->getMeleeWeapon()->getDrawer()->name(), enemies_[this_name]->getMeleeWeapon()->getDrawer());
                            gui_->addEntity(enemies_[this_name]->getReload()->getDrawer()->name(), enemies_[this_name]->getReload()->getDrawer());
                        }
                        enemies_[this_name]->getDrawer()->setPos(x, y);
                        enemies_[this_name]->getDrawer()->setRotation(rotation);
                        enemies_[this_name]->getMeleeWeapon()->getDrawer()->setPos(x+IMAGE_SIZE/2, y+IMAGE_SIZE/2);
                        enemies_[this_name]->getMeleeWeapon()->getDrawer()->setRotation(rotation);
                        enemies_[this_name]->getReload()->getDrawer()->setPos(x, y);
                        enemies_[this_name]->setHp(hp);

                        if (is_swinging) {
                            enemies_[this_name]->getMeleeWeapon()->getDrawer()->setZValue(5);
                        } else {
                            enemies_[this_name]->getMeleeWeapon()->getDrawer()->setZValue(-1);
                        }

                        if (is_reloading) {
                            enemies_[this_name]->getReload()->getDrawer()->setZValue(7);
                        } else {
                            enemies_[this_name]->getReload()->getDrawer()->setZValue(-1);
                        }
                    }

                    if(tilechange) {
                        map_->getDrawer()->change_picture(tile_id, "../silent-edge/src/images/ground.png");
                        map_->removeFromActive(tile_id);
                    }
                }
            } else if (object["type"].toString() == "bullet")
            {
                for(const auto& value : array) {
                    QJsonObject bulletObject = value.toObject();

                    int id = bulletObject["id"].toInt();
                    QString owner_name = bulletObject["owner_name"].toString();
                    qreal x = bulletObject["position_x"].toDouble();
                    qreal y = bulletObject["position_y"].toDouble();
                    qreal rotation = bulletObject["rotation"].toDouble();


                    if(!bullet_drawers_.contains(id))
                    {
                        Bullet *bullet = new Bullet(id, owner_name);
                        bullet_drawers_[id] = bullet->getDrawer();
                        gui_->addEntity(bullet_drawers_[id]->name(), bullet_drawers_[id]);
                    }

                    if(bullet_drawers_[id]->x() == x && bullet_drawers_[id]->y() == y)
                    {
                        qDebug() << "erasing bullet: " << id;
                        bullet_drawers_.erase(id);
                        gui_->removeEntity(QString::number(id));
                    }
                    else
                    {
                        bullet_drawers_[id]->setPos(x, y);
                        bullet_drawers_[id]->setRotation(rotation);
                    }
                }
            }
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
    json_object_["mouse_x"] = QJsonValue(static_cast<qreal>(gui_->getMouseX()));
    json_object_["mouse_y"] = QJsonValue(static_cast<qreal>(gui_->getMouseY()));

    client_->sendMessage(QJsonDocument(json_object_).toJson());
}

