#include "game_logic_handler.hpp"


GameLogicHandler::GameLogicHandler(Player* player)
    : controllable_player_(player)
{
    initializeTimers();
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

void GameLogicHandler::updateMovement()
{
    qreal x = controllable_player_->getDrawer()->x();
    qreal y = controllable_player_->getDrawer()->y();

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

    if (moved)
    {
        emit playerMoved();
        controllable_player_->getDrawer()->setPos(x, y);
    }
}

void GameLogicHandler::initializeTimers()
{
    movement_timer_.setInterval(1000.0 / TARGET_FPS);  // Approximately 60 FPS
    connect(&movement_timer_, &QTimer::timeout, this, &GameLogicHandler::updateMovement);
    movement_timer_.start();
}
