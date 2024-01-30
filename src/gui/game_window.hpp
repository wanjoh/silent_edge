#pragma once

#include <QGraphicsScene>
#include "entity_drawer.hpp"
#include "../server/server_config.hpp"

// mogao bi room drawer ili stagod, samo da se ovo izbaci
#include "../map/room.hpp"

class GameWindow : public QGraphicsScene
{
    Q_OBJECT
public:
    static constexpr qreal WINDOW_WIDTH = 1280;
    static constexpr qreal WINDOW_HEIGHT = 720;
    GameWindow(Room *, qreal = WINDOW_WIDTH, qreal = WINDOW_HEIGHT, QObject* = nullptr);
    ~GameWindow() override;

    enum class GamePhase : quint32
    {
        PREP_PHASE = 0,
        FIGHT_PHASE
    };
    void show(GamePhase);

    void keyPressEvent(QKeyEvent * /*event*/) override;
    void keyReleaseEvent(QKeyEvent * /*event*/) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent * /*event*/) override;
    void mousePressEvent(QGraphicsSceneMouseEvent * /*event*/) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * /*event*/) override;
    void focusOutEvent(QFocusEvent * /*event*/) override;
    auto eventFilter(QObject *obj, QEvent *event) -> bool override;
    void make_overlay();
    void setSceneUp();
    void changeRoom(Room *new_room);
    void teleportPlayer(const QString &name, qreal x, qreal y);
    auto getMovement() const -> quint32;
    auto getMouseX() -> qreal;
    auto getMouseY() -> qreal;

    void resetMovement();
    void addEntity(const QString&, EntityDrawer*);
    void removeEntity(const QString&);
    void updateBulletsLabel(qint32, qint32, qint32);
signals:
    void focusedOutSignal();
    void mousePos(QPointF);
public slots:
    void change_weapon(int id);
    void update_hp_overlay(qreal hp);
private:
    using uint = unsigned int;
    std::map<uint, uint> movement_map_
    {
        {static_cast<uint>(Qt::Key_W), static_cast<uint>(ServerConfig::PlayerActions::UP)},
        {static_cast<uint>(Qt::Key_A), static_cast<uint>(ServerConfig::PlayerActions::LEFT)},
        {static_cast<uint>(Qt::Key_D), static_cast<uint>(ServerConfig::PlayerActions::RIGHT)},
        {static_cast<uint>(Qt::Key_S), static_cast<uint>(ServerConfig::PlayerActions::DOWN)},
        {static_cast<uint>(Qt::LeftButton), static_cast<uint>(ServerConfig::PlayerActions::SHOOT)},
        {static_cast<uint>(Qt::RightButton), static_cast<uint>(ServerConfig::PlayerActions::MELEE)},
        {static_cast<uint>(Qt::Key_R), static_cast<uint>(ServerConfig::PlayerActions::RELOAD)}
    };
    quint32 movement_;

    qreal width_zoom_level_;
    qreal height_zoom_level_;
    quint32 window_width_;
    quint32 window_height_;
    GamePhase current_active_phase_;

    Room* room_;
    std::unordered_map<QString, EntityDrawer*> items_;
    QGraphicsView *phase_;
    qreal start_x_;
    qreal start_y_;

    QGraphicsPixmapItem *pistol_overlay_;
    QGraphicsItemGroup *overlay_group_;
    QGraphicsTextItem *bullets_label_;
    QGraphicsTextItem *remaining_bullets_label_;
    QGraphicsRectItem *hp_overlay_;
};
