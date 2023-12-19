#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include "entity_drawer.hpp"
#include "map_drawer.hpp"

// mogao bi room drawer ili stagod, samo da se ovo izbaci
#include "../map/room.hpp"

class GameWindow : public QGraphicsScene
{
    Q_OBJECT
public:
    static constexpr qreal WINDOW_WIDTH = 1280;
    static constexpr qreal WINDOW_HEIGHT = 720;
    GameWindow(MapDrawer*, Room*, qreal = WINDOW_WIDTH, qreal = WINDOW_HEIGHT, QObject* = nullptr);
    ~GameWindow();

    enum class GamePhase : quint32
    {
        PREP_PHASE = 0,
        FIGHT_PHASE
    };

    void show(GamePhase);

    void addEntity(QString, EntityDrawer*);
    void removeEntity(QString);

    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
    void wheelEvent(QGraphicsSceneWheelEvent*) override;
    void focusOutEvent(QFocusEvent*) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void setSceneUp();
    void changeRoom(Room *new_room);
signals:
    void keyPressedSignal(quint32, bool);
    void mousePressedSignal(Qt::MouseButton, bool);
    void wheelScrollSignal(qint32);
    void focusedOutSignal();
    void mousePos(QPointF);
private:
    qreal width_zoom_level_;
    qreal height_zoom_level_;
    quint32 window_width_;
    quint32 window_height_;
    GamePhase current_active_phase_;

    QGraphicsItemGroup *map_group_;
    Room* room_;
    std::unordered_map<QString, EntityDrawer*> items_;
    QGraphicsView *phase_;
    qreal start_x_;
    qreal start_y_;
};
