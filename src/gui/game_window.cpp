#include "game_window.hpp"

#include <QKeyEvent>


GameWindow::GameWindow(Map* map, EntityDrawer* player, quint32 width, quint32 height, QObject *parent)
    : QGraphicsScene(0, 0, width, height, parent)
    , window_width_(width)
    , window_height_(height)
    , controllable_player_(player)
    , map_(map)
    , matrix_(map->get_matrix())
{
    addItem(map_->draw_matrix());
    addItem(controllable_player_);

    fight_phase_ = new QGraphicsView(this);
    fight_phase_->setFixedSize(width, height);
    fight_phase_->setSceneRect(0, 0, width, height);
    fight_phase_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fight_phase_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fight_phase_->setBackgroundBrush(Qt::gray);

    // todo: promeniti
    current_active_phase_ = GamePhase::FIGHT_PHASE;
    initializeTimers();
}

GameWindow::~GameWindow()
{
    for (auto& [_, item] : items_)
        delete item;

    for (auto& [_, tile] : tiles_)
        delete tile;
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

void GameWindow::addTile(QString name, TileDrawer* tile)
{
    tiles_[name] = tile;
    addItem(tile);
}

void GameWindow::deleteTile(QString name)
{
    delete tiles_[name];
    delete items_[name];
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

    bool can_move = canPlayerMove(x, y);

    if (moved && can_move) {
        controllable_player_->setPos(x, y);
        emit playerMoved();

        QString name = map_->get_name(y/64, x/64);
        if (map_->get_ammo_piles().contains(name)) {
            map_->remove_name_from_ammo_list(name);
            deleteTile(name);
        }

    }
}

bool GameWindow::canPlayerMove(qreal x, qreal y)
{
    bool can_move = true;

    // magicni broj IMAGE_SIZE = 64, samo za sada, pa da se kasnije dogovorimo kako cemo da cuvamo(koristi se na vise mesta)

    int x1 = x / 64;
    int y1 = y / 64;

    if((*matrix_)[y1][x1] == 2 || (*matrix_)[y1+1][x1] == 2 || (*matrix_)[y1][x1+1] == 2 || (*matrix_)[y1+1][x1+1] == 2)
        can_move = false;

    return can_move;
}

void GameWindow::initializeTimers()
{
    movement_timer_.setInterval(16);  // Approximately 60 FPS
    connect(&movement_timer_, &QTimer::timeout, this, &GameWindow::updateMovement);
    movement_timer_.start();
}
