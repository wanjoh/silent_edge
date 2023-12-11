#pragma once

#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "entity_drawer.hpp"
#include "tile_drawer.hpp"
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
    void updatePosition(QString, std::pair<qreal, qreal>, qreal);
    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;
    bool canPlayerMove(int, int);
public slots:
    void updateMovement();
    void updateAmmo();
signals:
    void keyPressEventSignal(QKeyEvent*);
    void keyReleaseEventSignal(QKeyEvent*);
    void playerMoved();
    void tileDeleted(QString name);
private:
    void initializeTimers();
    quint32 window_width_;
    quint32 window_height_;
    GamePhase current_active_phase_;
    // --------------------------------------------------
    // hendlovanje igracevog kretanja bi moglo da ide u zasebnu klasu
    QTimer movement_timer_;
    QTimer ammo_timer_;
    // ovo ce moci i da bude lista od 10ak bulova jer nemamo mnogo dugmica
    std::unordered_map<quint32, bool> keys_;
    EntityDrawer* controllable_player_;
    Map* map_object_;
    std::map<QString, Tile*> map_;
    // --------------------------------------------------
    std::unordered_map<QString, EntityDrawer*> items_;
    std::unordered_map<QString, TileDrawer*> tile_drawer_map_;
    QGraphicsView *fight_phase_;
};
