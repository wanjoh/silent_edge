#include "game_window.hpp"

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include "../map/overlay.hpp"

const int IMAGE_SIZE = 64;

GameWindow::GameWindow(Map* map, EntityDrawer* player, qreal width, qreal height, QObject *parent)
    : QGraphicsScene(0, 0, width, height, parent)
    , window_width_(width)
    , window_height_(height)
    , map_object_(map)
    , map_(map_object_->get_matrix())
    , room_(map_object_->add_player_to_a_room(player))
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

    overlay_group_ = new QGraphicsItemGroup();
    make_overlay();
    addItem(overlay_group_);

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
    removeItem(items_[name]);
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

void GameWindow::make_overlay()
{
    QString base_path = "../silent-edge/src/images/";
    QVector<QString> weapon_overlay_paths = {base_path + "pistol_overlay.png", base_path + "pistol_overlay_2.png"};

    int x_pos = 0;
    int y_pos = start_y_ + window_height_ - 3 * IMAGE_SIZE;

    int n = weapon_overlay_paths.size();
    for(int i = 0; i < n; i++) {
        Overlay *overlay = new Overlay(i, weapon_overlay_paths[i], {start_x_/IMAGE_SIZE + x_pos/IMAGE_SIZE, start_y_/IMAGE_SIZE + y_pos/IMAGE_SIZE});
        OverlayDrawer *drawer = overlay->getDrawer();
        drawer->setPos(start_x_+ x_pos, start_y_ + y_pos);

        if(i != 0)
            drawer->setVisible(false);
        overlay_group_->addToGroup(drawer);
    }

    int hp_x_pos = start_x_ + window_width_ - 1 * IMAGE_SIZE;
    int hp_y_pos = start_y_ + window_height_ - 1 * IMAGE_SIZE;
    QGraphicsRectItem *hp_overlay = new QGraphicsRectItem(hp_x_pos, hp_y_pos, IMAGE_SIZE, IMAGE_SIZE);
    hp_overlay->setBrush(Qt::green);

    overlay_group_->addToGroup(hp_overlay);
    overlay_group_->setZValue(0);
}

void GameWindow::change_weapon(int id)
{
    QList<QGraphicsItem*> child_items = overlay_group_->childItems();

    int i = 0;
    for (auto it = child_items.begin(); it != child_items.end(); it++) {
        QGraphicsItem* current_item = *it;
        if (current_item) {
            if (i != id)
                current_item->setVisible(false);
            else
                current_item->setVisible(true);
        }
        i++;
    }
}

void GameWindow::update_hp_overlay(qreal hp)
{
    qreal percentage = hp/100;

    QList<QGraphicsItem*> childItems = overlay_group_->childItems();
    if (!childItems.isEmpty()) {
        qreal x_pos = childItems.last()->x();
        qreal y_pos = childItems.last()->y();
        childItems.last()->setPos(x_pos, y_pos + percentage);
    }
}
