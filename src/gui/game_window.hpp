#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include "entity_drawer.hpp"
#include "../map/map.hpp"

class GameWindow : public QGraphicsScene
{
    Q_OBJECT
public:
    static constexpr quint32 WINDOW_WIDTH = 1920u;
    static constexpr quint32 WINDOW_HEIGHT = 1024u;
    GameWindow(Map*, EntityDrawer*, quint32 = WINDOW_WIDTH, quint32 = WINDOW_HEIGHT, QObject* = nullptr);
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
signals:
    void keyPressedSignal(quint32, bool);
    void mousePressedSignal(Qt::MouseButton, bool);
    void wheelScrollSignal(qint32);
    void focusedOutSignal();
    void mousePos(QPointF);
private:
    quint32 window_width_;
    quint32 window_height_;
    GamePhase current_active_phase_;

    Map* map_object_;
    std::unordered_map<QString, Tile*> map_;
    std::unordered_map<QString, EntityDrawer*> items_;
    QGraphicsView *fight_phase_;
};
