#include "game_logic_handler.hpp"
#include <QtMath>

GameLogicHandler::GameLogicHandler(QString name, QObject* parent)
    : QObject(parent)
    , player_(new Player(name, false))
{
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

    // moved |= updateRotation();

    if (moved)
    {
        emit playerMoved(player_->toVariant());
        player_->getDrawer()->setPos(x, y);
    }
}

void GameLogicHandler::updateBullets()
{
    if (keys_[Qt::LeftButton])
    {
        QVector2D aim_dir = QVector2D(aiming_point_-player_->getDrawer()->scenePos());

        aim_dir.normalize();

        Bullet *bullet = new Bullet(player_->getName());

        bullet->setAim_dir(aim_dir);

        addBullet(player_->getName(),bullet);

        bullet->getDrawer()->setPos(player_->getDrawer()->scenePos().x(),player_->getDrawer()->scenePos().y()-1.1*BULLET_HEIGHT);

        emit bulletMoved(bullet->toVariant());

        //todo: naci nacin na koji se metak pravi
        //addBullet(player_->getName(), bullet);
    }

    if(!(bullets_[player_->getName()].empty()))
    {
        for (Bullet* bullet : bullets_[player_->getName()])
        {
            qreal x_pos = bullet->getDrawer()->scenePos().x() + 10 * bullet->aim_dir().x();
            qreal y_pos = bullet->getDrawer()->scenePos().y() + 10 * bullet->aim_dir().y();

//            qDebug() << bullet->getDrawer()->scenePos().x();
//            qDebug() << x_pos << " " << y_pos;

            bullet->getDrawer()->setPos(x_pos, y_pos);

//            emit bulletMoved(bullet->toVariant());

            emit bulletUpdating(bullet);
        }
    }
}

void GameLogicHandler::checkCollisions(Bullet* bullet)
{
    QList<QGraphicsItem*> colidingItems = bullet->getDrawer()->collidingItems();

    foreach(QGraphicsItem* item, colidingItems)
    {
        if(typeid(*item) == typeid(Player))
        {
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

void GameLogicHandler::handleEnemy(QVariant variant)
{
    Player *enemy = new Player("enemy");
    enemy->fromVariant(variant);
    QString enemy_name = enemy->getName();

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
}

bool GameLogicHandler::updateRotation()
{
    qreal angle = atan2(aiming_point_.y() - player_->getDrawer()->scenePos().y(),
                        aiming_point_.x() - player_->getDrawer()->scenePos().x());
    bool rotated = qFabs(angle - player_->getDrawer()->rotation()) > EPSILON;
    player_->getDrawer()->setRotation(qRadiansToDegrees(angle));
    return rotated;
}
