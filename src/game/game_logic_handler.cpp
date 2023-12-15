#include "game_logic_handler.hpp"
#include <QtMath>

const int IMAGE_SIZE = 64;

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


void GameLogicHandler::addBullet(QString name, Bullet* bullet)
{
    // thread safe?
    QString bullet_name = name + QString::number(player_bullet_count_[name]);
    player_bullet_count_[name]++;
    bullets_[name].push_back(bullet);
    bullet->setName(bullet_name);
    emit newBulletSignal(bullet_name, bullet->getDrawer());

    QVector2D aim_dir = QVector2D(aiming_point_-player_->getDrawer()->scenePos());

    aim_dir.normalize();

    bullet->setAim_dir(aim_dir);

    bullet->getDrawer()->setPos(player_->getDrawer()->scenePos().x(),player_->getDrawer()->scenePos().y()-1.1*bullet->BULLET_HEIGHT);

    emit bulletMoved(bullet->toVariant());

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
        player_->getDrawer()->setPos(x, y);

        for(QString edge : edges) {
            if(active_buckets.contains(edge)) {
                QPair<int, int> coords = map_[edge]->get_coords();
                map_object_->remove_tile(edge);
                map_object_->add_ground_tile_of_type_ammo(edge, coords.first, coords.second);

                emit tileDeleted(edge);
            }
        }
    }

    emit playerMoved(player_->toVariant());
}

void GameLogicHandler::updateAmmo()
{
    map_object_->restock_ammo_piles();
}

bool GameLogicHandler::canEntityMove(QVector<QString> &edges)
{
    bool can_move = true;

    for(QString edge : edges) {
        Tile* tile = map_[edge];
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

                Player *enemy = enemies_[enemy_name];

                decreaseHp(enemy,bullet);

                qDebug() << enemy_name << " has " << enemy->getHp() << "hp";

                if(enemy->getHp() == 0)
                   emit destroyPlayer(enemy->getName());

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
                    emit destroyBullet(bullet->getName());
                    break;
                }

            }

            //emit destroyBullet(bullet->getName());
        }
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
    connect(&movement_timer_, &QTimer::timeout, this, &GameLogicHandler::updateMovement);
    // za sad se koristi isti tajmer
    connect(&movement_timer_, &QTimer::timeout, this, &GameLogicHandler::updateBullets);
    connect(this,&GameLogicHandler::bulletUpdating,this,&GameLogicHandler::checkCollisions);
    movement_timer_.start();

    ammo_respawn_timer_.setInterval(5000);
    connect(&ammo_respawn_timer_, &QTimer::timeout, this, &GameLogicHandler::updateAmmo);
    ammo_respawn_timer_.start();
}

bool GameLogicHandler::updateRotation()
{
    qreal angle = atan2(aiming_point_.y() - player_->getDrawer()->scenePos().y(),
                        aiming_point_.x() - player_->getDrawer()->scenePos().x());
    bool rotated = qFabs(angle - player_->getDrawer()->rotation()) > EPSILON;
    player_->getDrawer()->setRotation(qRadiansToDegrees(angle));
    return rotated;
}
