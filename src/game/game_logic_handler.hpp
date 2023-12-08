#pragma once

#include <QObject>
#include <QTimer>
#include "../entity/player.hpp"

class GameLogicHandler : public QObject
{
    Q_OBJECT
public:
    static constexpr qreal DEFAULT_PLAYER_VELOCITY = 5.0f;
    static constexpr quint32 TARGET_FPS = 60;

    GameLogicHandler(Player*);
    GameLogicHandler(const GameLogicHandler&) = delete;
    ~GameLogicHandler() = default;

public slots:
    void resetKeys();
    void updateKeys(quint32, bool);
    void updateMovement();
signals:
    void playerMoved();
private:
    void initializeTimers();
    QTimer movement_timer_;
    Player* controllable_player_;
    // ovo ce mozda moci i da bude lista od 10ak bulova jer nemamo mnogo dugmica
    std::unordered_map<quint32, bool> keys_;
};

