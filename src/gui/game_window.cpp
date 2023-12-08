#include "game_window.hpp"

#include <QKeyEvent>


GameWindow::GameWindow(EntityDrawer* player, QGraphicsItemGroup* map, quint32 width, quint32 height, QObject *parent)
    : QGraphicsScene(0, 0, width, height, parent)
    , window_width_(width)
    , window_height_(height)
    , controllable_player_(player)
    , map_(map)
{
    addItem(controllable_player_);
    addItem(map_);

    fight_phase_ = new QGraphicsView(this);
    fight_phase_->setBackgroundBrush(Qt::gray);

    // todo: promeniti
    current_active_phase_ = GamePhase::FIGHT_PHASE;
    initializeTimers();
}

GameWindow::~GameWindow()
{
    for (auto& [_, item] : items_)
    {
        delete item;
    }
}

void GameWindow::show(GamePhase phase)
{
    switch(phase)
    {
    case GamePhase::FIGHT_PHASE:
        fight_phase_->show();
        break;
    default:
        qDebug() << "not supported yet";
        break;
    }
}

void GameWindow::addEntity(QString name, EntityDrawer* entity)
{
    items_[name] = entity;
    addItem(entity);
}

void GameWindow::updatePosition(QString name, std::pair<qreal, qreal> pos, qreal rot)
{
    items_[name]->setPos(pos.first, pos.second);
    items_[name]->setRotation(rot);
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    keys_[event->key()] = true;

}

void GameWindow::keyReleaseEvent(QKeyEvent *event)
{
    keys_[event->key()] = false;

}

void GameWindow::updateMovement()
{
    qreal x = controllable_player_->x();
    qreal y = controllable_player_->y();

    bool moved = false;

    qreal dx = (keys_[Qt::Key_D] ? 1.0 : 0.0) - (keys_[Qt::Key_A] ? 1.0 : 0.0);
    qreal dy = (keys_[Qt::Key_S] ? 1.0 : 0.0) - (keys_[Qt::Key_W] ? 1.0 : 0.0);

    if (dx != 0.0 && dy != 0.0) {
        qreal length = qSqrt(dx * dx + dy * dy);
        dx /= length;
        dy /= length;
    }


    x += dx * controllable_player_->getVelocity();
    y += dy * controllable_player_->getVelocity();

    if (dx != 0.0 || dy != 0.0) {
        moved = true;
    }

    if (moved) {
        controllable_player_->setPos(x, y);
        emit playerMoved();
    }
}

void GameWindow::initializeTimers()
{
    movement_timer_.setInterval(16);  // Approximately 60 FPS
    connect(&movement_timer_, &QTimer::timeout, this, &GameWindow::updateMovement);
    movement_timer_.start();
}
