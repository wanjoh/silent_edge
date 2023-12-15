#include "game_logic_handler.hpp"
#include <QtMath>

GameLogicHandler::GameLogicHandler(QString name, QObject* parent)
    : QObject(parent)
    , player_(new Player(name, false))
{
    player_->getDrawer()->rect().translate(QPointF(25, 25));
    player_->addWeapon(new Weapon("Gun", Weapon::RarenessType::COMMON, 220.0, 10.0, 6.6));
    player_->addWeapon(new Weapon("Gun2", Weapon::RarenessType::COMMON, 220.0, 10.0, 8.6));
    current_weapon_ = player_->currentWeapon();
    emit weaponDrawSignal(current_weapon_->getName(), current_weapon_->getDrawer());
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
    emit newBulletSignal(bullet_name, bullet->getDrawer());
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

    moved |= updateRotation();

    if (moved)
    {
        emit playerMoved(player_->toVariant());
        player_->getDrawer()->setPos(x, y);
    }
}

void GameLogicHandler::shoot()
{

    if (keys_[Qt::LeftButton])
    {
        Bullet *bullet = new Bullet(player_->getName());
        QPointF top_left = player_->getDrawer()->mapToScene(player_->getDrawer()->rect().topLeft());
        QPointF top_right = player_->getDrawer()->mapToScene(player_->getDrawer()->rect().topRight());
        QPointF top_center = QPointF((top_left.x() + top_right.x()) / 2, (top_left.y() + top_right.y()) / 2);

        QLineF line(top_center, aiming_point_);


        QVector2D vectorFromLine(line.dx(), line.dy());
        vectorFromLine.normalize();

        qreal angle = player_->getDrawer()->rotation() - 90;

        QTransform transform;
        transform.rotate(angle);
        bullet->getDrawer()->setTransform(transform);
        bullet->getDrawer()->setPos(top_center + QPointF(0, bullet->getDrawer()->rect().height()));

        bullet->setAim_dir(vectorFromLine);
        addBullet(player_->getName(),bullet);
    }

}

void GameLogicHandler::updateBullets()
{

    if(!(bullets_[player_->getName()].empty())) {


        for (Bullet* bullet : bullets_[player_->getName()])
        {
            qreal x_pos = bullet->getDrawer()->scenePos().x() + 10 * bullet->aim_dir().x();
            qreal y_pos = bullet->getDrawer()->scenePos().y() + 10 * bullet->aim_dir().y();

            bullet->getDrawer()->setPos(x_pos, y_pos);
        }

    }

}


void GameLogicHandler::checkCollisions(Bullet* bullet){

    QList<QGraphicsItem*> colidingItems = bullet->getDrawer()->collidingItems();

    foreach(QGraphicsItem* item, colidingItems){

        if(typeid(*item) == typeid(Player)){


            Player* player = dynamic_cast<Player*>(item);

            qDebug() << "bullet collision";


            // player->decreaseHp(this);


            // emit destroyBullet(bullet->getName());

            // if(player->getHp() == 0)
            //     player->destroy();



            break;

        }
    }

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
    connect(&shooting_timer_, &QTimer::timeout, this, &GameLogicHandler::shoot);
    connect(&movement_timer_, &QTimer::timeout, this, &GameLogicHandler::updateBullets);

    connect(this,&GameLogicHandler::bulletUpdating,this,&GameLogicHandler::checkCollisions);
    movement_timer_.start();
    shooting_timer_.start();
}

bool GameLogicHandler::updateRotation()
{
    QPointF top_left = player_->getDrawer()->mapToScene(player_->getDrawer()->rect().topLeft());
    QPointF top_right = player_->getDrawer()->mapToScene(player_->getDrawer()->rect().topRight());
    QPointF top_center = QPointF((top_left.x() + top_right.x()) / 2, (top_left.y() + top_right.y()) / 2);

    QLineF line(top_center, aiming_point_);
    qreal angle = -line.angle();
    bool rotated = qFabs(angle - player_->getDrawer()->rotation()) > EPSILON;
    player_->getDrawer()->setRotation(angle);
    return rotated;
}
