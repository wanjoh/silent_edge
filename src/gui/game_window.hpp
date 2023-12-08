#pragma once

#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "entity_drawer.hpp"

class GameWindow : public QGraphicsScene
{
    Q_OBJECT
public:
    static constexpr quint32 WINDOW_WIDTH = 640u;
    static constexpr quint32 WINDOW_HEIGHT = 384u;
    GameWindow(EntityDrawer*, QGraphicsItemGroup*, quint32 = WINDOW_WIDTH, quint32 = WINDOW_HEIGHT, QObject* = nullptr);
    ~GameWindow();

    enum class GamePhase : quint32
    {
        PREP_PHASE = 0,
        FIGHT_PHASE
    };

    void show(GamePhase);
    void addEntity(QString, EntityDrawer*);
    void updatePosition(QString, std::pair<qreal, qreal>, qreal);
    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;
public slots:
    void updateMovement();
signals:
    void keyPressEventSignal(QKeyEvent*);
    void keyReleaseEventSignal(QKeyEvent*);
    void playerMoved();
private:
    void initializeTimers();
    quint32 window_width_;
    quint32 window_height_;
    GamePhase current_active_phase_;
    // --------------------------------------------------
    // hendlovanje igracevog kretanja bi moglo da ide u zasebnu klasu
    QTimer movement_timer_;
    // ovo ce moci i da bude lista od 10ak bulova jer nemamo mnogo dugmica
    std::unordered_map<quint32, bool> keys_;
    EntityDrawer* controllable_player_;
    QGraphicsItemGroup* map_;
    // --------------------------------------------------
    std::unordered_map<QString, EntityDrawer*> items_;
    QGraphicsView *fight_phase_;
};
