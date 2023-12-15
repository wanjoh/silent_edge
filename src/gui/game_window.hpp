#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include "entity_drawer.hpp"
#include "../map/map.hpp"

class GameWindow : public QGraphicsScene
{
    Q_OBJECT
public:
    qreal WINDOW_WIDTH = 1200;
    qreal WINDOW_HEIGHT = 800;
    GameWindow(Map*, EntityDrawer*, qreal = 1280, qreal = 720, QObject* = nullptr);
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
    void make_overlay();
signals:
    void keyPressedSignal(quint32, bool);
    void mousePressedSignal(Qt::MouseButton, bool);
    void wheelScrollSignal(qint32);
    void focusedOutSignal();
    void mousePos(QPointF);
public slots:
    void change_weapon(int id);
    void update_hp_overlay(qreal hp);
private:
    qreal width_zoom_level_ = 1.0;
    qreal height_zoom_level_ = 1.0;
    quint32 window_width_;
    quint32 window_height_;
    GamePhase current_active_phase_;

    Map* map_object_;
    Room* room_;
    std::unordered_map<int, Tile*> map_;
    std::unordered_map<QString, EntityDrawer*> items_;
    QGraphicsView *fight_phase_;
    qreal start_x_;
    qreal start_y_;
    QGraphicsItemGroup* overlay_group_;
};
