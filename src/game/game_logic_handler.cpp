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

    if (dx != 0.0 && dy != 0.0)
    {
        qreal length = qSqrt(dx * dx + dy * dy);
        dx /= length;
        dy /= length;
    }


    x += dx * DEFAULT_PLAYER_VELOCITY;
    y += dy * DEFAULT_PLAYER_VELOCITY;

    if (dx != 0.0 || dy != 0.0)
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

void GameLogicHandler::updateBullets()
{
    // todo: promeniti u left mouse button
    if (keys_[Qt::Key_Space])
    {
        //todo: naci nacin na koji se metak pravi
        //addBullet(player_->getName(), bullet);
    }
}

void GameLogicHandler::updateAimingPoint(QPointF point)
{
    aiming_point_ = point;
}

void GameLogicHandler::initializeTimers()
{
    movement_timer_.setInterval(1000 / TARGET_FPS);
    connect(&movement_timer_, &QTimer::timeout, this, &GameLogicHandler::updateMovement);
    // za sad se koristi isti tajmer
    connect(&movement_timer_, &QTimer::timeout, this, &GameLogicHandler::updateBullets);
    movement_timer_.start();
}

bool GameLogicHandler::updateRotation()
{
    qreal angle = atan2(aiming_point_.y() - player_->getDrawer()->scenePos().y(),
                        aiming_point_.x() - player_->getDrawer()->scenePos().x());
    bool rotated = angle - player_->getDrawer()->rotation() > EPSILON;
    player_->getDrawer()->setRotation(qRadiansToDegrees(angle));
    return rotated;
}
