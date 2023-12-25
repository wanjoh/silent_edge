#include "game_logic_handler.hpp"
#include "server_config.hpp"
#include <QtMath>
#include <QVector2D>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

int bullet_id = 1;

GameLogicHandler::GameLogicHandler(Map *map, QObject* parent)
    : QObject(parent)
    , map_(map)
    , matrix_(map_->getMatrix())
    , rooms_(map_->getRooms())
{
    initializeTimers();
}

GameLogicHandler::~GameLogicHandler()
{
    for (auto& [_, player] : players_)
    {
        delete player;
    }
}

void GameLogicHandler::putPlayersIntoRooms()
{
    // todo: ovo je fobra funkcija, ako postoji neki lobby i igra se pokrece tek kad ima X igraca
    // inace nije jer trenutno igraci ulaze jedan po jedan i potrebno je zasebno stavljati svakog igraca u sobu
    auto player_it = players_.begin();
    for(auto it = rooms_.begin(); it != rooms_.end(); it++)
    {
        int n = it->second->getUnusedSpawnpoints().size();

        for(int i = 0; i < n && player_it != players_.end(); i++)
        {
            it->second->addPlayerToRoom(player_it->second);
            player_it++;
        }
    }
}

void GameLogicHandler::addBullet(int x, int y, const QString& name)
{
    QMutexLocker locker(&mutex_);

    QString bullet_name = QString::number(bullet_id);
    bullet_id++;
    Bullet* bullet = new Bullet(bullet_name);
    bullets_[name].push_back(bullet);

    QPointF top_left = players_[name]->getDrawer()->scenePos();
    QPointF top_right = players_[name]->getDrawer()->mapToScene(players_[name]->getDrawer()->pixmap().rect().topRight());
    QPointF top_center = (top_left + top_right) / 2;


    bullet->getDrawer()->setRotation(players_[name]->getDrawer()->rotation());
    bullet->getDrawer()->setPos(top_center.x(),top_center.y()-bullet->BULLET_HEIGHT);

   // todo: add bullet rotation
   // limun: evo
}

void GameLogicHandler::updatePlayerPosition(int x, int y, const QString& name)
{
    using namespace ServerConfig;
    bool moved = false;
    // todo: apdejtovati pomeraj na osnovu poslate komande od strane igraca
    // limun: važi, urađeno

    qreal dx = 0.0;
    qreal dy = 0.0;

    if(commands_[name] & PlayerActions::LEFT)
        dx -= 1.0;
    if(commands_[name] & PlayerActions::RIGHT)
        dx += 1.0;
    if(commands_[name] & PlayerActions::UP)
        dy -= 1.0;
    if(commands_[name] & PlayerActions::DOWN)
        dy += 1.0;

    if(qFabs(dx) < EPSILON && qFabs(dy) < EPSILON)
        return;

    if (qFabs(dx) > EPSILON && qFabs(dy) > EPSILON)
    {
        qreal length = qSqrt(dx * dx + dy * dy);
        dx /= length;
        dy /= length;
    }

    x += dx * DEFAULT_PLAYER_VELOCITY;
    y += dy * DEFAULT_PLAYER_VELOCITY;

    std::unordered_map<int, Tile*> active_buckets = map_->getActiveAmmoBuckets();
    QList<QGraphicsItem*> collidingItems = players_[name]->getDrawer()->collidingItems();
    for(auto item : collidingItems)
    {
        QGraphicsPixmapItem* pixmap_item = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(pixmap_item)
        {
            if(typeid(*pixmap_item) == typeid(TileDrawer))
            {
                TileDrawer *tile_drawer = dynamic_cast<TileDrawer*>(pixmap_item);
                int tile_id = tile_drawer->y()/IMAGE_SIZE * map_->getM() + tile_drawer->x()/IMAGE_SIZE;
                if(matrix_[tile_id]->getTileType() == Tile::TileType::WALL) {
                    return;
                }

                if(matrix_[tile_id]->getTileType() == Tile::TileType::AMMO_PILE) {
                    // limun: treba i povecati municiju player-u, to je na drugoj grani

                    QString path ="../silent-edge/src/images/ground.png";
                    map_->getDrawer()->change_picture(active_buckets[tile_id], path);
                    map_->removeFromActive(tile_id);


                    emit tileChanged(tile_id, path);
                }
            }
        }
    }

    players_[name]->getDrawer()->setPos(x, y);
}

QByteArray GameLogicHandler::jsonify(const QString& data_type)
{
    if(data_type == "player") {
        QJsonArray playersArray;

        for(auto& [name, player] : players_)
        {
            QJsonObject playerObject;
            playerObject["type"] = "player";
            playerObject["name"] = name;
            playerObject["position_x"] = player->getDrawer()->x();
            playerObject["position_y"] = player->getDrawer()->y();
            playerObject["rotation"] = player->getDrawer()->rotation();
            playerObject["hp"] = player->getHp();

            playersArray.append(playerObject);
        }

        const QJsonDocument json_data(playersArray);

        return json_data.toJson();
    }
    else if(data_type == "bullet") {
        QJsonArray bulletsArray;

        for(auto& [name, bullet_group] : bullets_)
        {
            for(auto& bullet : bullet_group) {
                QJsonObject bulletObject;
                bulletObject["type"] = "bullet";
                bulletObject["id"] = bullet_id;
                bulletObject["name"] = name;
                bulletObject["position_x"] = bullet->getDrawer()->x();
                bulletObject["position_y"] = bullet->getDrawer()->y();
                bulletObject["rotation"] = bullet->getDrawer()->rotation();

                bulletsArray.append(bulletObject);
            }
        }

        const QJsonDocument json_data(bulletsArray);

        return json_data.toJson();
    }
    else {
        return nullptr;
    }
}

void GameLogicHandler::updatePlayers()
{
    for (auto& [name, player] : players_)
    {
        qreal x = player->getDrawer()->x();//positions_[name].first;
        qreal y = player->getDrawer()->y();//positions_[name].second;

        // limun: rotacija, pozicija, meci
        updatePlayerRotation(x, y, name);
        updatePlayerPosition(x, y, name);
        if((commands_[name] & ServerConfig::PlayerActions::SHOOT))
            addBullet(x, y, name);
    }

    QByteArray player_info = jsonify("player");
    QByteArray bullet_info = jsonify("bullet");

    emit updatePlayersSignal(player_info);
    emit updateBulletsSignal(bullet_info);
}

void GameLogicHandler::updateAmmo()
{
    map_->restockAmmoPiles();
}

void GameLogicHandler::addPlayer(Player* playa)
{
    QString name = playa->getName();
    players_[name] = playa;
    commands_[name] = 0;
    mouse_positions_[name] = {0.0, 0.0};
}

void GameLogicHandler::removePlayer(QString name)
{
    delete players_[name];
    players_.erase(name);
    commands_.erase(name);
    mouse_positions_.erase(name);
}

void GameLogicHandler::updateBullets()
{
    QMutexLocker locker(&mutex_);
    for(auto &[name, bullet_group] : bullets_)
    {
        for(int i = bullet_group.size() - 1; i >= 0; i--) {
            qreal x_pos = bullet_group[i]->getDrawer()->x() + BULLET_SPEED * qSin(qDegreesToRadians(bullet_group[i]->getDrawer()->rotation()));
            qreal y_pos = bullet_group[i]->getDrawer()->y() - BULLET_SPEED * qCos(qDegreesToRadians(bullet_group[i]->getDrawer()->rotation()));

            bullet_group[i]->getDrawer()->setPos(x_pos, y_pos);

            if (checkCollisions(bullet_group[i]))
                delete bullet_group[i];
        }
    }
}

void GameLogicHandler::updatePlayerRotation(int x, int y, const QString& name)
{
    QPointF aiming_point = QPointF(mouse_positions_[name].first, mouse_positions_[name].second);
    if(players_[name]->getDrawer()->contains(players_[name]->getDrawer()->mapFromScene(aiming_point)))
        return;
    QPointF top_left = players_[name]->getDrawer()->scenePos();
    QPointF top_right = players_[name]->getDrawer()->mapToScene(players_[name]->getDrawer()->pixmap().rect().topRight());
    QPointF top_center = (top_left + top_right) / 2;
    QPointF aim_dir = aiming_point - top_center;

    if (aim_dir.manhattanLength() < 50)
        return;
    qreal angle = qAtan2(aim_dir.x(), -aim_dir.y());
    angle = qRadiansToDegrees(angle);

    bool rotated = qFabs(angle - players_[name]->getDrawer()->rotation()) > EPSILON;
    players_[name]->getDrawer()->setRotation(angle);
}

bool GameLogicHandler::checkCollisions(Bullet* bullet)
{
    // todo: deluje sumnjicavo, verovatno se moze optimizovati
    // ne toliko thread safe, jer se moze desiti da se podaci o igracima azuriraju kad se ovo pozove
    QList<QGraphicsItem*> colidingItems = bullet->getDrawer()->collidingItems();
    foreach(QGraphicsItem *item, colidingItems)
    {
        QGraphicsPixmapItem* pixmap_item = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(pixmap_item)
        {
            if(typeid(*pixmap_item) == typeid(PlayerDrawer))
            {
                PlayerDrawer *player_drawer = static_cast<PlayerDrawer*>(pixmap_item);
                QString player_name = player_drawer->name();

                decreaseHp(players_[player_name], bullet);

                qDebug() << player_name << " has " << players_[player_name]->getHp() << "hp";

                if(players_[player_name]->getHp() == 0)
                {
                    qDebug() << "igrac unisten";
                    removePlayer(player_name);
                    emit playerDestroyedSignal();
                }

                return true;
            }
            if(typeid(*pixmap_item) == typeid(TileDrawer))
            {
                TileDrawer *tile_drawer = static_cast<TileDrawer*>(pixmap_item);
                int tile_id = tile_drawer->y()/IMAGE_SIZE * map_->getM() + tile_drawer->x()/IMAGE_SIZE;
                if(matrix_[tile_id]->getTileType() == Tile::TileType::WALL)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void GameLogicHandler::decreaseHp(Player* player, Bullet* bullet)
{
    qreal player_hp = player->getHp();
    qreal bullet_damage = bullet->getDamageDealt();
    player->setHp(player_hp - bullet_damage);
}

void GameLogicHandler::initializeTimers()
{
    ammo_respawn_timer_.setInterval(AMMO_RESPAWN_TIME);
    connect(&ammo_respawn_timer_, &QTimer::timeout, this, &GameLogicHandler::updateAmmo);
    ammo_respawn_timer_.start();
}

void GameLogicHandler::updatePlayerStats(const QByteArray &player_info)
{
    // limun: evo
    // limun: 8 + 4 + 2 * 8 = 28 bajtova
    QJsonParseError parse_error;
    const QJsonDocument json_data = QJsonDocument::fromJson(player_info, &parse_error);
    if (parse_error.error == QJsonParseError::NoError && json_data.isObject())
    {
        QString name = json_data["name"].toString();
        if (commands_.find(name) == commands_.end())
        {
            Player* playa = new Player(name);
            addPlayer(playa);
            if (commands_.size() >= 2)
            {
                // todo: ovo je privremeni fix jer nemamo lobby
                // kad ubacimo lobby, ovo ce se vratiti u konstruktor
                putPlayersIntoRooms();
            }
        }
        commands_[name] = static_cast<quint32>(json_data["movement"].toInteger());
        qDebug() << commands_[name];
        //positions_[name] = QPair<int, int>(json_data["x"].toInteger(), json_data["y"].toInteger());
        mouse_positions_[name] = QPair<qreal, qreal>(json_data["mouseX"].toDouble(), json_data["mouseY"].toDouble());
    }
    else
    {
        // todo: handle? ili ne? nemamo vremena :(((
    }
}

