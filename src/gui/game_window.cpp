#include "game_window.hpp"

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

const int IMAGE_SIZE = 64;

GameWindow::GameWindow(Map* map, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(0, 0, width, height, parent)
    , window_width_(width)
    , window_height_(height)
    , map_object_(map)
    , map_(map_object_->get_matrix())
// todo: mrdunuti ovo gde treba
//    , room_(map_object_->add_player_to_a_room(player))
{
    addItem(map_object_->get_group());

    std::pair<int, int> start_coords = room_->get_start_coords();
    window_width_ = room_->get_width()*IMAGE_SIZE;
    window_height_ = room_->get_height()*IMAGE_SIZE;

    start_x_ = start_coords.first*IMAGE_SIZE;
    start_y_ = start_coords.second*IMAGE_SIZE;

    fight_phase_ = new QGraphicsView(this);
    fight_phase_->setSceneRect(start_x_, start_y_, window_width_, window_height_);
    fight_phase_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fight_phase_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fight_phase_->setBackgroundBrush(Qt::gray);
    fight_phase_->setMouseTracking(true);

    fight_phase_->installEventFilter(this);

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

bool GameWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == fight_phase_ && event->type() == QEvent::Resize) {
        QResizeEvent *resizeEvent = static_cast<QResizeEvent*>(event);

        width_zoom_level_ *= (qreal)resizeEvent->size().width() / window_width_;
        height_zoom_level_ *= (qreal)resizeEvent->size().height() / window_height_;
        fight_phase_->setTransform(QTransform::fromScale(width_zoom_level_, height_zoom_level_));

        window_width_ = resizeEvent->size().width();
        window_height_ = resizeEvent->size().height();

        return QGraphicsScene::eventFilter(obj, event);
    }
    return false;
}
