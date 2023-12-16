#include "game_window.hpp"

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

GameWindow::GameWindow(MapDrawer* map_drawer, Room* room, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(0, 0, width, height, parent)
    , window_width_(width)
    , window_height_(height)
    , map_group_(map_drawer->get_group())
    , room_(room)
// todo: mrdunuti ovo gde treba
// limun: delim mapu na map i mapDrawer onda, urađeno
{
    addItem(map_group_);

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
    // limun: stvarno treba, ali to ću kada bude postojao globalni tajmer
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
    if (event->type() == QEvent::Resize) {
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
