#include "game_logic_handler.hpp"
#include <QtMath>
#include <QPen>

GameLogicHandler::GameLogicHandler(QString name, Map *map, QObject* parent)
    : QObject(parent)
    , map_object_(map)
    , map_(map_object_->get_matrix())
    , player_(new Player(name, false))
{
    player_->getDrawer()->setPos(2*IMAGE_SIZE, 2*IMAGE_SIZE);
    initializeTimers();
<<<<<<< HEAD
=======
    reload_drawer_ = new EntityDrawer("reload", "../silent-edge/src/images/reload.png");
    reload_drawer_->setZValue(3);
>>>>>>> weapon

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
    if (button == Qt::RightButton)
    {
        if(pressed && !swing_timer_.isActive())
        {
            emit meleeSwingSignal(player_->getMeleeWeapon()->getName(), player_->getMeleeWeapon()->getDrawer());
            swing_timer_.start();
        }
    }
    else if (button == Qt::LeftButton && pressed && !shooting_cooldown_timer_.isActive() && !reload_timer_.isActive())
    {
        if (player_bullet_count_[player_->getName()] < player_->getRangedWeapon()->getCapacity())
        {
            addBullet(player_->getName());
            shooting_cooldown_timer_.start();
            emit labelSignal(player_->getRangedWeapon()->getCapacity() - player_bullet_count_[player_->getName()], player_->getRangedWeapon()->getCapacity(), player_->getRangedWeapon()->getRemainingBullets());
        }
        else
        {
            qDebug() << "I need more bullets!";
        }
    }

}


void GameLogicHandler::addBullet(QString name)
{
    QMutexLocker locker(&mutex_);
    QString bullet_name = name + QString::number(player_bullet_count_[name]);
    Bullet* bullet = new Bullet(bullet_name);
    player_bullet_count_[name]++;
    bullets_[name].push_back(bullet);
    emit newBulletSignal(bullet_name, bullet->getDrawer());
    QPointF top_left = player_->getDrawer()->scenePos();
    QPointF top_right = player_->getDrawer()->mapToScene(player_->getDrawer()->pixmap().rect().topRight());
    QPointF top_center = (top_left + top_right) / 2;

    bullet->getDrawer()->setRotation(player_->getDrawer()->rotation());
    bullet->getDrawer()->setPos(top_center.x(),top_center.y()-bullet->BULLET_HEIGHT);

    emit bulletMoved(bullet->toVariant());

<<<<<<< HEAD
    qDebug() << "bullet added " << bullet_name;
=======


>>>>>>> weapon
}

void GameLogicHandler::updateMovement()
{
    if(keys_[Qt::Key_R])
    {
        if(player_bullet_count_[player_->getName()] != 0 && player_->getRangedWeapon()->getRemainingBullets() != 0)
        {
            reload_timer_.start();
            if(!reload_drawer_->isActive())
            {
                emit reloadItemSignal(reload_drawer_->name(), reload_drawer_);
            }
        }
    }

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

<<<<<<< HEAD
=======
    moved |= updateRotation();

>>>>>>> weapon
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
        reload_drawer_->setPos(x, y);

<<<<<<< HEAD
        for(int id : ids) {
            if(active_buckets.contains(id)) {
                map_[id]->setDrawer("../silent-edge/src/images/ground.png");
                map_object_->remove_from_active(id);

                emit tileDeleted(id);
=======
        for(QString& edge : edges) {
            if(active_buckets.contains(edge)) {
                QPair<int, int> coords = map_[edge]->get_coords();
                map_object_->remove_tile(edge);
                map_object_->add_ground_tile_of_type_ammo(edge, coords.first, coords.second);
                player_->getRangedWeapon()->setRemainingBullets(player_->getRangedWeapon()->getRemainingBullets() + AMMO_BUCKET_CAPCITY);
                emit labelSignal(player_->getRangedWeapon()->getCapacity() - player_bullet_count_[player_->getName()], player_->getRangedWeapon()->getCapacity(), player_->getRangedWeapon()->getRemainingBullets());
                emit tileDeleted(edge);
>>>>>>> weapon
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

<<<<<<< HEAD
    for(int edge : edges) {
=======
    for(QString& edge : edges) {
>>>>>>> weapon
        Tile* tile = map_[edge];
        if(tile && tile->getTileType() == Tile::TileType::WALL) {
            can_move = false;
        }
    }

    return can_move;
}

void GameLogicHandler::updateBullets()
{
<<<<<<< HEAD
    if (keys_[Qt::RightButton])
    {

    }
    else if (keys_[Qt::LeftButton])
    {
        addBullet(player_->getName());
    }
=======

>>>>>>> weapon
    QMutexLocker locker(&mutex_);
    for(auto& [_, bullets] : bullets_)
    {
        for(int i = bullets.size() - 1; i >= 0; i--)
        {
<<<<<<< HEAD
            qreal x_pos = bullets[i]->getDrawer()->scenePos().x() + 10 * bullets[i]->aim_dir().x();
            qreal y_pos = bullets[i]->getDrawer()->scenePos().y() + 10 * bullets[i]->aim_dir().y();


=======
            qreal x_pos = bullets[i]->getDrawer()->x() + 10 * qSin(qDegreesToRadians(bullets[i]->getDrawer()->rotation()));
            qreal y_pos = bullets[i]->getDrawer()->y() - 10 * qCos(qDegreesToRadians(bullets[i]->getDrawer()->rotation()));

>>>>>>> weapon
            bullets[i]->getDrawer()->setPos(x_pos, y_pos);

            //qDebug() << bullet->getDrawer()->scenePos().x();
            //            qDebug() << x_pos << " " << y_pos;
            //            emit bulletMoved(bullet->toVariant());

<<<<<<< HEAD
            if (checkCollisions(bullets[i]))
            {
                bullets.erase(bullets.begin() + i);
            }
=======

            checkCollisions(bullets[i]);


>>>>>>> weapon
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

<<<<<<< HEAD
bool GameLogicHandler::checkCollisions(Bullet* bullet)
{
    QList<QGraphicsItem*> coliding_items = bullet->getDrawer()->collidingItems();
=======

void GameLogicHandler::checkCollisions(Bullet* bullet){
    QList<QGraphicsItem*> colidingItems = bullet->getDrawer()->collidingItems();
>>>>>>> weapon

    foreach(QGraphicsItem *item, coliding_items)
    {
        QGraphicsPixmapItem* pixmap_item = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(pixmap_item)
        {
            if(typeid(*pixmap_item) == typeid(PlayerDrawer))
            {
                //Player* player = dynamic_cast<Player>(item);

                PlayerDrawer *player_drawer = static_cast<PlayerDrawer*>(pixmap_item);

                QString enemy_name = player_drawer->name();
<<<<<<< HEAD
                // ako je igrac nas, on nece biti u enemies_
                // todo: OVO CE SE SVE IZMENITI KAD PREBACIMO NA SERVER, TRENUTNO JE VELIKO MEGA SPAGETI KOD
                if (enemy_name == player_->getName())
                {
                    decreaseHp(player_, bullet);
                    return true;
                }
=======
                //qDebug() << player_drawer->name();

>>>>>>> weapon
                //qDebug() << "collision with " << enemies_[enemy_name]->getName();
                if(player_drawer->name() == player_->getName())
                {
                    return;
                }

                Player *enemy = enemies_[enemy_name];

                decreaseHp(enemy,bullet);

                qDebug() << enemy_name << " has " << enemy->getHp() << "hp";

<<<<<<< HEAD
=======

>>>>>>> weapon
            }
            if(typeid(*pixmap_item) == typeid(TileDrawer))
            {

                TileDrawer *tile_drawer = static_cast<TileDrawer*>(pixmap_item);


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
<<<<<<< HEAD

    emit update_hp(player->getHp());
=======
>>>>>>> weapon
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

void GameLogicHandler::reload()
{
    reload_timer_.stop();

    quint32 bullets_shot = player_bullet_count_[player_->getName()];
    player_bullet_count_[player_->getName()] -= std::min(player_->getRangedWeapon()->getRemainingBullets(), static_cast<qint32>(player_bullet_count_[player_->getName()]));
    player_->getRangedWeapon()->setRemainingBullets(std::max(player_->getRangedWeapon()->getRemainingBullets() - static_cast<qint32>(bullets_shot), 0));


    emit removeReload(reload_drawer_->name());
    emit labelSignal(player_->getRangedWeapon()->getCapacity() - player_bullet_count_[player_->getName()], player_->getRangedWeapon()->getCapacity(), player_->getRangedWeapon()->getRemainingBullets());

}

void GameLogicHandler::initializeTimers()
{
    movement_timer_.setInterval(1000 / TARGET_FPS);
    shooting_cooldown_timer_.setInterval(player_->getRangedWeapon()->getShotCooldown());
    shooting_cooldown_timer_.setSingleShot(true);
    reload_timer_.setInterval(player_->getRangedWeapon()->getReloadTime());
    reload_timer_.setSingleShot(true);
    swing_timer_.setInterval(player_->getMeleeWeapon()->getSwingTime());
    swing_timer_.setSingleShot(true);
    connect(&reload_timer_, &QTimer::timeout, this, &GameLogicHandler::reload);
    connect(&shooting_cooldown_timer_, &QTimer::timeout, this, [this]()
    {
        shooting_cooldown_timer_.stop();
    });
    connect(&swing_timer_, &QTimer::timeout, this, [this]()
            {
                emit removeMelee(player_->getMeleeWeapon()->getName());
                swing_timer_.stop();
            });
    connect(&movement_timer_, &QTimer::timeout, this, &GameLogicHandler::updateMovement);
    // za sad se koristi isti tajmer
    connect(&movement_timer_, &QTimer::timeout, this, &GameLogicHandler::updateBullets);

    movement_timer_.start();

    ammo_respawn_timer_.setInterval(5000);
    connect(&ammo_respawn_timer_, &QTimer::timeout, this, &GameLogicHandler::updateAmmo);
    ammo_respawn_timer_.start();
}

bool GameLogicHandler::updateRotation()
{
    if(player_->getDrawer()->contains(player_->getDrawer()->mapFromScene(aiming_point_)))
        return false;
    QPointF top_left = player_->getDrawer()->scenePos();
    QPointF top_right = player_->getDrawer()->mapToScene(player_->getDrawer()->pixmap().rect().topRight());
    QPointF top_center = (top_left + top_right) / 2;
    QPointF aim_dir = aiming_point_ - top_center;

    player_->getMeleeWeapon()->getDrawer()->setPos(top_right.x(), top_right.y());
    qreal angle = qAtan2(aim_dir.x(), -aim_dir.y());
    angle = qRadiansToDegrees(angle);

    bool rotated = qFabs(angle - player_->getDrawer()->rotation()) > EPSILON;
    player_->getDrawer()->setRotation(angle);
    player_->getMeleeWeapon()->getDrawer()->setRotation(angle);

    return rotated;
}

