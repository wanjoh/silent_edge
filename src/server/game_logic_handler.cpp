#include "game_logic_handler.hpp"
#include "server_config.hpp"
#include <QtMath>
#include <QVector2D>

GameLogicHandler::GameLogicHandler(Map *map, QObject* parent)
    : QObject(parent)
    , map_(map)
    , matrix_(map_->getMatrix())
    , rooms_(map_->getRooms())
{
    initializeTimers();
    put_players_into_rooms();
}

GameLogicHandler::~GameLogicHandler()
{
    for (auto& [_, player] : players_)
    {
        delete player;
    }
}

void GameLogicHandler::put_players_into_rooms()
{
    auto player_it = players_.begin();
    for(auto it = rooms_.begin(); it != rooms_.end(); it++) {
        std::unordered_map<int, QPair<int, int>> spawnpoints = it->second->get_unused_spawnpoints();
        int n = spawnpoints.size();
        for(int i = 0; i < n; i++) {
            it->second->add_player_to_room(player_it->second);
            player_it++;
        }
    }
}

void GameLogicHandler::addBullet(const QString& name, qreal rotation)
{
    if(!(commands_[name] & ServerConfig::PlayerActions::SHOOT))
        return;
    QMutexLocker locker(&mutex_);
    Bullet *bullet = new Bullet("bullet");
    bullet->getDrawer()->setRotation(rotation);
    bullets_.push_back({name, bullet});
   // todo: add bullet rotation
   // limun: evo
}

void GameLogicHandler::updatePlayerPosition(int x, int y, const QString& name, Player *player)
{
    using namespace ServerConfig;
    bool moved = false;
    // todo: apdejtovati pomeraj na osnovu poslate komande od strane igraca
    // limun: važi, urađeno
    qreal dx = 0.0 - (commands_[name] & PlayerActions::LEFT) + (commands_[name] & PlayerActions::RIGHT);
    qreal dy = 0.0 - (commands_[name] & PlayerActions::DOWN) + (commands_[name] & PlayerActions::UP);

    if(dx == 0 && dy == 0)
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

    QList<QGraphicsItem*> collidingItems = player->getDrawer()->collidingItems();
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

    player->getDrawer()->setPos(x, y);
}

void GameLogicHandler::updatePlayers()
{
    for (auto& [name, player] : players_)
    {
        qreal x = players_[name]->getDrawer()->x();
        qreal y = players_[name]->getDrawer()->y();

        // limun: rotacija, pozicija, meci
        qreal rotation = updatePlayerRotation(x, y, name, player);
        updatePlayerPosition(x, y, name, player);
        addBullet(name, rotation);
    }
}

void GameLogicHandler::updateAmmo()
{
    map_->restockAmmoPiles();
}

void GameLogicHandler::updateBullets()
{
    QMutexLocker locker(&mutex_);
    QList<QPair<QString, Bullet*>>::Iterator it = bullets_.begin();
    while (it != bullets_.end())
    {
        qreal x_pos = it->second->getDrawer()->scenePos().x() + BULLET_SPEED * it->second->aim_dir().x();
        qreal y_pos = it->second->getDrawer()->scenePos().y() + BULLET_SPEED * it->second->aim_dir().y();

        it->second->getDrawer()->setPos(x_pos, y_pos);

        if (checkCollisions(it->second))
        {
            delete it->second;
            bullets_.erase(it++);
        }
        else
        {
            it++;
        }
    }
}

qreal GameLogicHandler::updatePlayerRotation(int x, int y, const QString& name, Player *player)
{
    qreal theta = 0.0;

    qreal mouse_x = mouse_positions_[name].first;
    qreal mouse_y = mouse_positions_[name].second;

    QVector2D v1(mouse_x - x, mouse_y - y);
    QVector2D v2(0.0, y);

    qreal dot = QVector2D::dotProduct(v1, v2);

    if(abs(v1.length()) < EPSILON || abs(v2.length()) < EPSILON)
    {
        return theta;
    }

    qreal cos_theta = dot / (v1.length() * v2.length());

    theta = std::acos(cos_theta);

    player->getDrawer()->setRotation(theta);

    return theta;
}

bool GameLogicHandler::checkCollisions(Bullet* bullet)
{
    // todo: deluje sumnjicavo, verovatno se moze optimizovati
    QList<QGraphicsItem*> colidingItems = bullet->getDrawer()->collidingItems();

    foreach(QGraphicsItem *item, colidingItems)
    {
        QGraphicsPixmapItem* pixmap_item = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(pixmap_item)
        {
            if(typeid(*pixmap_item) == typeid(PlayerDrawer))
            {
                PlayerDrawer *player_drawer = dynamic_cast<PlayerDrawer*>(pixmap_item);
                QString player_name = player_drawer->name();

                decreaseHp(players_[player_name], bullet);

                qDebug() << player_name << " has " << players_[player_name]->getHp() << "hp";

                if(players_[player_name]->getHp() == 0)
                {
                    qDebug() << "igrac unisten";
                    // todo: handle
                    emit playerDestroyedSignal();
                }

                return true;
            }
            if(typeid(*pixmap_item) == typeid(TileDrawer))
            {
                TileDrawer *tile_drawer = dynamic_cast<TileDrawer*>(pixmap_item);
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
    // limun: ovo je verovatno nepotrebno

    // movement_timer_.setInterval(1000 / TARGET_FPS);
    // shooting_timer_.setInterval(1000 / BULLETS_PER_SECOND);
    // connect(&movement_timer_, &QTimer::timeout, this, &GameLogicHandler::updatePlayerPosition);
    // connect(&shooting_timer_, &QTimer::timeout, this, &GameLogicHandler::updateBullets);
    // movement_timer_.start();
    // shooting_timer_.start();

    ammo_respawn_timer_.setInterval(AMMO_RESPAWN_TIME);
    connect(&ammo_respawn_timer_, &QTimer::timeout, this, &GameLogicHandler::updateAmmo);
    ammo_respawn_timer_.start();
}

void GameLogicHandler::updatePlayerStats(QByteArray &player_info)
{
    // todo: deserijalizovati BEZ KLOSARSKOG QVARIANT MOLIM VAS
    // limun: evo
    // limun: 8 + 4 + 2 * 8 = 28 bajtova

    QDataStream stream(player_info);

    QString name;
    int commands;
    qreal mouse_x;
    qreal mouse_y;

    stream >> name >> commands >> mouse_x >> mouse_y;

    commands_[name] = commands;
    mouse_positions_[name] = QPair<qreal, qreal>(mouse_x, mouse_y);
}

