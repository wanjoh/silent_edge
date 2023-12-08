#include "game_window.hpp"

#include <QKeyEvent>


GameWindow::GameWindow(quint32 width, quint32 height, QObject *parent)
    : QGraphicsScene(0, 0, width, height, parent)
    , window_width_(width)
    , window_height_(height)
{
    fight_phase_ = new QGraphicsView(this);
    fight_phase_->setBackgroundBrush(Qt::gray);

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

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    emit keyPressedSignal(event->key(), true);
}

void GameWindow::keyReleaseEvent(QKeyEvent *event)
{
    emit keyPressedSignal(event->key(), false);
}

void GameWindow::focusOutEvent(QFocusEvent *)
{
    emit focusedOutSignal();
}

