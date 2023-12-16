#include "game_logic_handler.hpp"
#include <QtMath>

GameLogicHandler::GameLogicHandler(QString name, Map *map, QObject* parent)
    : QObject(parent)
    , map_object_(map)
    , map_(map_object_->get_matrix())
    , player_(new Player(name, false))
{

    player_->getDrawer()->setPos(2*IMAGE_SIZE, 2*IMAGE_SIZE);
    initializeTimers();

}

GameLogicHandler::~GameLogicHandler()
{
    for (auto& [_, enemy] : enemies_)
    {
        delete enemy;
    }
}

Player *GameLogicHandler::getPlayer()
{
    return player_;
}

void GameLogicHandler::resetKeys()
{
    for (auto& [_, v] : keys_)
    {
        v = false;
    }
}

void GameLogicHandler::updateKeys(quint32 key, bool pressed)
{
    keys_[key] = pressed;
}

void GameLogicHandler::updateMouseClick(Qt::MouseButton button, bool pressed)
{
    keys_[button] = pressed;
}


void GameLogicHandler::addBullet(QString name)
{
    QMutexLocker locker(&mutex_);
    QString bullet_name = name + QString::number(player_bullet_count_[name]);
    Bullet* bullet = new Bullet(bullet_name);
    player_bullet_count_[name]++;
    bullets_[name].push_back(bullet);
    emit newBulletSignal(bullet_name, bullet->getDrawer());

    QVector2D aim_dir = QVector2D(aiming_point_-player_->getDrawer()->scenePos());

    aim_dir.normalize();

    bullet->setAim_dir(aim_dir);

    bullet->getDrawer()->setPos(player_->getDrawer()->scenePos().x(),player_->getDrawer()->scenePos().y()-1.1*bullet->BULLET_HEIGHT);

    emit bulletMoved(bullet->toVariant());

    qDebug() << "bullet added " << bullet_name;
}

void GameLogicHandler::updateMovement()
{
    qreal x = player_->getDrawer()->x();
    qreal y = player_->getDrawer()->y();

    bool moved = false;

    qreal dx = (keys_[Qt::Key_D] ? 1.0 : 0.0) - (keys_[Qt::Key_A] ? 1.0 : 0.0);
    qreal dy = (keys_[Qt::Key_S] ? 1.0 : 0.0) - (keys_[Qt::Key_W] ? 1.0 : 0.0);

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

    QVector<int> ids;
    for(int i = x1; i < x1 + 2; i++)
        for(int j = y1; j < y1 + 2; j++)
            ids.append(i * 12 + j);

    bool can_move = canEntityMove(ids);
    std::unordered_map<int, Tile*> active_buckets = map_object_->get_active_ammo_buckets();

    if (moved && can_move) {
        player_->getDrawer()->setPos(x, y);

        for(int id : ids) {
            if(active_buckets.contains(id)) {
                map_[id]->setDrawer("../silent-edge/src/images/ground.png");
                map_object_->remove_from_active(id);

                emit tileDeleted(id);
            }
        }
    }

    emit playerMoved(player_->toVariant());
}


void GameLogicHandler::updateAmmo()
{
    map_object_->restock_ammo_piles();
}

bool GameLogicHandler::canEntityMove(QVector<int> &edges)
{
    bool can_move = true;

    for(int edge : edges) {
        Tile* tile = map_[edge];
        if(tile && tile->getTileType() == Tile::TileType::WALL) {
            can_move = false;
        }
    }

    return can_move;
}

void GameLogicHandler::updateBullets()
{
    if (keys_[Qt::RightButton])
    {

    }
    else if (keys_[Qt::LeftButton])
    {
        addBullet(player_->getName());
    }
    QMutexLocker locker(&mutex_);
    for(auto& [_, bullets] : bullets_)
    {
        for(int i = bullets.size() - 1; i >= 0; i--)
        {
            qreal x_pos = bullets[i]->getDrawer()->scenePos().x() + 10 * bullets[i]->aim_dir().x();
            qreal y_pos = bullets[i]->getDrawer()->scenePos().y() + 10 * bullets[i]->aim_dir().y();


            bullets[i]->getDrawer()->setPos(x_pos, y_pos);

            //qDebug() << bullet->getDrawer()->scenePos().x();
            //            qDebug() << x_pos << " " << y_pos;
            //            emit bulletMoved(bullet->toVariant());

            if (checkCollisions(bullets[i]))
            {
                bullets.erase(bullets.begin() + i);
            }
        }
    }
}

void GameLogicHandler::swingMelee()
{
    if(keys_[Qt::LeftButton])
    {
        // ne moze da se swinguje
    }
    else if(keys_[Qt::RightButton])
    {
        // e ovde moze
    }

}

bool GameLogicHandler::checkCollisions(Bullet* bullet)
{
    QList<QGraphicsItem*> coliding_items = bullet->getDrawer()->collidingItems();

    foreach(QGraphicsItem *item, coliding_items)
    {
        QGraphicsPixmapItem* pixmap_item = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(pixmap_item)
        {
            if(typeid(*pixmap_item) == typeid(PlayerDrawer))
            {
                //Player* player = dynamic_cast<Player>(item);

                PlayerDrawer *player_drawer = dynamic_cast<PlayerDrawer*>(pixmap_item);

                QString enemy_name = player_drawer->name();
                // ako je igrac nas, on nece biti u enemies_
                // todo: OVO CE SE SVE IZMENITI KAD PREBACIMO NA SERVER, TRENUTNO JE VELIKO MEGA SPAGETI KOD
                if (enemy_name == player_->getName())
                {
                    decreaseHp(player_, bullet);
                    return true;
                }
                //qDebug() << "collision with " << enemies_[enemy_name]->getName();

                Player *enemy = enemies_[enemy_name];

                decreaseHp(enemy,bullet);

                qDebug() << enemy_name << " has " << enemy->getHp() << "hp";

            }
            if(typeid(*pixmap_item) == typeid(TileDrawer))
            {

                TileDrawer *tile_drawer = dynamic_cast<TileDrawer*>(pixmap_item);


                int x1 = tile_drawer->x()/IMAGE_SIZE;
                int y1 = tile_drawer->y()/IMAGE_SIZE;

                QVector<int> ids = {x1 * 12 + y1};

                bool can_move = canEntityMove(ids);

                if(!can_move) {
                    QString bullet_name = bullet->getName();
                    delete bullet;
                    bullet = nullptr;
                    emit destroyBullet(bullet_name);
                    return true;
                }

            }

            //emit destroyBullet(bullet->getName());
        }
    }
    return false;
}

void GameLogicHandler::decreaseHp(Player* player, Bullet* bullet)
{
    qreal player_hp = player->getHp();
    qreal bullet_damage = bullet->getDamageDealt();
    player->setHp(player_hp - bullet_damage);
    if(player->getHp() == 0)
        emit destroyPlayer(player->getName());

    emit update_hp(player->getHp());
}

void GameLogicHandler::updateAimingPoint(QPointF point)
{
    aiming_point_ = point;
}

void GameLogicHandler::handleEnemy(QVariant variant)
{
    Player *enemy = new Player("enemy");
    enemy->fromVariant(variant);
    QString enemy_name = enemy->getName();

    enemy->getDrawer()->setName(enemy_name);

    if (enemies_.find(enemy_name) == enemies_.end())
    {
        enemies_[enemy_name] = enemy;
        emit update(enemy_name, enemy->getDrawer());
    }
    else
    {
        delete enemy;
        enemies_[enemy_name]->fromVariant(variant);
    }
}

void GameLogicHandler::handleBullet(QVariant variant)
{
    Bullet *bullet = new Bullet(player_->getName());
    bullet->fromVariant(variant);

    if(!(bullets_[player_->getName()].empty()))
    {
        for (Bullet* bullet_ : bullets_[player_->getName()])
        {
            bullet_->fromVariant(variant);
//          emit update(player_->getName(), bullet_->getDrawer());
        }
    }
    else
    {
        bullets_[player_->getName()].push_back(bullet);

        emit update(player_->getName(), bullet->getDrawer());
    }

    //        emit update(player_->getName(), bullet->getDrawer());

}

void GameLogicHandler::recognizeEntityType(QVariant variant)
{
    if(variant.canConvert<QVariantMap>())
    {
        QVariantMap map = variant.toMap();

        if(map.contains("type_entity"))
        {
            QString type = map.value("type_entity").toString();

            if(type == "bullet")
            {
                handleBullet(variant);
            }
            else if(type == "player")
            {
                handleEnemy(variant);
            }
            else
            {
                qDebug() << "Greska.";
            }
        }
    }
}

void GameLogicHandler::initializeTimers()
{
    movement_timer_.setInterval(1000 / TARGET_FPS);
    shooting_timer_.setInterval(1000 / player_->getRangedWeapon()->getRateOfFire());
    connect(&movement_timer_, &QTimer::timeout, this, &GameLogicHandler::updateMovement);
    // za sad se koristi isti tajmer
    connect(&movement_timer_, &QTimer::timeout, this, &GameLogicHandler::updateBullets);
    connect(&shooting_timer_, &QTimer::timeout, this, &GameLogicHandler::swingMelee);

    movement_timer_.start();

    ammo_respawn_timer_.setInterval(5000);
    connect(&ammo_respawn_timer_, &QTimer::timeout, this, &GameLogicHandler::updateAmmo);
    ammo_respawn_timer_.start();
    shooting_timer_.start();
}

bool GameLogicHandler::updateRotation()
{
    qreal angle = atan2(aiming_point_.y() - player_->getDrawer()->scenePos().y(),
                        aiming_point_.x() - player_->getDrawer()->scenePos().x());
    bool rotated = qFabs(angle - player_->getDrawer()->rotation()) > EPSILON;
    player_->getDrawer()->setRotation(qRadiansToDegrees(angle));
    return rotated;
}

