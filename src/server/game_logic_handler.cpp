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
    , results_(new Results)
{
    initializeTimers();


    rangList();
}

GameLogicHandler::~GameLogicHandler()
{
    for (auto& [_, player] : players_)
    {
        delete player;
    }
}

void GameLogicHandler::putPlayerIntoRoom(const QString &name)
{
    for(auto it = rooms_.begin(); it != rooms_.end(); it++)
    {
        int n = it->second->getUnusedSpawnpoints().size();

        if(n == 0)
            continue;

        it->second->addPlayerToRoom(players_[name]);

        QJsonObject object;
        object["type"] = "refresh_camera";
        object["name"] = name;
        object["x"] = players_[name]->getDrawer()->x();
        object["y"] = players_[name]->getDrawer()->y();
        const QJsonDocument json_data(object);

        QByteArray refresh_info = json_data.toJson();

        emit sendRefreshCameraSignal(refresh_info);

        return;
    }
}

void GameLogicHandler::removePlayerFromRoom(const QString &name)
{
    for(auto it = rooms_.begin(); it != rooms_.end(); it++)
    {
        int n = it->second->getUnusedSpawnpoints().size();

        if(n == 0)
            continue;

        if(it->second->getPlayersInRoom().contains(players_[name])) {
            it->second->removePlayerFromRoom(players_[name]);
            return;
        }
    }
}

void GameLogicHandler::addBullet(int x, int y, const QString& name)
{
    QMutexLocker locker(&mutex_);

    Bullet* bullet = new Bullet(bullet_id, name);

    QPointF top_left = players_[name]->getDrawer()->scenePos();
    QPointF top_right = players_[name]->getDrawer()->mapToScene(players_[name]->getDrawer()->pixmap().rect().topRight());
    QPointF top_center = (top_left + top_right) / 2;

    bullet->getDrawer()->setRotation(players_[name]->getDrawer()->rotation());
    bullet->getDrawer()->setPos(top_center.x(), top_center.y()-bullet->BULLET_HEIGHT);

    bullets_[bullet_id] = bullet;

    bullet_id++;
    if(bullet_id > 10000)
        bullet_id = 1;
}

void GameLogicHandler::updatePlayerPosition(int x, int y, const QString& name)
{
    using namespace ServerConfig;


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

    qreal width = players_[name]->getDrawer()->boundingRect().width();
    qreal height = players_[name]->getDrawer()->boundingRect().height();

    if(checkPlayerCollision(x, y, name) ||
        checkPlayerCollision(x + width, y, name) ||
        checkPlayerCollision(x, y + height, name) ||
        checkPlayerCollision(x + width, y + height, name))
        return;


    players_[name]->getDrawer()->setPos(x, y);
    players_[name]->getMeleeWeapon()->getDrawer()->setPos(x+IMAGE_SIZE/2, y+IMAGE_SIZE/2);
}

bool GameLogicHandler::checkPlayerCollision(qreal x, qreal y, const QString &name)
{
    int id_x = (int)x/IMAGE_SIZE;
    int id_y = (int)y/IMAGE_SIZE;
    int tile_id = id_y * map_->getM() + id_x;

    if(matrix_[tile_id]->getTileType() == Tile::TileType::AMMO_PILE && map_->getActiveAmmoBuckets().contains(tile_id))
    {
        std::unordered_map<int, Tile*> active_buckets = map_->getActiveAmmoBuckets();

        map_->removeFromActive(tile_id);
        players_[name]->getRangedWeapon()->setRemainingBullets(players_[name]->getRangedWeapon()->getRemainingBullets() + AMMO_BUCKET_CAPACITY);


        QByteArray tile_info = jsonify_tile(tile_id, "../silent-edge/src/images/ground.png");
        emit tileChangedSignal(tile_info);
    }

    if(matrix_[tile_id]->getTileType() == Tile::TileType::WALL)
        return true;

    return false;
}

QByteArray GameLogicHandler::jsonify_tile(int tile_id, const QString &path)
{
    QJsonObject tile_object;
    tile_object["type"] = "tile";
    tile_object["tile_id"] = tile_id;
    tile_object["path"] = path;
    const QJsonDocument json_data(tile_object);

    return json_data.toJson();
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
            playerObject["swinging"] = melee_in_progress_[name];
            playerObject["reloading"] = reloading_in_progress_[name];
            playerObject["shooting"] = shooting_in_progress_[name];
            playerObject["bullet_count"] = static_cast<qint32>(player_bullet_count_[name]);
            playerObject["remaining_bullets"] = player->getRangedWeapon()->getRemainingBullets();

            playersArray.append(playerObject);
        }

        const QJsonDocument json_data(playersArray);

        return json_data.toJson();
    }
    else if(data_type == "bullet") {
        QJsonArray bulletsArray;

        for(auto& [bullet_id, bullet] : bullets_) {
            QJsonObject bulletObject;
            bulletObject["type"] = "bullet";
            bulletObject["id"] = bullet_id;
            bulletObject["owner_name"] = bullet->getOwnerName();
            bulletObject["position_x"] = bullet->getDrawer()->x();
            bulletObject["position_y"] = bullet->getDrawer()->y();
            bulletObject["rotation"] = bullet->getDrawer()->rotation();

            bulletsArray.append(bulletObject);
        }

        const QJsonDocument json_data(bulletsArray);

        return json_data.toJson();
    }
    else {
        return nullptr;
    }
}

void GameLogicHandler::updateAll()
{
    updatePlayers();
    updateBullets();

    QByteArray player_info = jsonify("player");
    QByteArray bullet_info = jsonify("bullet");

    emit updatePlayersSignal(player_info);
    emit updateBulletsSignal(bullet_info);
}

void GameLogicHandler::updatePlayers()
{
    for (auto& [name, player] : players_)
    {
        qreal x = player->getDrawer()->x();
        qreal y = player->getDrawer()->y();


        // limun: kolizija, rotacija, pozicija, meci

        for(auto &[other_name, other_player] : players_)
        {
            if(name == other_name)
                continue;

            auto melee_drawer = other_player->getMeleeWeapon()->getDrawer();
            if(melee_drawer->zValue() < 0)
                continue;
            if(melee_drawer->collidesWithItem(players_[name]->getDrawer()))
            {
                players_[name]->setHp(players_[name]->getHp() - 100);
                qDebug() << "Player " << name << " was cut into pieces! by " << other_name << "!";
                removePlayer(name);
            }
        }

        updatePlayerRotation(x, y, name);
        updatePlayerPosition(x, y, name);

        if((commands_[name] & ServerConfig::PlayerActions::SHOOT))
        {
            if(!shooting_in_progress_[name] && player_bullet_count_[name] < player->getRangedWeapon()->getCapacity())
            {
                addBullet(x, y, name);
                shooting_in_progress_[name] = true;
                ++player_bullet_count_[name];

            }
        }
        else
            shooting_in_progress_[name] = false;

        if((commands_[name] & ServerConfig::PlayerActions::MELEE))
        {
            if(!melee_in_progress_[name])
            {
                melee_in_progress_[name] = true;
                players_[name]->getMeleeWeapon()->getDrawer()->setZValue(1);
                players_[name]->getSwingTimer()->start();
            }
        }
        else
        {
            players_[name]->getMeleeWeapon()->getDrawer()->setZValue(-1);
            melee_in_progress_[name] = false;
        }

        if(commands_[name] & ServerConfig::PlayerActions::RELOAD)
        {
            if(player_bullet_count_[name] != 0 && players_[name]->getRangedWeapon()->getRemainingBullets() != 0)
            {
                if(!reloading_in_progress_[name])
                {
                    reloading_in_progress_[name] = true;
                    players_[name]->getReloadTimer()->start();
                }
            }
        }
    }
}

void GameLogicHandler::updateAmmo()
{
    map_->restockAmmoPiles();

    QJsonObject json_object;
    QString type = "bucket_activation";
    json_object["type"] = type;
    QJsonDocument json_data(json_object);
    QByteArray byte_array = json_data.toJson();

    emit restockAmmoPilesSignal(byte_array);
}

void GameLogicHandler::addPlayer(Player* playa)
{
    QString name = playa->getName();
    players_[name] = playa;
    commands_[name] = 0;
    mouse_positions_[name] = {0.0, 0.0};
    shooting_in_progress_[name] = false;
    melee_in_progress_[name] = false;
    reloading_in_progress_[name] = false;
    player_bullet_count_[name] = 0;
    connect(playa->getReloadTimer(), &QTimer::timeout, std::bind(&GameLogicHandler::reload, this, name));
    connect(playa->getSwingTimer(), &QTimer::timeout, std::bind(&GameLogicHandler::swing, this, name));


    putPlayerIntoRoom(name);
}

void GameLogicHandler::removePlayer(QString name)
{
    removePlayerFromRoom(name);
    players_[name]->setHp(100);

    putPlayerIntoRoom(name);
}

void GameLogicHandler::updateBullets()
{
    QMutexLocker locker(&mutex_);
    for(auto it = bullets_.cbegin(); it != bullets_.cend();)
    {
        if (checkBulletCollisions(it->second))
        {
            // limun: neki emit treba ovde ili prosto da brišemo svaki metak koji se ne pojavljuje više u poruci od servera

            QJsonObject object;
            object["type"] = "delete_bullet";
            object["id"] = it->first;
            const QJsonDocument json_data(object);

            QByteArray bullet_info = json_data.toJson();

            emit bulletDestroyedSignal(bullet_info);

            bullets_.erase(it++->first);
        }
        else
        {
            qreal x_pos = it->second->getDrawer()->x() + BULLET_SPEED * qSin(qDegreesToRadians(it->second->getDrawer()->rotation()));
            qreal y_pos = it->second->getDrawer()->y() - BULLET_SPEED * qCos(qDegreesToRadians(it->second->getDrawer()->rotation()));

            it->second->getDrawer()->setPos(x_pos, y_pos);

            it++;
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

    players_[name]->getMeleeWeapon()->getDrawer()->setPos(top_right.x(), top_right.y());
    if (aim_dir.manhattanLength() < 50)
        return;
    qreal angle = qAtan2(aim_dir.x(), -aim_dir.y());
    angle = qRadiansToDegrees(angle);

    players_[name]->getDrawer()->setRotation(angle);
    players_[name]->getMeleeWeapon()->getDrawer()->setRotation(angle);
}

bool GameLogicHandler::checkBulletCollisions(Bullet *bullet)
{
    // limun: dosta bolje (videćemo)
    for(auto &[name, player] : players_) {
        if(name == bullet->getOwnerName())
            continue;
        if(bullet->getDrawer()->collidesWithItem(player->getDrawer())) {
            decreaseHp(player, bullet);

            qDebug() << name << " has " << player->getHp() << "hp";

            if(player->getHp() <= 0)
            {
                qDebug() << "igrac unisten";
                removePlayer(name);
                // limun: ko zna, zna
            }

            return true;
        }
    }

    int x = bullet->getDrawer()->x()/IMAGE_SIZE;
    int y = bullet->getDrawer()->y()/IMAGE_SIZE;

    int tile_id = y * map_->getM() + x;

    if(matrix_[tile_id]->getTileType() == Tile::TileType::WALL)
    {
        return true;
    }

    return false;
}

void GameLogicHandler::decreaseHp(Player* player, Bullet* bullet)
{
    qreal player_hp = player->getHp();
    qreal bullet_damage = bullet->getDamageDealt();
    player->setHp(player_hp - bullet_damage);
}


void GameLogicHandler::reload(const QString& name)
{
    players_[name]->getReloadTimer()->stop();
    reloading_in_progress_[name] = false;

    quint32 bullets_shot = player_bullet_count_[name];
    player_bullet_count_[name] -= std::min(players_[name]->getRangedWeapon()->getRemainingBullets(), static_cast<qint32>(player_bullet_count_[name]));
    players_[name]->getRangedWeapon()->setRemainingBullets(std::max(players_[name]->getRangedWeapon()->getRemainingBullets() - static_cast<qint32>(bullets_shot), 0));

}

void GameLogicHandler::swing(const QString &name)
{
    players_[name]->getSwingTimer()->stop();
}


void GameLogicHandler::initializeTimers()
{
    ammo_respawn_timer_.setInterval(AMMO_RESPAWN_TIME);
    connect(&ammo_respawn_timer_, &QTimer::timeout, this, &GameLogicHandler::updateAmmo);
    ammo_respawn_timer_.start();
}

// pozvati na kraju runde
void GameLogicHandler::updateScores()
{
    for(auto it = rooms_.begin(); it != rooms_.end(); it++)
    {
        Room *room = it->second;

        for (auto player : room->players_in_room_)
        {
            if(player->getHp() != 0)
            {
                player->setScore(player->getScore() + 1);
            }
        }
    }
}

void GameLogicHandler::rangList()
{
    QVector<std::pair<QString, Player*>> playersVector(players_.begin(), players_.end());
    std::sort(playersVector.begin(), playersVector.end(),
              [](const auto& p1, const auto& p2) {
                  return p1.second->getScore() > p2.second->getScore();
              });

//    QString path = QDir::currentPath().mid(0, QDir::currentPath().lastIndexOf('/'));
//    QFile resultsFile(path + "/silent-edge/resources/results.txt");
//    resultsFile.open(QIODevice::WriteOnly | QIODevice::Text);

//    if(!resultsFile.exists()){
//        qDebug() << "File does not exist.";
//        return ;
//    }
//    if(!resultsFile.isOpen())
//    {
//        qDebug() << "Opening failed";
//        return;
//    }

//    QTextStream stream(&resultsFile);

//    for (auto player = playersVector.begin(); player != playersVector.end(); player++)
//    {
//        stream << player->first << " " << player->second->getScore() << "\n";
//    }

//    resultsFile.close();

//    emit gameIsOver();
//    results_->showResults();
}

void GameLogicHandler::updatePlayerStats(const QByteArray &player_info)
{
    QJsonParseError parse_error;
    const QJsonDocument json_data = QJsonDocument::fromJson(player_info, &parse_error);
    if (parse_error.error == QJsonParseError::NoError && json_data.isObject())
    {
        QString name = json_data["name"].toString();
        if (commands_.find(name) == commands_.end())
        {
            Player* playa = new Player(name);
            addPlayer(playa);
        }
        commands_[name] = static_cast<quint32>(json_data["movement"].toInteger());
        mouse_positions_[name] = QPair<qreal, qreal>(json_data["mouse_x"].toDouble(), json_data["mouse_y"].toDouble());
    }
    else
    {
        // todo: handle? ili ne? nemamo vremena :(((
    }
}

