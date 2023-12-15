#include "game_logic_handler.hpp"

#include <QtMath>
GameLogicHandler::GameLogicHandler(QString name, Map *map, QObject* parent)
    : QObject(parent)
    , map_object_(map)
    , map_(map->initialize_matrix())
    , player_(new Player(name, false))
{
    player_->addWeapon(new Weapon("Gun", Weapon::RarenessType::COMMON, 220.0, 10.0, 6.6));
    player_->addWeapon(new Weapon("Gun2", Weapon::RarenessType::COMMON, 220.0, 10.0, 8.6));
    current_weapon_ = player_->currentWeapon();

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

void GameLogicHandler::updateMouseScroll(qint32 delta)
{
    if(delta > 0)
    {
        current_weapon_ = player_->nextWeapon();
        shooting_timer_.setInterval(1000 / current_weapon_->getFireRate());
    }
    else if(delta < 0)
    {
        current_weapon_ = player_->previousWeapon();
        shooting_timer_.setInterval(1000 / current_weapon_->getFireRate());
    }
}


void GameLogicHandler::addBullet(QString name, Bullet* bullet)
{
    QMutexLocker locker(&mutex_);
    QString bullet_name = name + QString::number(player_bullet_count_[name]);
    player_bullet_count_[name]++;
    bullets_[name].push_back(bullet);
    bullet->setName(bullet_name);
    emit newBulletSignal(bullet_name, bullet->getDrawer());


    QVector2D aim_dir = QVector2D(aiming_point_-player_->getDrawer()->scenePos());

    aim_dir.normalize();

    bullet->setAim_dir(aim_dir);

    bullet->getDrawer()->setPos(player_->getDrawer()->scenePos().x(),player_->getDrawer()->scenePos().y()-1.1*bullet->BULLET_HEIGHT);

    qDebug() << bullet_name;
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


    int x1 = x/64;
    int y1 = y/64;


    QVector<QString> names;
    for(int i = x1; i < x1 + 2; i++) {
        for(int j = y1; j < y1 + 2; j++) {
            QString name = QString("%1 %2").arg(i).arg(j);
            names.push_back(name);
        }
    }

    bool can_move = canPlayerMove(names);
    std::unordered_map<QString, Tile*> active_buckets = map_object_->get_active_ammo_buckets();

    if (moved && can_move) {
        player_->getDrawer()->setPos(x, y);

        for(QString &name : names) {
            if(active_buckets.contains(name)) {
                QPair<int, int> coords = map_[name]->get_coords();
                map_object_->remove_tile(name);
                map_object_->add_ground_tile_of_type_ammo(name, coords.first, coords.second);

                emit tileDeleted(name);
            }
        }
    }

    emit playerMoved(player_->toVariant());
}


void GameLogicHandler::updateAmmo()
{
    map_object_->restock_ammo_piles();
}

bool GameLogicHandler::canPlayerMove(QVector<QString> names)
{
    bool can_move = true;

    for(QString &name : names) {
        Tile* tile = map_[name];
        if(tile && tile->getTileType() == Tile::TileType::WALL) {
            can_move = false;
        }
    }

    return can_move;
}


void GameLogicHandler::updateBullets()
{
    if (keys_[Qt::LeftButton])
    {
        Bullet *bullet = new Bullet(player_->getName());

        addBullet(player_->getName(),bullet);
    }

    for(auto& [_, bullets] : bullets_)
    {
        for(Bullet *bullet : bullets)
        {
            qreal x_pos = bullet->getDrawer()->scenePos().x() + 10 * bullet->aim_dir().x();
            qreal y_pos = bullet->getDrawer()->scenePos().y() + 10 * bullet->aim_dir().y();


            bullet->getDrawer()->setPos(x_pos, y_pos);

            //qDebug() << bullet->getDrawer()->scenePos().x();
            //            qDebug() << x_pos << " " << y_pos;
            //            emit bulletMoved(bullet->toVariant());

            emit bulletUpdating(bullet);

            emit checkCollisions(bullet);


        }
    }
}


void GameLogicHandler::checkCollisions(Bullet* bullet){
    QList<QGraphicsItem*> colidingItems = bullet->getDrawer()->collidingItems();

    foreach(QGraphicsItem* item, colidingItems){
        if(typeid(*item) == typeid(Player)){
            Player* player = dynamic_cast<Player*>(item);

            qDebug() << "bullet collision";

            decreaseHp(player,bullet);


            // player->decreaseHp(this);


            // emit destroyBullet(bullet->getName());

            // if(player->getHp() == 0)
            //     player->destroy();



            if(player->getHp() == 0)
                emit destroyPlayer(player->getName());
            break;

        }
        //emit destroyBullet(bullet->getName());


        }

    }

void GameLogicHandler::decreaseHp(Player* player, Bullet* bullet)
{
    qreal player_hp = player->getHp();
    qreal bullet_damage = bullet->getDamageDealt();
    player->setHp(player_hp - bullet_damage);
}

void GameLogicHandler::updateAimingPoint(QPointF point)
{
    aiming_point_ = point;
}

void GameLogicHandler::initializeTimers()
{
    movement_timer_.setInterval(1000 / TARGET_FPS);
    shooting_timer_.setInterval(1000 / current_weapon_->getFireRate());
    connect(&movement_timer_, &QTimer::timeout, this, &GameLogicHandler::updateMovement);
    // za sad se koristi isti tajmer
    connect(&movement_timer_, &QTimer::timeout, this, &GameLogicHandler::updateBullets);

    connect(this,&GameLogicHandler::bulletUpdating,this,&GameLogicHandler::checkCollisions);
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

