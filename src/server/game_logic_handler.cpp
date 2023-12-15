#include "game_logic_handler.hpp"
#include <QtMath>

const int IMAGE_SIZE = 64;

GameLogicHandler::GameLogicHandler(Map *map, QObject* parent)
    : QObject(parent)
    , map_object_(map)
    , map_(map_object_->get_matrix())
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

void GameLogicHandler::addBullet(QString name, Bullet* bullet)
{
    QMutexLocker locker(&mutex_);
    bullets_.push_back({name, bullet});
   // todo: add bullet rotation
}

void GameLogicHandler::updateMovement()
{
    for (auto& [_, player] : players_)
    {

        qreal x = player->getDrawer()->x();
        qreal y = player->getDrawer()->y();

        bool moved = false;

        // todo: apdejtovati pomeraj na osnovu poslate komande od strane igraca
        qreal dx = 0.0;
        qreal dy = 0.0;

        if (qFabs(dx) > EPSILON && qFabs(dy) > EPSILON)
        {
            qreal length = qSqrt(dx * dx + dy * dy);
            dx /= length;
            dy /= length;
        }


        x += dx * DEFAULT_PLAYER_VELOCITY;
        y += dy * DEFAULT_PLAYER_VELOCITY;

        if (qFabs(dx) > EPSILON || qFabs(dy) > EPSILON)
        {
            moved = true;
        }


        int x1 = x/IMAGE_SIZE;
        int y1 = y/IMAGE_SIZE;

        QVector<QString> edges;
        for(int i = x1; i < x1 + 2; i++) {
            for(int j = y1; j < y1 + 2; j++) {
                QString edge = QString("%1 %2").arg(i).arg(j);
                edges.push_back(edge);
            }
        }

        bool can_move = canEntityMove(edges);
        std::unordered_map<QString, Tile*> active_buckets = map_object_->get_active_ammo_buckets();

        if (moved && can_move) {
            player->getDrawer()->setPos(x, y);

            for(QString& edge : edges) {
                if(active_buckets.contains(edge)) {
                    QPair<int, int> coords = map_[edge]->get_coords();
                    map_object_->remove_tile(edge);
                    map_object_->add_ground_tile_of_type_ammo(edge, coords.first, coords.second);

                    emit tileDeleted(edge);
                }
            }
        }
    }
}


void GameLogicHandler::updateAmmo()
{
    map_object_->restock_ammo_piles();
}

bool GameLogicHandler::canEntityMove(QVector<QString> &edges)
{
    bool can_move = true;

    for(QString& edge : edges) {
        Tile* tile = map_[edge];
        if(tile && tile->getTileType() == Tile::TileType::WALL) {
            can_move = false;
        }
    }

    return can_move;
}

void GameLogicHandler::updateBullets()
{
    QMutexLocker locker(&mutex_);
    quint32 i = 0;
    // ovo ide od nazad jer se elementi brisu
    for(int i = bullets_.size(); i >= 0; i--)
    {
        // todo: ovo ce se se skloniti kad se uvede bolja struktura podataka, jako je lose
        if (bullets_[i].second == nullptr)
        {
            bullets_.erase(bullets_.begin() + i);
            continue;
        }
        qreal x_pos = bullets_[i].second->getDrawer()->scenePos().x() + 10 * bullets_[i].second->aim_dir().x();
        qreal y_pos = bullets_[i].second->getDrawer()->scenePos().y() + 10 * bullets_[i].second->aim_dir().y();


        bullets_[i].second->getDrawer()->setPos(x_pos, y_pos);
        // todo: proveriti kolizije na optimalniji nacin
        checkCollisions(bullets_[i].second);
    }
}

void GameLogicHandler::destroyBullet(Bullet* bullet)
{
    delete bullet;
}


void GameLogicHandler::checkCollisions(Bullet* bullet)
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
                //Player* player = dynamic_cast<Player>(item);

                PlayerDrawer *player_drawer = dynamic_cast<PlayerDrawer*>(pixmap_item);

                QString enemy_name = player_drawer->name();

                //qDebug() << "collision with " << enemies_[enemy_name]->getName();

                Player *enemy = players_[enemy_name];

                decreaseHp(enemy,bullet);

                qDebug() << enemy_name << " has " << enemy->getHp() << "hp";

                if(enemy->getHp() == 0)
                {
                    qDebug() << "igrac unisten";
                    //todo: hendlovati ovo
                }
            }
            if(typeid(*pixmap_item) == typeid(TileDrawer))
            {

                TileDrawer *tile_drawer = dynamic_cast<TileDrawer*>(pixmap_item);


                int x1 = tile_drawer->x()/IMAGE_SIZE;
                int y1 = tile_drawer->y()/IMAGE_SIZE;


                QVector<QString> edges;

                QString name = QString("%1 %2").arg(x1).arg(y1);

                edges.push_back(name);

                bool can_move = canEntityMove(edges);

                if(!can_move) {
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
}

