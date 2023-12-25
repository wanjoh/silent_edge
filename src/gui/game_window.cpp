#include "game_window.hpp"

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

GameWindow::GameWindow(Room* room, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(0, 0, width, height, parent)
    , window_width_(width)
    , window_height_(height)
    , width_zoom_level_(1.0)
    , height_zoom_level_(1.0)
    , room_(room)
    , movement_(0)
{
    phase_ = new QGraphicsView(this);
    phase_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    phase_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    phase_->setBackgroundBrush(Qt::gray);
    phase_->setMouseTracking(true);
    phase_->installEventFilter(this);

    setSceneUp();
}

GameWindow::~GameWindow()
{
    for (auto& [_, item] : items_)
    {
        delete item;
    }

}

void GameWindow::changeRoom(Room *new_room)
{
    room_ = new_room;
}

quint32 GameWindow::getMovement()
{
    // ne toliko thread safe, moze se desiti da se movement promeni usred slanja
    // todo: poboljsati ako ostane vremena
    return movement_;
}

void GameWindow::resetMovement()
{
    movement_ = 0;
}

void GameWindow::setSceneUp()
{
    std::pair<int, int> start_coords = room_->getStartCoords();
    window_width_ = room_->getWidth()*IMAGE_SIZE;
    window_height_ = room_->getHeight()*IMAGE_SIZE;

    start_x_ = start_coords.first*IMAGE_SIZE;
    start_y_ = start_coords.second*IMAGE_SIZE;

    phase_->setSceneRect(start_x_, start_y_, window_width_, window_height_);
}

void GameWindow::show(GamePhase phase)
{
    switch(phase)
    {
        case GamePhase::FIGHT_PHASE:
            phase_->show();
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
    if (movement_map_.find(event->key()) != movement_map_.end())
    {
        movement_ |= movement_map_[static_cast<uint>(event->key())];
    }
}

void GameWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (movement_map_.find(event->key()) != movement_map_.end())
    {
        movement_ &= ~movement_map_[static_cast<uint>(event->key())];
    }
}

void GameWindow::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit mousePos(event->scenePos());
}

void GameWindow::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (movement_map_.find(event->button()) != movement_map_.end())
    {
        movement_ |= movement_map_[static_cast<uint>(event->button())];
    }
}

void GameWindow::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (movement_map_.find(event->button()) != movement_map_.end())
    {
        movement_ &= ~movement_map_[static_cast<uint>(event->button())];
    }
}

void GameWindow::focusOutEvent(QFocusEvent *event)
{
    emit focusedOutSignal();
}

bool GameWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Resize) {
        QResizeEvent *resizeEvent = static_cast<QResizeEvent*>(event);

        width_zoom_level_ *= (qreal)resizeEvent->size().width() / window_width_;
        height_zoom_level_ *= (qreal)resizeEvent->size().height() / window_height_;
        phase_->setTransform(QTransform::fromScale(width_zoom_level_, height_zoom_level_));

        window_width_ = resizeEvent->size().width();
        window_height_ = resizeEvent->size().height();

        return QGraphicsScene::eventFilter(obj, event);
    }
    return false;
}

qreal GameWindow::getMouseX()
{
    return phase_->mapToScene(phase_->mapFromGlobal(QCursor::pos())).x();
}

qreal GameWindow::getMouseY()
{
    return phase_->mapToScene(phase_->mapFromGlobal(QCursor::pos())).y();
}
