#include "game_window.hpp"

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

GameWindow::GameWindow(quint32 width, quint32 height, QObject *parent)
    : QGraphicsScene(0, 0, width, height, parent)
    , window_width_(width)
    , window_height_(height)
{
    fight_phase_ = new QGraphicsView(this);
    fight_phase_->setBackgroundBrush(Qt::gray);
    fight_phase_->setMouseTracking(true);

    // todo: promeniti
    current_active_phase_ = GamePhase::FIGHT_PHASE;
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

void GameWindow::removeEntity(QString name)
{
    delete items_[name];
    items_.erase(name);
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    emit keyPressedSignal(event->key(), true);
}

void GameWindow::keyReleaseEvent(QKeyEvent *event)
{
    emit keyPressedSignal(event->key(), false);
}

void GameWindow::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit mousePos(event->scenePos());
}

void GameWindow::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit mousePressedSignal(event->button(), true);
}

void GameWindow::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit mousePressedSignal(event->button(), false);
}

void GameWindow::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    emit wheelScrollSignal(event->delta());
}



void GameWindow::focusOutEvent(QFocusEvent *event)
{
    emit focusedOutSignal();
}

