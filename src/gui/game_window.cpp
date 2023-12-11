#include "game_window.hpp"

#include <QKeyEvent>


GameWindow::GameWindow(Map* map, EntityDrawer* player, quint32 width, quint32 height, QObject *parent)
    : QGraphicsScene(0, 0, width, height, parent)
    , window_width_(width)
    , window_height_(height)
    , controllable_player_(player)
    , map_object_(map)
    , map_(map->initialize_matrix())
{
    addItem(map_object_->get_group());
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

    int x1 = x/64;
    int y1 = y/64;

    QVector<QString> names;
    for(int i = x1; i < x1 + 2; i++) {
        for(int j = y1; j < y1 + 2; j++) {
            QString name = QString("%1 %2").arg(i).arg(j);
            names.push_back(name);
        }
    }

    // magicni broj IMAGE_SIZE = 64, samo za sada, pa da se kasnije dogovorimo kako cemo da cuvamo(koristi se na vise mesta)
    bool can_move = canPlayerMove(names);
    std::unordered_map<QString, Tile*> active_buckets = map_object_->get_active_ammo_buckets();

    if (moved && can_move) {
        controllable_player_->setPos(x, y);

        for(QString &name : names) {
            if(active_buckets.contains(name)) {
                QPair<int, int> coords = map_[name]->get_coords();
                map_object_->remove_tile(name);
                map_object_->add_ground_tile_of_type_ammo(name, coords.first, coords.second);

                qDebug() << "tile " << name << " was deeleted";

                emit tileDeleted(name);
            }
        }
    }

    emit playerMoved();
}

void GameWindow::updateAmmo()
{
    map_object_->restock_ammo_piles();
}

bool GameWindow::canPlayerMove(QVector<QString> names)
{
    bool can_move = true;

    for(QString &name : names) {
        Tile* tile = map_[name];
        if(tile && tile->getTileType() == Tile::TileType::WALL) {
            can_move = false;
        }
    }

    return can_move;
}

void GameWindow::initializeTimers()
{
    movement_timer_.setInterval(16);  // Approximately 60 FPS
    connect(&movement_timer_, &QTimer::timeout, this, &GameWindow::updateMovement);
    movement_timer_.start();

    ammo_timer_.setInterval(5000);
    connect(&ammo_timer_, &QTimer::timeout, this, &GameWindow::updateAmmo);
    ammo_timer_.start();
}
