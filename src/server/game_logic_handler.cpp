#include "game_logic_handler.hpp"
#include <QtMath>

GameLogicHandler::GameLogicHandler(Map *map, QObject* parent)
    : QObject(parent)
    , map_(map)
    , matrix_(map_->get_matrix())
    , rooms_(map_->get_rooms())
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

void GameLogicHandler::addBullet(QString name, Bullet* bullet)
{
    QMutexLocker locker(&mutex_);
    bullets_.push_back({name, bullet});
   // todo: add bullet rotation
   // limun: oni na tome rade na drugoj grani, ne diram za sada
}

void GameLogicHandler::updateMovement()
{
    for (auto& [name, player] : players_)
    {
        qreal x = players_[name]->getDrawer()->x();
        qreal y = players_[name]->getDrawer()->y();

        bool moved = false;

        // todo: apdejtovati pomeraj na osnovu poslate komande od strane igraca
        // limun: važi, urađeno
        qreal dx = -commands[name] & 1 + commands[name] & (1 << 3);
        qreal dy = -commands[name] & (1 << 4) + commands[name] & (1 << 2);

        if(dx == 0 && dy == 0)
            break;

        if (qFabs(dx) > EPSILON && qFabs(dy) > EPSILON)
        {
            qreal length = qSqrt(dx * dx + dy * dy);
            dx /= length;
            dy /= length;
        }

        x += dx * DEFAULT_PLAYER_VELOCITY;
        y += dy * DEFAULT_PLAYER_VELOCITY;

        std::unordered_map<int, Tile*> active_buckets = map_->get_active_ammo_buckets();

        QList<QGraphicsItem*> collidingItems = player->getDrawer()->collidingItems();
        for(auto item : collidingItems) {
            QGraphicsPixmapItem* pixmap_item = dynamic_cast<QGraphicsPixmapItem*>(item);
            if(pixmap_item) {
                if(typeid(*pixmap_item) == typeid(TileDrawer))
                {
                    TileDrawer *tile_drawer = dynamic_cast<TileDrawer*>(pixmap_item);
                    int tile_id = tile_drawer->y()/IMAGE_SIZE * map_->get_m() + tile_drawer->x()/IMAGE_SIZE;
                    if(matrix_[tile_id]->getTileType() == Tile::TileType::WALL) {
                        return;
                    }

                    if(matrix_[tile_id]->getTileType() == Tile::TileType::AMMO_PILE) {
                        // limun: treba i povecati municiju player-u, to je na drugoj grani

                        QString path ="../silent-edge/src/images/ground.png";
                        map_->get_drawer()->change_picture(active_buckets[tile_id], path);
                        map_->remove_from_active(tile_id);


                        emit tile_changed(tile_id, path);
                    }
                }
            }
        }

        player->getDrawer()->setPos(x, y);
    }
}

void GameLogicHandler::updateAmmo()
{
    map_->restock_ammo_piles();
}

void GameLogicHandler::updateBullets()
{
    QMutexLocker locker(&mutex_);
    // ovo ide od nazad jer se elementi brisu
    // limun: evo sa it, valjda je tako bolje
    for(auto it = bullets_.crbegin(); it != bullets_.crend(); it++)
    {
        // todo: ovo ce se se skloniti kad se uvede bolja struktura podataka, jako je lose
        // limun: nmp šta bi to podrazumevalo
        if(it->second == nullptr)
        {
            // limun: stackoverflow.com/questions/1830158/how-to-call-erase-with-a-reverse-iterator
            std::advance(it, 1);
            bullets_.erase(it.base());
            continue;
        }
        // limun: može bullet speed da se stavi umesto 10
        qreal x_pos = it->second->getDrawer()->scenePos().x() + 10 * it->second->aim_dir().x();
        qreal y_pos = it->second->getDrawer()->scenePos().y() + 10 * it->second->aim_dir().y();


        it->second->getDrawer()->setPos(x_pos, y_pos);
        // todo: proveriti kolizije na optimalniji nacin
        // limun: mislim da je ok, optimalnost ćemo kasnije

        checkCollisions(it->second);
    }
}

void GameLogicHandler::destroyBullet(Bullet* bullet)
{
    delete bullet;
}

void GameLogicHandler::updatePlayers()
{
    updateMovement();
    updateRotation();
}

void GameLogicHandler::updateRotation()
{

}

void GameLogicHandler::checkCollisions(Bullet* bullet)
{
    // todo: deluje sumnjicavo, verovatno se moze optimizovati
    // limun: ne znam
    QList<QGraphicsItem*> colidingItems = bullet->getDrawer()->collidingItems();

    foreach(QGraphicsItem *item, colidingItems)
    {
        QGraphicsPixmapItem* pixmap_item = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(pixmap_item)
        {
            if(typeid(*pixmap_item) == typeid(PlayerDrawer))
            {
                PlayerDrawer *player_drawer = dynamic_cast<PlayerDrawer*>(pixmap_item);
                QString enemy_name = player_drawer->name();
                Player *enemy = players_[enemy_name];

                decreaseHp(enemy, bullet);

                qDebug() << enemy_name << " has " << enemy->getHp() << "hp";

                if(enemy->getHp() == 0)
                {
                    qDebug() << "igrac unisten";
                    // todo: hendlovati ovo
                    // limun: užas, nemam pojma
                }
            }
            if(typeid(*pixmap_item) == typeid(TileDrawer))
            {
                TileDrawer *tile_drawer = dynamic_cast<TileDrawer*>(pixmap_item);
                int tile_id = tile_drawer->y()/IMAGE_SIZE * map_->get_m() + tile_drawer->x()/IMAGE_SIZE;
                if(matrix_[tile_id]->getTileType() == Tile::TileType::WALL) {
                    destroyBullet(bullet);
                    break;
                }
            }
        }
    }
}

void GameLogicHandler::decreaseHp(Player* player, Bullet* bullet)
{
    qreal player_hp = player->getHp();
    qreal bullet_damage = bullet->getDamageDealt();
    player->setHp(player_hp - bullet_damage);
}

void GameLogicHandler::initializeTimers()
{
    movement_timer_.setInterval(1000 / TARGET_FPS);
    shooting_timer_.setInterval(1000 / BULLETS_PER_SECOND);
    connect(&movement_timer_, &QTimer::timeout, this, &GameLogicHandler::updateMovement);

    connect(&shooting_timer_, &QTimer::timeout, this, &GameLogicHandler::updateBullets);

    movement_timer_.start();
    shooting_timer_.start();

    ammo_respawn_timer_.setInterval(AMMO_RESPAWN_TIME);
    connect(&ammo_respawn_timer_, &QTimer::timeout, this, &GameLogicHandler::updateAmmo);
    ammo_respawn_timer_.start();
}

void GameLogicHandler::updatePlayer(QByteArray player_info)
{
    //todo: deserijalizovati BEZ KLOSARSKOG QVARIANT MOLIM VAS
    //limun: smešno, ignorisaću caps deo za sada
}

